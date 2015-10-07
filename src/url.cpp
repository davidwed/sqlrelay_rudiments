// Copyright (c) 2015 David Muse
// See the COPYING file for more information

#include <rudiments/url.h>
#ifdef RUDIMENTS_HAS_LIBCURL
	#include <rudiments/bytestring.h>
	#include <rudiments/threadmutex.h>
	#include <rudiments/error.h>
#else
	#include <rudiments/stringbuffer.h>
	#include <rudiments/inetsocketclient.h>
#endif
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

#include <rudiments/private/winsock.h>

//#define DEBUG_CURL 1
//#define DEBUG_HTTP 1

#ifdef RUDIMENTS_HAS_LIBCURL
	#include <curl/curl.h>

	threadmutex	_urlmutex;
	bool		_initialized=false;
#endif

class urlprivate {
	friend class url;
	private:
		#ifdef RUDIMENTS_HAS_LIBCURL
			CURL	*_curl;
			unsigned char	_b[CURL_MAX_WRITE_SIZE];
			uint64_t	_bpos;
			ssize_t		_bsize;
			bool		_fetched;
		#else
			inetsocketclient	_isc;
			uint64_t		_contentlength;
			uint64_t		_fetchedsofar;
		#endif
};

url::url() : file() {
	pvt=new urlprivate;
	dontGetCurrentPropertiesOnOpen();
	#ifdef RUDIMENTS_HAS_LIBCURL
		pvt->_curl=NULL;
	#endif
	init();
	type("url");
	#ifdef RUDIMENTS_HAS_LIBCURL
		winsock::initWinsock();
	#endif
}

url::url(const url &u) : file(u) {
	// no good way to do this
	pvt=new urlprivate;
	type("url");
}

url &url::operator=(const url &u) {
	// no good way to do this
	return *this;
}

url::~url() {
	// filedescriptor's destructor calls close(), why the close() call here?
	// Destructors don't always call overridden methods, but rather the
	// version defined in that class.  In this case, lowLevelClose() needs
	// to be called from this class.  If close() is called here, it will
	// eventually call this method's lowLevelClose() rather than
	// filedescriptor::lowLevelClose().  Also, lowLevelClose() needs to
	// access pvt->_curl or pvt->_isc so we need to call close(), before
	// deleting pvt.
	close();
	delete pvt;
}

void url::init() {
	#ifdef RUDIMENTS_HAS_LIBCURL
		pvt->_bpos=0;
		pvt->_bsize=0;
		pvt->_fetched=false;
	#else
		pvt->_contentlength=0;
		pvt->_fetchedsofar=0;
	#endif
}

