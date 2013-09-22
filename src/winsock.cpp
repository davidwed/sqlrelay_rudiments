// Copyright (c) 2011 David Muse
// See the COPYING file for more information

#include <rudiments/private/winsock.h>

#ifdef RUDIMENTS_HAVE_WINSOCK2_H
	threadmutex	winsock::_winsockmutex;
	bool		winsock::_initialized=false;
	WSADATA		winsock::_wsadata;
#endif

bool winsock::initWinsock() {
	#ifdef RUDIMENTS_HAVE_WINSOCK2_H
		_winsockmutex.lock();
		if (!_initialized) {
			int32_t	result=WSAStartup(MAKEWORD(2,0),&_wsadata);
			_initialized=true;
			if (!result) {
				if (LOBYTE(_wsadata.wVersion)!=2 ||
					HIBYTE(_wsadata.wVersion)!=0) {
					WSACleanup();
					result=1;
				} else {
					atexit(winsock::shutDownWinsock);
				}
			}
			_winsockmutex.unlock();
			return !result;
		}
		_winsockmutex.unlock();
	#endif
	return true;
}

void winsock::shutDownWinsock() {
	#ifdef RUDIMENTS_HAVE_WINSOCK2_H
		if (_initialized) {
			WSACleanup();
		}
	#endif
}
