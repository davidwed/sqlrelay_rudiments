// Copyright (c) 2015 David Muse
// See the COPYING file for more information


#include <rudiments/url.h>
#include <rudiments/threadmutex.h>
#include <rudiments/stdio.h>

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
		CURLM	*_curlm;
		bool	_curlminitsuccess;
		int32_t	_stillrunning;
	#endif
};

url::url() : file() {
	pvt=new urlprivate;
	#ifdef RUDIMENTS_HAS_LIBCURL
		pvt->_curl=NULL;
		pvt->_curlm=NULL;
		pvt->_curlminitsuccess=false;
		pvt->_stillrunning=0;
	#endif
	type("url");
}

url::url(const url &u) : file(u) {
	pvt=new urlprivate;
	urlClone(u);
	type("url");
}

url &url::operator=(const url &u) {
	if (this!=&u) {
		file::operator=(u);
		urlClone(u);
	}
	return *this;
}

void url::urlClone(const url &f) {
	#ifdef RUDIMENTS_HAS_LIBCURL
		pvt->_curl=f.pvt->_curl;
	#endif
}

url::~url() {
	delete pvt;
}

void url::lowLevelOpen(const char *name, int32_t flags,
				mode_t perms, bool useperms) {

	#ifdef RUDIMENTS_HAS_LIBCURL

		// clean up from a previous run
		close();

		// for now, we don't support create or write
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

		// init a "multi" instance
		// (so we can get the file descriptor later)
		pvt->_curlm=curl_multi_init();
		if (!pvt->_curlm) {
			return;
		}

		// add this instance to the multi
		pvt->_curlminitsuccess=
			!curl_multi_add_handle(pvt->_curlm,pvt->_curl);
		if (pvt->_curlminitsuccess) {
			return;
		}

		// tell it what url to fetch
		curl_easy_setopt(pvt->_curl,CURLOPT_URL,name);

		// tell it what write function and data to use
		curl_easy_setopt(pvt->_curl,CURLOPT_WRITEFUNCTION,
							url::writeData);
		curl_easy_setopt(pvt->_curl,CURLOPT_WRITEDATA,this);

		// begin the transfer
		curl_multi_perform(pvt->_curlm,(int *)&pvt->_stillrunning);

		// extract the file descriptor
		fd_set	fdread;
		int32_t	maxfd;
		FD_ZERO(&fdread);
		curl_multi_fdset(pvt->_curlm,&fdread,NULL,NULL,(int *)&maxfd);
		for (int32_t i=0; i<maxfd; i++) {
			if (FD_ISSET(i,&fdread)) {
				fd(i);
				break;
			}
		}
	#endif
}

int32_t url::lowLevelClose() {
	#ifdef RUDIMENTS_HAS_LIBCURL
		if (pvt->_curlminitsuccess) {
			curl_multi_remove_handle(pvt->_curlm,pvt->_curl);
		}
		if (pvt->_curlm) {
			curl_multi_cleanup(pvt->_curlm);
		}
		if (pvt->_curl) {
			curl_easy_cleanup(pvt->_curl);
		}
		return 0;
	#else
		return 1;
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
	// FIXME: implement this for real
	stdoutput.write((unsigned char *)buffer,nmemb);
	return nmemb;
}