void url::lowLevelOpen(const char *name, int32_t flags,
				mode_t perms, bool useperms) {

	// clean up from a previous run
	close();
	init();

	// for now, don't support create or write
	if (perms || useperms || flags&O_WRONLY || flags&O_RDWR) {
		return;
	}

	// don't support local files
	const char	*protodelim=charstring::findFirst(name,"://");
	if (!protodelim || !charstring::compare(name,"file://",7)) {
		return;
	}

	// extract user and password (and combined userpwd) from url...
	#if defined(RUDIMENTS_HAS_CURLOPT_USERNAME)
	char	*user=NULL;
	char	*password=NULL;
	#endif
	char	*userpwd=NULL;
	char	*cleanurl=NULL;

	const char	*at=charstring::findFirst(protodelim+3,'@');
	if (at) {

		userpwd=charstring::duplicate(protodelim+3,at-protodelim-3);

		#if defined(RUDIMENTS_HAS_CURLOPT_USERNAME)
		user=userpwd;
		password=charstring::findFirst(userpwd,':');
		if (password) {
			password++;
			*(password-1)='\0';
		}
		#endif

		cleanurl=new char[charstring::length(name)+1];
		charstring::copy(cleanurl,name,protodelim+3-name);
		charstring::append(cleanurl,at+1);
	}
	if (!userpwd) {
		cleanurl=charstring::duplicate(name);
	}


	// init the framework, if necessary
	if (!initUrl()) {
		return;
	}

	#ifdef RUDIMENTS_HAS_LIBCURL

		// init this curl instance
		pvt->_curl=curl_easy_init();
		if (!pvt->_curl) {
			return;
		}

		// variable to hold the socket...
		#ifdef RUDIMENTS_HAS_CURLINFO_ACTIVESOCKET
		curl_socket_t	s;
		#else
		long	s;
		#endif

		// clear any existing errors
		error::clearError();

		#ifdef DEBUG_CURL
		stdoutput.printf("url: \"%s\"\n",cleanurl);
		stdoutput.printf("userpwd: \"%s\"\n",userpwd);
		#if defined(RUDIMENTS_HAS_CURLOPT_USERNAME)
		stdoutput.printf("user: \"%s\"\n",user);
		stdoutput.printf("password: \"%s\"\n",password);
		#endif
		#endif

		// make the connection...
		if (
			#ifdef DEBUG_CURL
			curl_easy_setopt(pvt->_curl,
				CURLOPT_VERBOSE,1)==CURLE_OK &&
			#endif

			// set up the url to open
			curl_easy_setopt(pvt->_curl,
				CURLOPT_URL,cleanurl)==CURLE_OK &&

			#if defined(RUDIMENTS_HAS_CURLOPT_USERNAME)
			// set the user
			(!user ||
			curl_easy_setopt(pvt->_curl,
				CURLOPT_USERNAME,user)==CURLE_OK) &&

			// set the password
			(!password ||
			curl_easy_setopt(pvt->_curl,
				CURLOPT_PASSWORD,password)==CURLE_OK) &&

			#elif defined(RUDIMENTS_HAS_CURLOPT_USERPWD)

			// set the user/password
			(!userpwd ||
			curl_easy_setopt(pvt->_curl,
				CURLOPT_USERPWD,userpwd)==CURLE_OK) &&
			#endif

			// if a password is supplied, then use password
			// authentication for ssh protocols, or otherwise
			// allow curl to choose an appropriate auth type
			#ifdef RUDIMENTS_HAS_CURLOPT_SSH_AUTH_TYPES
			((password &&
			curl_easy_setopt(pvt->_curl,
				CURLOPT_SSH_AUTH_TYPES,
				CURLSSH_AUTH_PASSWORD)==CURLE_OK) ||
			curl_easy_setopt(pvt->_curl,
				CURLOPT_SSH_AUTH_TYPES,
				CURLSSH_AUTH_ANY)==CURLE_OK) &&
			#endif
			

			// set up write handler
			curl_easy_setopt(pvt->_curl,
				CURLOPT_WRITEFUNCTION,writeData)==CURLE_OK &&
			curl_easy_setopt(pvt->_curl,
				CURLOPT_WRITEDATA,this)==CURLE_OK &&

			// just connect during the first "perform" call
			curl_easy_setopt(pvt->_curl,
				CURLOPT_CONNECT_ONLY,1)==CURLE_OK &&

			// connect
			curl_easy_perform(pvt->_curl)==CURLE_OK &&

			// now configure subsequent "perform" calls to
			// actually read data
			curl_easy_setopt(pvt->_curl,
				CURLOPT_CONNECT_ONLY,0)==CURLE_OK &&

			// get the file descriptor
			// FIXME: this doesn't work with file:// urls
			// Also, this looks odd...  Why not just ifdef wrap the
			// CURLINFO_ACTIVESOCKET/CURLINFO_LASTSOCKET parameter?
			// For some reason that doesn't work with some
			// preprocessors.  Namely Haiku's, but probably others
			// as well.
			#ifdef RUDIMENTS_HAS_CURLINFO_ACTIVESOCKET
			curl_easy_getinfo(pvt->_curl,
				CURLINFO_ACTIVESOCKET,&s)==CURLE_OK
			#else
			curl_easy_getinfo(pvt->_curl,
				CURLINFO_LASTSOCKET,&s)==CURLE_OK
			#endif
			) {

			#ifdef DEBUG_CURL
			stdoutput.printf("open succeeded, fd: %d\n",s);
			#endif

			// set the file descriptor
			fd(s);

		} else {

			#ifdef DEBUG_CURL
			stdoutput.printf("open failed\n");
			#endif

			close();
		}

	#else

		// http connect...
		if (!charstring::compare(cleanurl,"http://",7)) {

			// parse out the host, port and path
			protodelim=charstring::findFirst(cleanurl,"://");
			const char	*path=
				charstring::findFirstOrEnd(protodelim+3,'/');
			char	*host=charstring::duplicate(protodelim+3,
							path-protodelim-3);
			const char	*port="80";
			char	*colon=charstring::findFirst(host,':');
			if (colon) {
				port=colon+1;
				*colon='\0';
			}

			// connect and request file
			if (http(host,charstring::toInteger(port),
							userpwd,path)) {

				#ifdef DEBUG_HTTP
				stdoutput.printf("open succeeded, fd: %d\n",
						pvt->_isc.getFileDescriptor());
				#endif

				fd(pvt->_isc.getFileDescriptor());

			} else {

				#ifdef DEBUG_HTTP
				stdoutput.printf("open failed\n");
				#endif

				close();
			}
		}

	#endif

	delete[] cleanurl;
	delete[] userpwd;
}

