// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS
	#include <strings.h>
#endif

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE groupentry::groupentry() {
	grp=NULL;
	buffer=NULL;
	#if !defined(HAVE_GETGRNAM_R) || !defined(HAVE_GETGRUID_R)
		gemutex=NULL;
	#endif
}

RUDIMENTS_INLINE groupentry::~groupentry() {
	delete[] buffer;
}

RUDIMENTS_INLINE char *groupentry::getName() const {
	return grp->gr_name;
}

RUDIMENTS_INLINE char *groupentry::getPassword() const {
	return grp->gr_passwd;
}

RUDIMENTS_INLINE gid_t groupentry::getGroupId() const {
	return grp->gr_gid;
}

RUDIMENTS_INLINE char **groupentry::getMembers() const {
	return grp->gr_mem;
}

RUDIMENTS_INLINE bool groupentry::getPassword(const char *groupname,
							char **password) {
	groupentry	grp;
	if (grp.initialize(groupname)) {
		*password=strdup(grp.getPassword());
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool groupentry::getGroupId(const char *groupname,
							gid_t *groupid) {
	groupentry	grp;
	if (grp.initialize(groupname)) {
		*groupid=grp.getGroupId();
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool groupentry::getMembers(const char *groupname,
							char ***members) {
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

RUDIMENTS_INLINE bool groupentry::getName(gid_t groupid, char **name) {
	groupentry	grp;
	if (grp.initialize(groupid)) {
		*name=strdup(grp.getName());
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool groupentry::getPassword(gid_t groupid, char **password) {
	groupentry	grp;
	if (grp.initialize(groupid)) {
		*password=strdup(grp.getPassword());
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool groupentry::getMembers(gid_t groupid, char ***members) {
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

RUDIMENTS_INLINE bool groupentry::needsMutex() {
	#if !defined(HAVE_GETGRNAM_R) || !defined(HAVE_GETGRUID_R)
		return true;
	#else
		return false;
	#endif
}

RUDIMENTS_INLINE void groupentry::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_GETGRNAM_R) || !defined(HAVE_GETGRUID_R)
		gemutex=mutex;
	#endif
}

RUDIMENTS_INLINE bool groupentry::initialize(const char *groupname) {
	return initialize(groupname,0);
}

RUDIMENTS_INLINE bool groupentry::initialize(gid_t groupid) {
	return initialize(NULL,groupid);
}
