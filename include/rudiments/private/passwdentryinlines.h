// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE passwdentry::passwdentry() {
	pwd=NULL;
	buffer=NULL;
	#if !defined(HAVE_GETPWNAM_R) || !defined(HAVE_GETPWUID_R)
		pemutex=NULL;
	#endif
}

RUDIMENTS_INLINE passwdentry::~passwdentry() {
	delete[] buffer;
}

RUDIMENTS_INLINE char *passwdentry::getName() const {
	return pwd->pw_name;
}

RUDIMENTS_INLINE char *passwdentry::getPassword() const {
	return pwd->pw_passwd;
}

RUDIMENTS_INLINE uid_t passwdentry::getUserId() const {
	return pwd->pw_uid;
}

RUDIMENTS_INLINE gid_t passwdentry::getPrimaryGroupId() const {
	return pwd->pw_gid;
}

RUDIMENTS_INLINE char *passwdentry::getRealName() const {
	return pwd->pw_gecos;
}

RUDIMENTS_INLINE char *passwdentry::getHomeDirectory() const {
	return pwd->pw_dir;
}

RUDIMENTS_INLINE char *passwdentry::getShell() const {
	return pwd->pw_shell;
}

RUDIMENTS_INLINE bool passwdentry::needsMutex() {
	#if !defined(HAVE_GETPWNAM_R) || !defined(HAVE_GETPWUID_R)
		return true;
	#else
		return false;
	#endif
}

RUDIMENTS_INLINE void passwdentry::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_GETPWNAM_R) || !defined(HAVE_GETPWUID_R)
		pemutex=mutex;
	#endif
}

RUDIMENTS_INLINE bool passwdentry::initialize(const char *username) {
	return initialize(username,0);
}

RUDIMENTS_INLINE bool passwdentry::initialize(uid_t userid) {
	return initialize(NULL,userid);
}
