// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/groupentry.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#include <errno.h>

#define MAXBUFFER (32*1024)

#if defined(__GNUC__) && \
	(!defined(HAVE_GETGRNAM_R) || !defined(HAVE_GETGRUID_R))
pthread_mutex_t	*groupentry::gemutex;
#endif

groupentry::groupentry() {
	grp=NULL;
	buffer=NULL;
	#if !defined(HAVE_GETGRNAM_R) || !defined(HAVE_GETGRUID_R)
		gemutex=NULL;
	#endif
}

groupentry::~groupentry() {
	delete[] buffer;
}

char *groupentry::getName() const {
	return grp->gr_name;
}

char *groupentry::getPassword() const {
	return grp->gr_passwd;
}

gid_t groupentry::getGroupId() const {
	return grp->gr_gid;
}

char **groupentry::getMembers() const {
	return grp->gr_mem;
}

bool groupentry::needsMutex() {
	#if !defined(HAVE_GETGRNAM_R) || !defined(HAVE_GETGRUID_R)
		return true;
	#else
		return false;
	#endif
}

void groupentry::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_GETGRNAM_R) || !defined(HAVE_GETGRUID_R)
		gemutex=mutex;
	#endif
}

bool groupentry::initialize(const char *groupname) {
	return initialize(groupname,0);
}

bool groupentry::initialize(gid_t groupid) {
	return initialize(NULL,groupid);
}

bool groupentry::initialize(const char *groupname, gid_t groupid) {

	if (grp) {
		grp=NULL;
		delete[] buffer;
		buffer=NULL;
	}
	#if defined(HAVE_GETGRNAM_R) && defined(HAVE_GETGRGID_R)
		// getgrnam_r and getgrgid_t are goofy.
		// They will retrieve an arbitrarily large amount of data, but
		// require that you pass them a pre-allocated buffer.  If the
		// buffer is too small, they return an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			if (!((groupname)
				?(getgrnam_r(groupname,&grpbuffer,
							buffer,size,&grp))
				:(getgrgid_r(groupid,&grpbuffer,
							buffer,size,&grp)))) {
				return (grp!=NULL);
			}
			delete[] buffer;
			buffer=NULL;
			grp=NULL;
			if (errno!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		return (((gemutex)?!pthread_mutex_lock(gemutex):true) &&
			((grp=((groupname)
				?getgrnam(groupname)
				:getgrgid(groupid)))!=NULL) &&
			((gemutex)?!pthread_mutex_unlock(gemutex):true));
	#endif
}

bool groupentry::getPassword(const char *groupname, char **password) {
	groupentry	grp;
	if (grp.initialize(groupname)) {
		*password=strdup(grp.getPassword());
		return true;
	}
	return false;
}

bool groupentry::getGroupId(const char *groupname, gid_t *groupid) {
	groupentry	grp;
	if (grp.initialize(groupname)) {
		*groupid=grp.getGroupId();
		return true;
	}
	return false;
}

bool groupentry::getMembers(const char *groupname, char ***members) {
	groupentry	grp;
	if (grp.initialize(groupname)) {
		int	counter;
		for (counter=0; grp.getMembers()[counter]; counter++);
		char	**memb=new char *[counter+1];
		memb[counter]=NULL;
		for (int i=0; i<counter; i++) {
			memb[i]=strdup(grp.getMembers()[i]);
		}
		*members=memb;
		return true;
	}
	return false;
}

bool groupentry::getName(gid_t groupid, char **name) {
	groupentry	grp;
	if (grp.initialize(groupid)) {
		*name=strdup(grp.getName());
		return true;
	}
	return false;
}

bool groupentry::getPassword(gid_t groupid, char **password) {
	groupentry	grp;
	if (grp.initialize(groupid)) {
		*password=strdup(grp.getPassword());
		return true;
	}
	return false;
}

bool groupentry::getMembers(gid_t groupid, char ***members) {
	groupentry	grp;
	if (grp.initialize(groupid)) {
		int	counter;
		for (counter=0; grp.getMembers()[counter]; counter++);
		char	**memb=new char *[counter+1];
		memb[counter]=NULL;
		for (int i=0; i<counter; i++) {
			memb[i]=strdup(grp.getMembers()[i]);
		}
		*members=memb;
		return true;
	}
	return false;
}

void groupentry::print() const {

	if (!grp) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Password: %s\n",getPassword());
	printf("Group Id: %d\n",getGroupId());
	printf("Members:\n");
	for (int i=0; getMembers()[i]; i++) {
		printf("	%s\n",getMembers()[i]);
	}
}