bool url::http(const char *host,
			uint16_t port,
			const char *userpwd,
			const char *path) {

	#ifdef RUDIMENTS_HAS_LIBCURL
		return false;
	#else
		#ifdef DEBUG_HTTP
		stdoutput.printf("host: %s\n",host),
		stdoutput.printf("port: %d\n",port);
		stdoutput.printf("userpwd: %s\n\n",userpwd);
		#endif

		// connect to the host
		if (!pvt->_isc.connect(host,port,-1,-1,0,0)) {
			#ifdef DEBUG_HTTP
			stdoutput.printf("http connect failed\n");
			#endif
			return false;
		}

		// build the request
		stringbuffer	request;
		request.append("GET ")->append(path);
		request.append(" HTTP/1.1\r\n");
		request.append("Host: ")->append(host)->append("\r\n");
		if (userpwd) {
			request.append("Authorization: Basic ");
			char	*userpwd64=charstring::base64Encode(
						(const unsigned char *)userpwd);
			request.append(userpwd64);
			delete[] userpwd64;
			request.append("\r\n");
		}
		request.append("\r\n");

		// send the request
		#ifdef DEBUG_HTTP
		stdoutput.printf("Request:\n%s",request.getString());
		#endif
		if (pvt->_isc.write(request.getString(),
				request.getStringLength())!=
				(ssize_t)request.getStringLength()) {
			#ifdef DEBUG_HTTP
			stdoutput.printf("http send request failed\n");
			#endif
			return false;
		}

		// fetch the headers
		char	*headers=NULL;
		if (pvt->_isc.read(&headers,"\r\n\r\n",-1,-1)<4) {
			#ifdef DEBUG_HTTP
			stdoutput.printf("http fetch headers failed\n");
			#endif
			delete[] headers;
			return false;
		}
		#ifdef DEBUG_HTTP
		stdoutput.printf("Response Headers:\n%s",headers);
		#endif

		// get the content-length
		const char	*cs=charstring::findFirst(
						headers,"Content-Length: ");
		if (!cs) {
			#ifdef DEBUG_HTTP
			stdoutput.printf("http content length not found\n");
			#endif
			delete[] headers;
			return false;
		}
		pvt->_contentlength=charstring::toInteger(cs+16);
		delete[] headers;

		return true;
	#endif
}

int32_t url::lowLevelClose() {
	#ifdef RUDIMENTS_HAS_LIBCURL
		if (pvt->_curl) {
			curl_easy_cleanup(pvt->_curl);
		}
		return 0;
	#else
		int32_t	retval=pvt->_isc.lowLevelClose();
		if (!retval) {
			pvt->_isc.setFileDescriptor(-1);
		}
		return retval;
	#endif
}

ssize_t url::lowLevelRead(void *buffer, ssize_t size) const {

	#ifdef RUDIMENTS_HAS_LIBCURL

		// buffer some data, if necessary
		if (pvt->_bsize==0 && !pvt->_fetched) {

			error::clearError();

			if (curl_easy_perform(pvt->_curl)) {
				return -1;
			}

			// ignore EAGAIN, even if this succeeds,
			// the error number is sometimes set to EAGAIN
			if (error::getErrorNumber()==EAGAIN) {
				error::clearError();
			}
		}

		// don't attempt to return more bytes
		// than there are in the buffer
		if (size>pvt->_bsize) {
			size=pvt->_bsize;
		}

		// copy data from the buffer
		bytestring::copy(buffer,pvt->_b+pvt->_bpos,size);

		// adjust the buffer position and size
		pvt->_bpos=pvt->_bpos+size;
		pvt->_bsize=pvt->_bsize-size;

		// return how much was actually read
		return size;
	#else
		if (pvt->_fetchedsofar>=pvt->_contentlength) {
			return 0;
		}
		ssize_t	bytesread=pvt->_isc.lowLevelRead(buffer,size);
		pvt->_fetchedsofar+=bytesread;
		return bytesread;
	#endif
}

bool url::initUrl() {
	bool	result=true;
	#ifdef RUDIMENTS_HAS_LIBCURL
		_urlmutex.lock();
		if (!_initialized) {
			result=!curl_global_init(CURL_GLOBAL_ALL);
		}
		_urlmutex.unlock();
	#endif
	return result;
}

void url::shutDownUrl() {
	#ifdef RUDIMENTS_HAS_LIBCURL
		if (_initialized) {
			curl_global_cleanup();
		}
	#endif
}

size_t url::writeData(void *buffer, size_t size, size_t nmemb, void *userp) {

	#ifdef RUDIMENTS_HAS_LIBCURL

		#ifdef DEBUG_CURL
			stdoutput.printf("writeData(%d,%d) (%d bytes)\n",
							size,nmemb,size*nmemb);
		#endif

		url	*u=(url *)userp;

		// get the actual size (in bytes)
		size=size*nmemb;

		// copy data into the buffer
		bytestring::copy(u->pvt->_b,buffer,size);

		// adjust the buffer position and size
		u->pvt->_bpos=0;
		u->pvt->_bsize=size;

		// did we fetch the entire file?
		u->pvt->_fetched=(size<sizeof(u->pvt->_b));

		#ifdef DEBUG_CURL
			stdoutput.printf("    %d bytes buffered\n",size);
		#endif

		// return how much data was buffered
		return size;
	#else
		return 0;
	#endif
}
