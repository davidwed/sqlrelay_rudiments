// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/shadowentry.h>
#ifndef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/shadowentryinlines.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#include <errno.h>

#define MAXBUFFER	(32*1024)

#if defined(__GNUC__) && !defined(HAVE_GETSPNAM_R)
pthread_mutex_t	*shadowentry::spmutex;
#endif

bool shadowentry::initialize(const char *username) {
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
				return (sp!=NULL);
			}
			delete[] buffer;
			buffer=NULL;
			sp=NULL;
			if (errno!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		return (((spmutex)?!pthread_mutex_lock(spmutex):true) &&
			((sp=getspnam((char *)username))!=NULL) &&
			((spmutex)?!pthread_mutex_unlock(spmutex):true));
	#endif
}

bool shadowentry::getEncryptedPassword(const char *username, char **password) {
	shadowentry	sp;
	if (sp.initialize(username)) {
		*password=strdup(sp.getEncryptedPassword());
		return true;
	}
	return false;
}

bool shadowentry::getLastChangeDate(const char *username, long *lstchg) {
	shadowentry	sp;
	if (sp.initialize(username)) {
		*lstchg=sp.getLastChangeDate();
		return true;
	}
	return false;
}

bool shadowentry::getDaysBeforeChangeAllowed(const char *username, int *min) {
	shadowentry	sp;
	if (sp.initialize(username)) {
		*min=sp.getDaysBeforeChangeAllowed();
		return true;
	}
	return false;
}

bool shadowentry::getDaysBeforeChangeRequired(const char *username, int *max) {
	shadowentry	sp;
	if (sp.initialize(username)) {
		*max=sp.getDaysBeforeChangeRequired();
		return true;
	}
	return false;
}

bool shadowentry::getDaysBeforeExpirationWarning(const char *username,
								int *warn) {
#ifdef HAVE_SP_WARN
	shadowentry	sp;
	if (sp.initialize(username)) {
		*warn=sp.getDaysBeforeExpirationWarning();
		return true;
	}
	return false;
#else
	*warn=-1;
	return true;
#endif
}

bool shadowentry::getDaysOfInactivityAllowed(const char *username, int *inact) {
#ifdef HAVE_SP_INACT
	shadowentry	sp;
	if (sp.initialize(username)) {
		*inact=sp.getDaysOfInactivityAllowed();
		return true;
	}
	return false;
#else
	*inact=-1;
	return true;
#endif
}

bool shadowentry::getExpirationDate(const char *username, int *expire) {
#ifdef HAVE_SP_EXPIRE
	shadowentry	sp;
	if (sp.initialize(username)) {
		*expire=sp.getExpirationDate();
		return true;
	}
	return false;
#else
	*expire=-1;
	return true;
#endif
}

bool shadowentry::getFlag(const char *username, int *flag) {
#ifdef HAVE_SP_FLAG
	shadowentry	sp;
	if (sp.initialize(username)) {
		*flag=sp.getFlag();
		return true;
	}
	return false;
#else
	*flag=-1;
	return true;
#endif
}

void shadowentry::print() const {

	if (!sp) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Encrypted Password: %s\n",getEncryptedPassword());
	printf("Last Change: %ld\n",getLastChangeDate());
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
