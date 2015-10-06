// Copyright (c) 2015 David Muse
// See the COPYING file for more information

#include <rudiments/url.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/bytestring.h>
#include <rudiments/threadmutex.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>

//#define DEBUG_CURL 1

#ifdef RUDIMENTS_HAS_LIBCURL
	#include <curl/curl.h>
#endif

threadmutex	_urlmutex;
bool		_initialized=false;

class urlprivate {
	friend class url;
	private:
		#ifdef RUDIMENTS_HAS_LIBCURL
			CURL	*_curl;
			unsigned char	_b[CURL_MAX_WRITE_SIZE];
			uint64_t	_bpos;
			ssize_t		_bsize;
			bool		_fetched;
		#endif
};

url::url() : file() {
	pvt=new urlprivate;
	#ifdef RUDIMENTS_HAS_LIBCURL
		pvt->_curl=NULL;
	#endif
	init();
	type("url");
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
	delete pvt;
}

void url::init() {
	#ifdef RUDIMENTS_HAS_LIBCURL
		pvt->_bpos=0;
		pvt->_bsize=0;
		pvt->_fetched=false;
	#endif
}

void url::lowLevelOpen(const char *name, int32_t flags,
				mode_t perms, bool useperms) {

	#ifdef RUDIMENTS_HAS_LIBCURL

		// clean up from a previous run
		close();
		init();

		// for now, don't support create or write
		if (perms || useperms || flags&O_WRONLY || flags&O_RDWR) {
			return;
		}

		// init all of curl, if necessary
		if (!initUrl()) {
			return;
		}

		// init this instance
		pvt->_curl=curl_easy_init();
		if (!pvt->_curl) {
			return;
		}

		// extract user/password from url...
		#if defined(RUDIMENTS_HAS_CURLOPT_USERNAME)
		char	*user=NULL;
		char	*password=NULL;
		#endif
		char	*userpwd=NULL;
		char	*cleanurl=NULL;

		const char	*protodelim=charstring::findFirst(name,"://");
		if (protodelim) {

			const char	*at=
				charstring::findFirst(protodelim+3,'@');
			if (at) {

				userpwd=charstring::duplicate(
							protodelim+3,
							at-protodelim-3);
				#if defined(RUDIMENTS_HAS_CURLOPT_USERNAME)
				user=userpwd;
				password=charstring::findFirst(userpwd,':');
				if (password) {
					password++;
					*(password-1)='\0';
				}
				#endif

				cleanurl=new char[charstring::length(name)+1];
				charstring::copy(cleanurl,name,
							protodelim+3-name);
				charstring::append(cleanurl,at+1);
			}
		}
		if (!userpwd) {
			cleanurl=charstring::duplicate(name);
		}

		// variable to hold the socket...
		#ifdef RUDIMENTS_HAS_CURLINFO_ACTIVESOCKET
		curl_socket_t	s;
		#else
		long	s;
		#endif

		// clear any existing errors
		error::clearError();

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

			// set the file descriptor
			fd(s);
		} else {
			close();
		}

		delete[] cleanurl;
		delete[] userpwd;
	#endif
}

int32_t url::lowLevelClose() {
	#ifdef RUDIMENTS_HAS_LIBCURL
		if (pvt->_curl) {
			curl_easy_cleanup(pvt->_curl);
		}
		return 0;
	#else
		return 1;
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
		return -1;
	#endif
}

bool url::initUrl() {
	bool	result=false;
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

		// return how much data was buffered
		return size;
	#else
		return 0;
	#endif
}
