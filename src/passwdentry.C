// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/passwdentry.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#include <errno.h>

#define MAXBUFFER	(32*1024)

#if defined(__GNUC__) && \
	(!defined(HAVE_GETPWNAM_R) || !defined(HAVE_GETPWUID_R))
pthread_mutex_t	*passwdentry::pemutex;
#endif


passwdentry::passwdentry() {
	pwd=NULL;
	buffer=NULL;
	#if !defined(HAVE_GETPWNAM_R) || !defined(HAVE_GETPWUID_R)
		pemutex=NULL;
	#endif
}

passwdentry::~passwdentry() {
	delete[] buffer;
}

char *passwdentry::getName() const {
	return pwd->pw_name;
}

char *passwdentry::getPassword() const {
	return pwd->pw_passwd;
}

uid_t passwdentry::getUserId() const {
	return pwd->pw_uid;
}

gid_t passwdentry::getPrimaryGroupId() const {
	return pwd->pw_gid;
}

char *passwdentry::getRealName() const {
	return pwd->pw_gecos;
}

char *passwdentry::getHomeDirectory() const {
	return pwd->pw_dir;
}

char *passwdentry::getShell() const {
	return pwd->pw_shell;
}

bool passwdentry::needsMutex() {
	#if !defined(HAVE_GETPWNAM_R) || !defined(HAVE_GETPWUID_R)
		return true;
	#else
		return false;
	#endif
}

void passwdentry::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_GETPWNAM_R) || !defined(HAVE_GETPWUID_R)
		pemutex=mutex;
	#endif
}

bool passwdentry::initialize(const char *username) {
	return initialize(username,0);
}

bool passwdentry::initialize(uid_t userid) {
	return initialize(NULL,userid);
}

bool passwdentry::initialize(const char *username, uid_t userid) {
	if (pwd) {
		pwd=NULL;
		delete[] buffer;
		buffer=NULL;
	}
	#if defined(HAVE_GETPWNAM_R) && defined(HAVE_GETPWUID_R)
		// getpwnam_r and getpwuid_r are goofy.
		// They will retrieve an arbitrarily large amount of data, but
		// require that you pass them a pre-allocated buffer.  If the
		// buffer is too small, they returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			if (!((username)
				?(getpwnam_r(username,&pwdbuffer,
							buffer,size,&pwd))
				:(getpwuid_r(userid,&pwdbuffer,
							buffer,size,&pwd)))) {
				return (pwd!=NULL);
			}
			delete[] buffer;
			buffer=NULL;
			pwd=NULL;
			if (errno!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		return (((pemutex)?!pthread_mutex_lock(pemutex):true) &&
				((pwd=((username)
					?getpwnam(username)
					:getpwuid(userid)))!=NULL) &&
			((pemutex)?!pthread_mutex_unlock(pemutex):true));
	#endif
}

bool passwdentry::getName(uid_t userid, char **name) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*name=strdup(pwd.getName());
		return true;
	}
	return false;
}

bool passwdentry::getPassword(uid_t userid, char **password) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*password=strdup(pwd.getPassword());
		return true;
	}
	return false;
}

bool passwdentry::getPrimaryGroupId(uid_t userid, gid_t *groupid) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*groupid=pwd.getPrimaryGroupId();
		return true;
	}
	return false;
}

bool passwdentry::getRealName(uid_t userid, char **realname) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*realname=strdup(pwd.getRealName());
		return true;
	}
	return false;
}

bool passwdentry::getHomeDirectory(uid_t userid, char **homedir) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*homedir=strdup(pwd.getHomeDirectory());
		return true;
	}
	return false;
}

bool passwdentry::getShell(uid_t userid, char **shell) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*shell=strdup(pwd.getShell());
		return true;
	}
	return false;
}

bool passwdentry::getUserId(const char *username, uid_t *userid) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*userid=pwd.getUserId();
		return true;
	}
	return false;
}

bool passwdentry::getPassword(const char *username, char **password) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*password=strdup(pwd.getPassword());
		return true;
	}
	return false;
}

bool passwdentry::getPrimaryGroupId(const char *username, gid_t *groupid) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*groupid=pwd.getPrimaryGroupId();
		return true;
	}
	return false;
}

bool passwdentry::getRealName(const char *username, char **realname) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*realname=strdup(pwd.getRealName());
		return true;
	}
	return false;
}

bool passwdentry::getHomeDirectory(const char *username, char **homedir) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*homedir=strdup(pwd.getHomeDirectory());
		return true;
	}
	return false;
}

bool passwdentry::getShell(const char *username, char **shell) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*shell=strdup(pwd.getShell());
		return true;
	}
	return false;
}

void passwdentry::print() const {

	if (!pwd) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Password: %s\n",getPassword());
	printf("User Id: %d\n",getUserId());
	printf("Primary Group Id: %d\n",getPrimaryGroupId());
	printf("Real Name: %s\n",getRealName());
	printf("Home Directory: %s\n",getHomeDirectory());
	printf("Shell: %s\n",getShell());
}
