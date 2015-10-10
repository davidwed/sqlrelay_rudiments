// Copyright (c) 2015 David Muse
// See the COPYING file for more information

#include <rudiments/url.h>
#ifdef RUDIMENTS_HAS_LIBCURL
	#include <rudiments/bytestring.h>
	#include <rudiments/threadmutex.h>
	#include <rudiments/error.h>
	#include <rudiments/snooze.h>
	#include <rudiments/listener.h>
#endif
#include <rudiments/stringbuffer.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/charstring.h>
#include <rudiments/character.h>
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
		inetsocketclient	_isc;
		uint64_t		_contentlength;
		uint64_t		_fetchedsofar;
		bool			_usingbuiltin;
		#ifdef RUDIMENTS_HAS_LIBCURL
			CURL	*_curl;
			CURLM	*_curlm;
			unsigned char	_b[CURL_MAX_WRITE_SIZE];
			uint64_t	_breadpos;
			ssize_t		_bsize;
			bool		_eof;
			int32_t		_stillrunning;
			listener	_l;
		#endif
};

url::url() : file() {

	pvt=new urlprivate;
	dontGetCurrentPropertiesOnOpen();

	#ifdef RUDIMENTS_HAS_LIBCURL
	pvt->_curl=NULL;
	pvt->_curlm=NULL;
	#endif

	init();
	type("url");

	winsock::initWinsock();
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

	pvt->_contentlength=0;
	pvt->_fetchedsofar=0;
	pvt->_usingbuiltin=true;

	#ifdef RUDIMENTS_HAS_LIBCURL
	pvt->_breadpos=0;
	pvt->_bsize=0;
	pvt->_eof=false;
	pvt->_stillrunning=0;
	#endif
}

