// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/passwdentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

#include <stdio.h>
#include <stdlib.h>

#define MAXBUFFER	(32*1024)

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class passwdentryprivate {
	friend class passwdentry;
	private:
		#ifndef MINGW32
			passwd	*_pwd;
		#if defined(RUDIMENTS_HAVE_GETPWNAM_R) && \
			defined(RUDIMENTS_HAVE_GETPWUID_R)
			passwd	_pwdbuffer;
			char	*_buffer;
		#endif
		#else
			LPUSER_INFO_23	_buffer;
			char		*_name;
			char		*_fullname;
		#endif
};

// LAME: not in the class
#if (!defined(RUDIMENTS_HAVE_GETPWNAM_R) || !defined(RUDIMENTS_HAVE_GETPWUID_R))
static mutex	*pemutex;
#endif


passwdentry::passwdentry() {
	pvt=new passwdentryprivate;
#ifndef MINGW32
	pvt->_pwd=NULL;
	#if defined(RUDIMENTS_HAVE_GETPWNAM_R) && \
		defined(RUDIMENTS_HAVE_GETPWUID_R)
		rawbuffer::zero(&pvt->_pwdbuffer,sizeof(pvt->_pwdbuffer));
		pvt->_buffer=NULL;
	#endif
#else
	pvr->_buffer=NULL;
	pvr->_name=NULL;
	pvr->_fullname=NULL;
#endif
}

passwdentry::passwdentry(const passwdentry &p) {
	pvt=new passwdentryprivate;
	initialize(p.getName());
}

passwdentry &passwdentry::operator=(const passwdentry &p) {
	if (this!=&p) {
		initialize(p.getName());
	}
	return *this;
}

passwdentry::~passwdentry() {
#ifndef MINGW32
	#if defined(RUDIMENTS_HAVE_GETPWNAM_R) && \
		defined(RUDIMENTS_HAVE_GETPWUID_R)
		delete[] pvt->_buffer;
	#endif
	delete pvt;
#else
	if (pvt->_buffer) {
		NetApiBufferFree(pvt->_buffer);
	}
	delete[] pvt->_name;
	delete[] pvt->_fullname;
#endif
}

const char *passwdentry::getName() const {
#ifndef MINGW32
	return pvt->_pwd->pw_name;
#else
	return pvt->_name;
#endif
}

const char *passwdentry::getPassword() const {
#ifndef MINGW32
	return pvt->_pwd->pw_passwd;
#else
	// FIXME:
	return "";
#endif
}

uid_t passwdentry::getUserId() const {
#ifndef MINGW32
	return pvt->_pwd->pw_uid;
#else
	// FIXME:
	return 0;
#endif
}

gid_t passwdentry::getPrimaryGroupId() const {
#ifndef MINGW32
	return pvt->_pwd->pw_gid;
#else
	// FIXME:
	return 0;
#endif
}

const char *passwdentry::getRealName() const {
#ifndef MINGW32
	return pvt->_pwd->pw_gecos;
#else
	return pvt->_fullname;
#endif
}

const char *passwdentry::getHomeDirectory() const {
#ifndef MINGW32
	return pvt->_pwd->pw_dir;
#else
	// FIXME:
	return "";
#endif
}

const char *passwdentry::getShell() const {
#ifndef MINGW32
	return pvt->_pwd->pw_shell;
#else
	// FIXME:
	return "";
#endif
}

bool passwdentry::needsMutex() {
	#if !defined(RUDIMENTS_HAVE_GETPWNAM_R) || \
		!defined(RUDIMENTS_HAVE_GETPWUID_R)
		return true;
	#else
		return false;
	#endif
}

void passwdentry::setMutex(mutex *mtx) {
	#if !defined(RUDIMENTS_HAVE_GETPWNAM_R) || \
		!defined(RUDIMENTS_HAVE_GETPWUID_R)
		pemutex=mtx;
	#endif
}

bool passwdentry::initialize(const char *username) {
	return initialize(username,0);
}

bool passwdentry::initialize(uid_t userid) {
	return initialize(NULL,userid);
}

