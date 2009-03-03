// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/passwdentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_HAVE_NETUSERGETINFO
	// for USER_INFO_2, functions
	#include <windows.h>
	#include <lm.h>
#else
	// for passwd, functions
	#include <pwd.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#define MAXBUFFER	(32*1024)

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#ifdef RUDIMENTS_HAVE_NETUSERGETINFO
CHAR *unicodeToAscii(WCHAR *in) {

	BOOL	useddefaultchar;
	int	size=WideCharToMultiByte(CP_ACP,0,in,-1,NULL,0,
						"?",&useddefaultchar);
	if (size==0) {
		return NULL;
	}

	CHAR	*out=new char[size];
	if (!WideCharToMultiByte(CP_ACP,0,in,-1,out,size,
						"?",&useddefaultchar)) {
		delete[] out;
		out=NULL;
	}
	return out;
}
#endif

class passwdentryprivate {
	friend class passwdentry;
	private:
		#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
			passwd	*_pwd;
			#if defined(RUDIMENTS_HAVE_GETPWNAM_R) && \
				defined(RUDIMENTS_HAVE_GETPWUID_R)
				passwd	_pwdbuffer;
				char	*_buffer;
			#endif
		#else
			USER_INFO_2	*_buffer;
			char		*_name;
			char		*_password;
			char		*_realname;
			char		*_homedir;
		#endif
};

// LAME: not in the class
#if (!defined(RUDIMENTS_HAVE_GETPWNAM_R) || !defined(RUDIMENTS_HAVE_GETPWUID_R))
static mutex	*pemutex;
#endif


passwdentry::passwdentry() {
	pvt=new passwdentryprivate;
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	pvt->_pwd=NULL;
	#if defined(RUDIMENTS_HAVE_GETPWNAM_R) && \
		defined(RUDIMENTS_HAVE_GETPWUID_R)
		rawbuffer::zero(&pvt->_pwdbuffer,sizeof(pvt->_pwdbuffer));
		pvt->_buffer=NULL;
	#endif
#else
	pvt->_buffer=NULL;
	pvt->_name=NULL;
	pvt->_password=NULL;
	pvt->_realname=NULL;
	pvt->_homedir=NULL;
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
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
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
	delete[] pvt->_password;
	delete[] pvt->_realname;
	delete[] pvt->_homedir;
#endif
}

const char *passwdentry::getName() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return pvt->_pwd->pw_name;
#else
	return pvt->_name;
#endif
}

const char *passwdentry::getPassword() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return pvt->_pwd->pw_passwd;
#else
	return pvt->_password;
#endif
}

uid_t passwdentry::getUserId() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return pvt->_pwd->pw_uid;
#else
	// Windows user id's aren't numbers, they're undefined, odd, variable
	// length structures.  There's probably soem way to get one, but
	// returning it is another matter.  You'd need to figure out it's
	// length, bytewise copy it out and return it's length too.  It doesn't
	// fit well into this paradigm so for now at least we'll call this
	// unsupported.
	return -1;
#endif
}

gid_t passwdentry::getPrimaryGroupId() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return pvt->_pwd->pw_gid;
#else
	// Windows group id's aren't numbers, they're undefined, odd, variable
	// length structures.  There's probably soem way to get one, but
	// returning it is another matter.  You'd need to figure out it's
	// length, bytewise copy it out and return it's length too.  It doesn't
	// fit well into this paradigm so for now at least we'll call this
	// unsupported.
	return -1;
#endif
}

const char *passwdentry::getRealName() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return pvt->_pwd->pw_gecos;
#else
	return pvt->_realname;
#endif
}

const char *passwdentry::getHomeDirectory() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return pvt->_pwd->pw_dir;
#else
	return pvt->_homedir;
#endif
}

const char *passwdentry::getShell() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return pvt->_pwd->pw_shell;
#else
	// Under windows, users don't have default shells.  As far as I know,
	// the command line is always the same.  You can run other shells
	// but they're not tied to a user.
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

bool passwdentry::initialize(const char *username, uid_t userid) {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO

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

#else

	if (pvt->_buffer) {
		NetApiBufferFree(pvt->_buffer);
	}
	delete[] pvt->_name;
	pvt->_name=NULL;
	delete[] pvt->_realname;
	pvt->_realname=NULL;
	delete[] pvt->_password;
	pvt->_password=NULL;
	delete[] pvt->_realname;
	pvt->_realname=NULL;
	delete[] pvt->_homedir;
	pvt->_homedir=NULL;

	if (username) {	

		// convert username to unicode...

		// get the size of the unicode buffer
		int	usernamewsize=MultiByteToWideChar(CP_ACP,
							MB_PRECOMPOSED,
							username,
							-1,NULL,0);
		if (!usernamewsize) {
			return false;
		}

		// create the buffer
		WCHAR	*usernamew=new WCHAR[usernamewsize];

		// perform the actual conversion
		usernamewsize=MultiByteToWideChar(CP_ACP,
							MB_PRECOMPOSED,
							username,-1,
							usernamew,
							usernamewsize);
		if (!usernamewsize) {
			delete[] usernamew;
			return false;
		}


		if (NetUserGetInfo(NULL,usernamew,1,
				reinterpret_cast<BYTE **>(&pvt->_buffer))!=
				NERR_Success) {
			delete[] usernamew;
			return false;
		}

		// convert the unicode values to ascii
		pvt->_name=unicodeToAscii(pvt->_buffer->usri2_name);
		pvt->_password=unicodeToAscii(pvt->_buffer->usri2_password);
		pvt->_realname=unicodeToAscii(pvt->_buffer->usri2_full_name);
		pvt->_homedir=unicodeToAscii(pvt->_buffer->usri2_home_dir);
	} else {
		// windows doesn't appear to support this
		error::setErrorNumber(ENOSYS);
		return false;
	}
	return true;
#endif
}

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

#ifdef RUDIMENTS_HAVE_NETUSERGETINFO
	if (!pvt->_buffer) {
		return;
	}
#else
	if (!pvt->_pwd) {
		return;
	}
#endif

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
