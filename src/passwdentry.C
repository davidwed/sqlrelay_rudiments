// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/passwdentry.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/passwdentryinlines.h>
#endif

int passwdentry::initialize(const char *username) {
	if (pwd) {
		delete pwd;
	}
	#ifdef HAVE_GETPWNAM_R
		pwd=new passwd;
		return (getpwnam_r(username,pwd,buffer,1024,&pwd)==0);
	#else
		return ((pwd=getpwnam(username))!=NULL);
	#endif
}

int passwdentry::initialize(uid_t userid) {
	if (pwd) {
		delete pwd;
	}
	#ifdef HAVE_GETPWUID_R
		pwd=new passwd;
		return (getpwuid_r(userid,pwd,buffer,1024,&pwd)==0);
	#else
		return ((pwd=getpwuid(userid))!=NULL);
	#endif
}
