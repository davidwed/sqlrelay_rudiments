// Copyright (c) 2011 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_WINSOCK_H
#define RUDIMENTS_WINSOCK_H

#include <rudiments/private/config.h>

#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif

#ifdef RUDIMENTS_HAVE_WINSOCK2_H
	#include <rudiments/mutex.h>
	#include <winsock2.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class winsock {
	public:
		static	bool	initWinsock();
		static	void	shutDownWinsock();
	private:
		#ifdef RUDIMENTS_HAVE_WINSOCK2_H
			static	bool	_initialized;
			static	mutex	_winsockmutex;
			static	WSADATA	_wsadata;
		#endif
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
