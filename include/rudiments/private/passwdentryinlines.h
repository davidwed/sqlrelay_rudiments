// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS
	#include <strings.h>
#endif

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

inline char *passwdentry::getName(uid_t userid) {
	passwdentry	pwd;
	return (pwd.initialize(userid))?strdup(pwd.getName()):NULL;
}

inline char *passwdentry::getPassword(uid_t userid) {
	passwdentry	pwd;
	return (pwd.initialize(userid))?strdup(pwd.getPassword()):NULL;
}

inline gid_t passwdentry::getPrimaryGroup(uid_t userid) {
	passwdentry	pwd;
	return (pwd.initialize(userid))?pwd.getPrimaryGroup():-1;
}

inline char *passwdentry::getRealName(uid_t userid) {
	passwdentry	pwd;
	return (pwd.initialize(userid))?strdup(pwd.getRealName()):NULL;
}

inline char *passwdentry::getHomeDirectory(uid_t userid) {
	passwdentry	pwd;
	return (pwd.initialize(userid))?strdup(pwd.getHomeDirectory()):NULL;
}

inline char *passwdentry::getShell(uid_t userid) {
	passwdentry	pwd;
	return (pwd.initialize(userid))?strdup(pwd.getShell()):NULL;
}

inline uid_t passwdentry::getUserId(const char *username) {
	passwdentry	pwd;
	return (pwd.initialize(username))?pwd.getUserId():-1;
}

inline char *passwdentry::getPassword(const char *username) {
	passwdentry	pwd;
	return (pwd.initialize(username))?strdup(pwd.getPassword()):NULL;
}

inline gid_t passwdentry::getPrimaryGroup(const char *username) {
	passwdentry	pwd;
	return (pwd.initialize(username))?pwd.getPrimaryGroup():-1;
}

inline char *passwdentry::getRealName(const char *username) {
	passwdentry	pwd;
	return (pwd.initialize(username))?strdup(pwd.getRealName()):NULL;
}

inline char *passwdentry::getHomeDirectory(const char *username) {
	passwdentry	pwd;
	return (pwd.initialize(username))?strdup(pwd.getHomeDirectory()):NULL;
}

inline char *passwdentry::getShell(const char *username) {
	passwdentry	pwd;
	return (pwd.initialize(username))?strdup(pwd.getShell()):NULL;
}
