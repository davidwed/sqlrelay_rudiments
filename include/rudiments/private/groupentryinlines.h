// Copyright (c) 2002 David Muse
// See the COPYING file for more information

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