bool url::lowLevelOpen(const char *name, int32_t flags,
				mode_t perms, bool useperms) {

	// clean up from a previous run
	close();
	init();

	// for now, don't support create or write
	if (perms || useperms || flags&O_WRONLY || flags&O_RDWR) {
		return false;
	}

	// skip leading whitespace
	while (*name && character::isWhitespace(*name)) {
		name++;
	}

	// don't support local files
	const char	*protodelim=charstring::findFirst(name,"://");
	if (!protodelim || !charstring::compare(name,"file://",7)) {
		return false;
	}

	// extract user and password (and combined userpwd) from url...
	#if defined(RUDIMENTS_HAS_CURLOPT_USERNAME)
	char	*user=NULL;
	char	*password=NULL;
	#endif
	char	*userpwd=NULL;
	char	*cleanurl=NULL;

	// is there an @ in the url?
	const char	*at=charstring::findFirst(protodelim+3,'@');
	if (at) {

		// extract the user/password block
		userpwd=charstring::duplicate(protodelim+3,at-protodelim-3);

		// handle password files
		if (userpwd[0]=='[' &&
			userpwd[charstring::length(userpwd)-1]==']') {

			charstring::rightTrim(userpwd,']');

			char	*temp=file::getContents(userpwd+1);
			charstring::bothTrim(temp);

			delete[] userpwd;
			userpwd=temp;
		}

		// build a clean url, without the user/password in it
		cleanurl=new char[charstring::length(name)+1];
		charstring::copy(cleanurl,name,protodelim+3-name);
		charstring::append(cleanurl,at+1);

	} else {
		cleanurl=charstring::duplicate(name);
	}


	// init the framework, if necessary
	if (!initUrl()) {
		return false;
	}

	bool	retval=false;

	// http...
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

			retval=true;

		} else {

			#ifdef DEBUG_HTTP
			stdoutput.printf("open failed\n");
			#endif

			close();
		}

	#ifdef RUDIMENTS_HAS_LIBCURL
	} else {

		// not using built-in protocol handler
		pvt->_usingbuiltin=false;

		// clear any existing errors
		error::clearError();

		// split to separate user/password if necessary
		#if defined(RUDIMENTS_HAS_CURLOPT_USERNAME)
		if (userpwd) {
			user=userpwd;
			password=charstring::findFirst(userpwd,':');
			if (password) {
				password++;
				*(password-1)='\0';
			}
		}
		#endif

		#ifdef DEBUG_CURL
		stdoutput.printf("url: \"%s\"\n",cleanurl);
		stdoutput.printf("userpwd: \"%s\"\n",userpwd);
		#if defined(RUDIMENTS_HAS_CURLOPT_USERNAME)
		stdoutput.printf("user: \"%s\"\n",user);
		stdoutput.printf("password: \"%s\"\n",password);
		#endif
		#endif

		if (
			// init this curl instance
			(pvt->_curl=curl_easy_init()) &&

			// set up debug
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
				CURLOPT_WRITEFUNCTION,readData)==CURLE_OK &&
			curl_easy_setopt(pvt->_curl,
				CURLOPT_WRITEDATA,this)==CURLE_OK &&

			// init the multi instance
			(pvt->_curlm=curl_multi_init()) &&

			// add the curl instance to the multi instance
			curl_multi_add_handle(pvt->_curlm,
				pvt->_curl)==CURLM_OK &&

			// connect
			perform()) {

			// It's actually possible for all of this above
			// to succeed, but for the file descriptor to
			// still be -1.
			//
			// As soon as curl has read the entire file, it
			// closes the connection and invalidates the
			// descriptor.
			//
			// If the file is small enough, that'll all happen
			// during a single call to curl_multi_perform().
			//
			// I tried telling it to just connect and not read
			// anything, but even then, it appears to buffer the
			// data, close the connection and invalidate the
			// file descriptor anyway, it just doesn't call its
			// "I've got data callback" until the next call
			// to curl_multi_perform().
			//
			// Even when you can get a hold of it, the file
			// descriptor appears to be in non-blocking mode,
			// and setting it otherwise appears to break curl.
			//
			// It seems a bit of a ponderous heap, actually.
			// I'm sure there's some paradigm that it fits neatly
			// into, but definitely not into the unix "everything's
			// a file" or Windows "everything's a handle" paradigms.

			#ifdef DEBUG_CURL
			stdoutput.printf("open succeeded, fd: %d\n",
						getFileDescriptor());
			#endif

			retval=true;

		} else {

			#ifdef DEBUG_CURL
			stdoutput.printf("open failed\n");
			#endif

			close();
		}
	}
	#endif

	delete[] cleanurl;
	delete[] userpwd;

	return retval;
}

bool url::http(const char *host,
			uint16_t port,
			const char *userpwd,
			const char *path) {

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
	const char	*cs=charstring::findFirst(headers,"Content-Length: ");
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
}

int32_t url::lowLevelClose() {

	if (pvt->_usingbuiltin) {

		int32_t	retval=pvt->_isc.lowLevelClose();
		if (!retval) {
			pvt->_isc.setFileDescriptor(-1);
		}
		return retval;

	#ifdef RUDIMENTS_HAS_LIBCURL
	} else {

		if (pvt->_curlm && pvt->_curl) {
			curl_multi_remove_handle(pvt->_curlm,pvt->_curl);
		}
		if (pvt->_curl) {
			curl_easy_cleanup(pvt->_curl);
			pvt->_curl=NULL;
		}
		if (pvt->_curlm) {
			curl_multi_cleanup(pvt->_curlm);
			pvt->_curlm=NULL;
		}
		return 0;
	}
	#endif
}

