// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/groupentry.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/groupentryinlines.h>
#endif

int groupentry::initialize(const char *groupname) {
	if (grp) {
		delete grp;
	}
	#ifdef HAVE_GETGRNAM_R
		grp=new group;
		char	buffer[1024];
		return (getgrnam_r(groupname,grp,buffer,1024,&grp)==0);
	#else
		return ((grp=getgrnam(groupname))!=NULL);
	#endif
}

int groupentry::initialize(gid_t groupid) {
	if (grp) {
		delete grp;
	}
	#ifdef HAVE_GETPWUID_R
		grp=new group;
		char	buffer[1024];
		return (getgrgid_r(groupid,grp,buffer,1024,&grp)==0);
	#else
		return ((grp=getgrgid(groupid))!=NULL);
	#endif
}