#ifndef MINGW32
bool passwdentry::initialize(const char *username, uid_t userid) {

	#if defined(RUDIMENTS_HAVE_GETPWNAM_R) && \
		defined(RUDIMENTS_HAVE_GETPWUID_R)
		if (pvt->_pwd) {
			pvt->_pwd=NULL;
			delete[] pvt->_buffer;
			pvt->_buffer=NULL;
		}
		// getpwnam_r and getpwuid_r are goofy.
		// They will retrieve an arbitrarily large amount of data, but
		// require that you pass them a pre-allocated buffer.  If the
		// buffer is too small, they returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			pvt->_buffer=new char[size];
			#if defined(RUDIMENTS_HAVE_GETPWNAM_R_5) && \
				defined(RUDIMENTS_HAVE_GETPWUID_R_5)
			if (!((username)
				?(getpwnam_r(username,
						&pvt->_pwdbuffer,
						pvt->_buffer,size,
						&pvt->_pwd))
				:(getpwuid_r(userid,
						&pvt->_pwdbuffer,
						pvt->_buffer,size,
						&pvt->_pwd)))) {
				return (pvt->_pwd!=NULL);
			}
			#elif defined(RUDIMENTS_HAVE_GETPWNAM_R_4) && \
				defined(RUDIMENTS_HAVE_GETPWUID_R_4)
			if ((username)
				?(pvt->_pwd=getpwnam_r(username,
							&pvt->_pwdbuffer,
							pvt->_buffer,size))
				:(pvt->_pwd=getpwuid_r(userid,
							&pvt->_pwdbuffer,
							pvt->_buffer,size))) {
				return true;
			}
			#endif
			delete[] pvt->_buffer;
			pvt->_buffer=NULL;
			pvt->_pwd=NULL;
			if (error::getErrorNumber()!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		pvt->_pwd=NULL;
		return (!(pemutex && !pemutex->lock()) &&
			((pvt->_pwd=((username)
				?getpwnam(username)
				:getpwuid(userid)))!=NULL) &&
			!(pemutex && !pemutex->unlock()));
	#endif
}
#else
bool passwdentry::initialize(const char *username, uid_t userid) {

	if (pvt->_buffer) {
		NetApiBufferFree(pvt->_buffer);
	}
	delete[] pvt->_name;
	pvt->_name=NULL;
	delete[] pvt->_fullname;
	pvt->_fullname=NULL;

	if (username) {	
		if (NetUserGetInfo(NULL,username,23,
				reinterpret_cast<LPBYTE *>(&pvt->_buffer))!=
				NERR_SUCCESS) {
			return false;
		}
		pvt->_name=new char[charstring::length(
					pvt->_buffer->usri23_name)]+1;
		pvt->_fullname=new char[charstring::length(
					pvt->_buffer->usri23_full_name)]+1;
		wsprintf(pvt->_name,"%s",pvt->_buffer->usri23_name);
		wsprintf(pvt->_fullname,"%s",pvt->_buffer->usri23_fullname);
	} else {
#error implement me...
	}
	return true;
}
#endif

bool passwdentry::getName(uid_t userid, char **name) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*name=charstring::duplicate(pwd.getName());
		return true;
	}
	return false;
}

bool passwdentry::getPassword(uid_t userid, char **password) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*password=charstring::duplicate(pwd.getPassword());
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
		*realname=charstring::duplicate(pwd.getRealName());
		return true;
	}
	return false;
}

bool passwdentry::getHomeDirectory(uid_t userid, char **homedir) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*homedir=charstring::duplicate(pwd.getHomeDirectory());
		return true;
	}
	return false;
}

bool passwdentry::getShell(uid_t userid, char **shell) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		*shell=charstring::duplicate(pwd.getShell());
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
		*password=charstring::duplicate(pwd.getPassword());
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
		*realname=charstring::duplicate(pwd.getRealName());
		return true;
	}
	return false;
}

bool passwdentry::getHomeDirectory(const char *username, char **homedir) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*homedir=charstring::duplicate(pwd.getHomeDirectory());
		return true;
	}
	return false;
}

bool passwdentry::getShell(const char *username, char **shell) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		*shell=charstring::duplicate(pwd.getShell());
		return true;
	}
	return false;
}

void passwdentry::print() const {

	if (!pvt->_pwd) {
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

#ifdef RUDIMENTS_NAMESPACE
}
#endif
