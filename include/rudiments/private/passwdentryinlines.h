// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <stdio.h>

inline passwdentry::passwdentry() {
	pwd=NULL;
	buffer=NULL;
}

inline passwdentry::~passwdentry() {
	delete[] buffer;
}

inline char *passwdentry::getName() const {
	return pwd->pw_name;
}

inline char *passwdentry::getPassword() const {
	return pwd->pw_passwd;
}

inline uid_t passwdentry::getUserId() const {
	return pwd->pw_uid;
}

inline gid_t passwdentry::getPrimaryGroup() const {
	return pwd->pw_gid;
}

inline char *passwdentry::getRealName() const {
	return pwd->pw_gecos;
}

inline char *passwdentry::getHomeDirectory() const {
	return pwd->pw_dir;
}

inline char *passwdentry::getShell() const {
	return pwd->pw_shell;
}