ssize_t url::lowLevelRead(void *buffer, ssize_t size) {

	if (pvt->_usingbuiltin) {

		// eof condition
		if (pvt->_fetchedsofar>=pvt->_contentlength) {
			return 0;
		}

		// read some data
		ssize_t	bytesread=pvt->_isc.lowLevelRead(buffer,size);

		// update the total bytes read
		// (so we can decide later if we've hit an eof)
		pvt->_fetchedsofar+=bytesread;

		return bytesread;

	#ifdef RUDIMENTS_HAS_LIBCURL
	} else {

		// buffer some data, if necessary
		if (pvt->_bsize==0 && !pvt->_eof) {

			error::clearError();

			#ifdef DEBUG_CURL
				stdoutput.printf("\n\ncalling perform()\n");
			#endif
			if (!perform()) {
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
		bytestring::copy(buffer,pvt->_b+pvt->_breadpos,size);

		// adjust the buffer position and size
		pvt->_breadpos=pvt->_breadpos+size;
		pvt->_bsize=pvt->_bsize-size;

		// return how much was actually read
		return size;
	}
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

bool url::perform() {

	#ifdef RUDIMENTS_HAS_LIBCURL

	do {
		// Later, we're going to have to wait.
		// Figure out how long we shold wait here.
		uint32_t	sec=60;
		uint32_t	usec=0;
		long		timeout=-1;
		if (curl_multi_timeout(pvt->_curlm,&timeout)!=CURLM_OK) {
			return false;
		}
		if (timeout>=0) {
			sec=timeout/1000;
			if (sec>1) {
				sec=1;
			} else {
				usec=(timeout%1000)*1000;
			}
		}

		// Try to get the file descriptors associated with this thing.
		int	maxfd=-1;
		fd_set	fdread;
		fd_set	fdwrite;
		fd_set	fdexecp;
		FD_ZERO(&fdread);
		FD_ZERO(&fdwrite);
		FD_ZERO(&fdexecp);
		if (curl_multi_fdset(pvt->_curlm,
				&fdread,&fdwrite,&fdexecp,&maxfd)!=CURLM_OK) {
			return false;
		}

		// Here's that wait I was talking about...
		if (maxfd==-1) {

			// if we weren't able to get a file descriptor,
			// then just wait for 100ms
			snooze::microsnooze(0,100000);

		} else {

			// if we were able to get a file descriptor...

			// if the class doesn't already have a file
			// descriptor associated with it...
			if (fd()==-1) {

				// figure out which fd it was and set the
				// class' file descriptor to it
				for (int32_t i=0; i<=maxfd; i++) {

					// turns out we have to check both,
					// even if we're only reading
					if (FD_ISSET(i,&fdread) ||
						FD_ISSET(i,&fdwrite)) {
						fd(i);
						break;
					}
				}

				pvt->_l.addFileDescriptor(this);
			}

			// now, instead of just waiting, we'll listen on
			// the file descriptor, and time out after the
			// recomended amount of time...
			if (pvt->_l.listen(sec,usec)==RESULT_ERROR) {
				return false;
			}
		}

		// and now, hopefully, some data will be ready
		if (curl_multi_perform(pvt->_curlm,
				(int *)&pvt->_stillrunning)!=CURLM_OK) {
			return false;
		}

	// if no data is available though, or if we just need to
	// try and get more data, then loop back and try again
	} while (pvt->_bsize==0 && pvt->_stillrunning);

	return true;

	#else

	return false;

	#endif
}

size_t url::readData(void *buffer, size_t size, size_t nmemb, void *userp) {

	#ifdef RUDIMENTS_HAS_LIBCURL

	#ifdef DEBUG_CURL
		stdoutput.printf("readData(%d,%d) (%d bytes)\n",
						size,nmemb,size*nmemb);
	#endif

	url	*u=(url *)userp;

	// get the actual size (in bytes)
	size=size*nmemb;

	// copy data into the buffer
	bytestring::copy(u->pvt->_b,buffer,size);

	// adjust the buffer position and size
	u->pvt->_breadpos=0;
	u->pvt->_bsize=size;

	// did we fetch the entire file?
	u->pvt->_eof=!u->pvt->_bsize;

	#ifdef DEBUG_CURL
		stdoutput.printf("    %d bytes buffered\n",size);
	#endif

	// return how much data was buffered
	return size;

	#else

	return 0;

	#endif
}
