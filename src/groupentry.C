// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/groupentry.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/groupentryinlines.h>
#endif

#include <stdio.h>
#include <errno.h>

#define MAXBUFFER (32*1024)

int groupentry::initialize(const char *groupname) {
	if (grp) {
		grp=NULL;
		delete[] buffer;
		buffer=NULL;
	}
	#ifdef HAVE_GETGRNAM_R
		// getgrnam_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			if (!getgrnam_r(groupname,&grpbuffer,
						buffer,size,&grp)) {
				return grp!=NULL;
			}
			delete[] buffer;
			buffer=NULL;
			grp=NULL;
			if (errno!=ENOMEM) {
				return 0;
			}
		}
		return 0;
	#else
		return ((grp=getgrnam(groupname))!=NULL);
	#endif
}

int groupentry::initialize(gid_t groupid) {
	if (grp) {
		grp=NULL;
		delete[] buffer;
		buffer=NULL;
	}
	#ifdef HAVE_GETPWUID_R
		// getgrgid_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			if (!getgrgid_r(groupid,&grpbuffer,buffer,size,&grp)) {
				return grp!=NULL;
			}
			delete[] buffer;
			buffer=NULL;
			grp=NULL;
			if (errno!=ENOMEM) {
				return 0;
			}
		}
		return 0;
	#else
		return ((grp=getgrgid(groupid))!=NULL);
	#endif
}

void groupentry::print() const {

	if (!grp) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Password: %s\n",getPassword());
	printf("Group Id: %d\n",getGroupId());
	printf("Members:\n",getGroupId());
	for (int i=0; getMembers()[i]; i++) {
		printf("	%s\n",getMembers()[i]);
	}
}
