// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/passwdentry.h>
#ifndef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/passwdentryinlines.h>
#endif

#include <stdio.h>
#include <errno.h>

#define MAXBUFFER	(32*1024)

#if defined(__GNUC__) && \
	(!defined(HAVE_GETPWNAM_R) || !defined(HAVE_GETPWUID_R))
pthread_mutex_t	*passwdentry::pemutex;
#endif

bool passwdentry::initialize(const char *username, uid_t userid) {
	if (pwd) {
		pwd=NULL;
		delete[] buffer;
		buffer=NULL;
	}
	#if defined(HAVE_GETPWNAM_R) && defined(HAVE_GETPWUID_R)
		// getpwnam_r and getpwuid_r are goofy.
		// They will retrieve an arbitrarily large amount of data, but
		// require that you pass them a pre-allocated buffer.  If the
		// buffer is too small, they returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			if (!((username)
				?(getpwnam_r(username,&pwdbuffer,
							buffer,size,&pwd))
				:(getpwuid_r(userid,&pwdbuffer,
							buffer,size,&pwd)))) {
				return (pwd!=NULL);
			}
			delete[] buffer;
			buffer=NULL;
			pwd=NULL;
			if (errno!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		return (((pemutex)?!pthread_mutex_lock(pemutex):true) &&
				((pwd=((username)
					?getpwnam(username)
					:getpwuid(userid)))!=NULL) &&
			((pemutex)?!pthread_mutex_unlock(pemutex):true));
	#endif
}

void passwdentry::print() const {

	if (!pwd) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Password: %s\n",getPassword());
	printf("User Id: %d\n",getUserId());
	printf("Primary Group Id: %d\n",getPrimaryGroupId());
	printf("Real Name: %s\n",getRealName());
	printf("Home Directory: %s\n",getHomeDirectory());
	printf("Shell: %s\n",getShell());
}
