// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS
	#include <strings.h>
#endif

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

RUDIMENTS_INLINE bool passwdentry::getName(uid_t userid, char **name) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*name=strdup(pwd.getName());
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool passwdentry::getPassword(uid_t userid, char **password) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*password=strdup(pwd.getPassword());
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool passwdentry::getPrimaryGroupId(uid_t userid,
							gid_t *groupid) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*groupid=pwd.getPrimaryGroupId();
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool passwdentry::getRealName(uid_t userid,
							char **realname) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*realname=strdup(pwd.getRealName());
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool passwdentry::getHomeDirectory(uid_t userid,
							char **homedir) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*homedir=strdup(pwd.getHomeDirectory());
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool passwdentry::getShell(uid_t userid, char **shell) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*shell=strdup(pwd.getShell());
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool passwdentry::getUserId(const char *username,
							uid_t *userid) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*userid=pwd.getUserId();
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool passwdentry::getPassword(const char *username,
							char **password) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*password=strdup(pwd.getPassword());
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool passwdentry::getPrimaryGroupId(const char *username,
							gid_t *groupid) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*groupid=pwd.getPrimaryGroupId();
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool passwdentry::getRealName(const char *username,
							char **realname) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*realname=strdup(pwd.getRealName());
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool passwdentry::getHomeDirectory(const char *username,
							char **homedir) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*homedir=strdup(pwd.getHomeDirectory());
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool passwdentry::getShell(const char *username,
							char **shell) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*shell=strdup(pwd.getShell());
		return true;
	}
	return false;
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
