// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/passwdentry.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/passwdentryinlines.h>
#endif

#include <stdio.h>
#include <errno.h>

#define MAXBUFFER	(32*1024)

int passwdentry::initialize(const char *username) {
	if (pwd) {
		pwd=NULL;
		delete[] buffer;
		buffer=NULL;
	}
	#ifdef HAVE_GETPWNAM_R
		// getpwnam_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			if (getpwnam_r(username,&pwdbuffer,
						buffer,size,&pwd)==0) {
				return 1;
			}
			delete[] buffer;
			buffer=NULL;
			pwd=NULL;
			if (errno!=ENOMEM) {
				return 0;
			}
		}
		return 0;
	#else
		return ((pwd=getpwnam(username))!=NULL);
	#endif
}

int passwdentry::initialize(uid_t userid) {
	if (pwd) {
		pwd=NULL;
		delete[] buffer;
		buffer=NULL;
	}
	#ifdef HAVE_GETPWUID_R
		// getpwuid_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			if (getpwuid_r(userid,&pwdbuffer,
						buffer,size,&pwd)==0) {
				return 1;
			}
			delete[] buffer;
			buffer=NULL;
			pwd=NULL;
			if (errno!=ENOMEM) {
				return 0;
			}
		}
		return 0;
	#else
		return ((pwd=getpwuid(userid))!=NULL);
	#endif
}

void passwdentry::print() const {

	if (!pwd) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Password: %s\n",getPassword());
	printf("User Id: %d\n",getUserId());
	printf("Primary Group: %d\n",getPrimaryGroup());
	printf("Real Name: %s\n",getRealName());
	printf("Home Directory: %s\n",getHomeDirectory());
	printf("Shell: %s\n",getShell());
}
