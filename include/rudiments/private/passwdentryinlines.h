// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS
	#include <strings.h>
#endif

INLINE passwdentry::passwdentry() {
	pwd=NULL;
	buffer=NULL;
}

INLINE passwdentry::~passwdentry() {
	delete[] buffer;
}

INLINE char *passwdentry::getName() const {
	return pwd->pw_name;
}

INLINE char *passwdentry::getPassword() const {
	return pwd->pw_passwd;
}

INLINE uid_t passwdentry::getUserId() const {
	return pwd->pw_uid;
}

INLINE gid_t passwdentry::getPrimaryGroup() const {
	return pwd->pw_gid;
}

INLINE char *passwdentry::getRealName() const {
	return pwd->pw_gecos;
}

INLINE char *passwdentry::getHomeDirectory() const {
	return pwd->pw_dir;
}

INLINE char *passwdentry::getShell() const {
	return pwd->pw_shell;
}

INLINE int passwdentry::getName(uid_t userid, char **name) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*name=strdup(pwd.getName());
		return 1;
	}
	return 0;
}

INLINE int passwdentry::getPassword(uid_t userid, char **password) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*password=strdup(pwd.getPassword());
		return 1;
	}
	return 0;
}

INLINE int passwdentry::getPrimaryGroup(uid_t userid, gid_t *groupid) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*groupid=pwd.getPrimaryGroup();
		return 1;
	}
	return 0;
}

INLINE int passwdentry::getRealName(uid_t userid, char **realname) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*realname=strdup(pwd.getRealName());
		return 1;
	}
	return 0;
}

INLINE int passwdentry::getHomeDirectory(uid_t userid, char **homedir) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*homedir=strdup(pwd.getHomeDirectory());
		return 1;
	}
	return 0;
}

INLINE int passwdentry::getShell(uid_t userid, char **shell) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*shell=strdup(pwd.getShell());
		return 1;
	}
	return 0;
}

INLINE int passwdentry::getUserId(const char *username, uid_t *userid) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*userid=pwd.getUserId();
		return 1;
	}
	return 0;
}

INLINE int passwdentry::getPassword(const char *username, char **password) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*password=strdup(pwd.getPassword());
		return 1;
	}
	return 0;
}

INLINE int passwdentry::getPrimaryGroup(const char *username, gid_t *groupid) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*groupid=pwd.getPrimaryGroup();
		return 1;
	}
	return 0;
}

INLINE int passwdentry::getRealName(const char *username, char **realname) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*realname=strdup(pwd.getRealName());
		return 1;
	}
	return 0;
}

INLINE int passwdentry::getHomeDirectory(const char *username, char **homedir) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*homedir=strdup(pwd.getHomeDirectory());
		return 1;
	}
	return 0;
}

INLINE int passwdentry::getShell(const char *username, char **shell) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*shell=strdup(pwd.getShell());
		return 1;
	}
	return 0;
}
