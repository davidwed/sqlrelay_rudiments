// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/shadowentry.h>
#ifndef ENABLE_INLINES
	#include <rudiments/private/shadowentryinlines.h>
#endif

#include <stdio.h>
#include <errno.h>

#define MAXBUFFER	(32*1024)

#if defined(__GNUC__) && !defined(HAVE_GETSPNAM_R)
pthread_mutex_t	*shadowentry::spmutex;
#endif

int shadowentry::initialize(const char *username) {
	if (sp) {
		sp=NULL;
		delete[] buffer;
		buffer=NULL;
	}
	#ifdef HAVE_GETSPNAM_R
		// getspnam_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			if (!getspnam_r(username,&spbuffer,buffer,size,&sp)) {
				return sp!=NULL;
			}
			delete[] buffer;
			buffer=NULL;
			sp=NULL;
			if (errno!=ENOMEM) {
				return 0;
			}
		}
		return 0;
	#else
		return (((spmutex)?!pthread_mutex_lock(spmutex):1) &&
			((sp=getspnam(username))!=NULL) &&
			((spmutex)?!pthread_mutex_unlock(spmutex):1));
	#endif
}

void shadowentry::print() const {

	if (!sp) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Encrypted Password: %s\n",getEncryptedPassword());
	printf("Last Change: %d\n",getLastChangeDate());
	printf("Days Before Change Allowed: %d\n",
				getDaysBeforeChangeAllowed());
	printf("Days Before Change Required: %d\n",
				getDaysBeforeChangeRequired());
	printf("Days Before Expiration Warning: %d\n",
				getDaysBeforeExpirationWarning());
	printf("Days Of Inactivity Allowed: %d\n",
				getDaysOfInactivityAllowed());
	printf("Expiration Date: %d\n",getExpirationDate());
	printf("Flag: %d\n",getFlag());
}
