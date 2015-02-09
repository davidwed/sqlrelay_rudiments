// Copyright (c) 2002-2014 David Muse
// See the COPYING file for more information

#include <rudiments/userentry.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/sys.h>
#include <rudiments/error.h>

#if defined(RUDIMENTS_HAVE_GETSPNAM) || defined(RUDIMENTS_HAVE_GETSPNAM_R)
	#define RUDIMENTS_HAVE_SHADOW
#endif

#ifdef RUDIMENTS_HAVE_NETUSERGETINFO
	#include <rudiments/dictionary.h>
	#include <rudiments/groupentry.h>
#else
	#include <pwd.h>
#endif

#ifdef RUDIMENTS_HAVE_SHADOW
	// for spwd, functions
	#include <shadow.h>
#endif

#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif
#ifdef RUDIMENTS_HAVE_LM_H
	#include <lm.h>
#endif
#ifdef RUDIMENTS_HAVE_SDDL_H
	#include <sddl.h>
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#define MAXBUFFER	(32*1024)

class userentryprivate {
	friend class userentry;
	private:
		#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
			passwd	*_pwd;
			#if defined(RUDIMENTS_HAVE_GETPWNAM_R) && \
				defined(RUDIMENTS_HAVE_GETPWUID_R)
				passwd	_pwdbuffer;
				char	*_pwdcharbuffer;
			#endif
			char	*_sid;
			#ifdef RUDIMENTS_HAVE_SHADOW
				spwd	*_sp;
				#if defined(RUDIMENTS_HAVE_GETSPNAM_R)
					spwd	_spbuffer;
					char	*_spcharbuffer;
				#endif
			#endif
		#else
			char		*_name;
			char		*_password;
			gid_t		_primarygroupid;
			char		*_realname;
			char		*_homedir;
			CHAR		*_sid;
			uid_t		_uid;
		#endif
};

#if (!defined(RUDIMENTS_HAVE_GETPWNAM_R) || \
	!defined(RUDIMENTS_HAVE_GETPWUID_R)) || \
	defined(RUDIMENTS_HAVE_NETUSERGETINFO)
// LAME: not in the class
static threadmutex	*uemutex;
#endif

#ifdef RUDIMENTS_HAVE_NETUSERGETINFO
// FIXME: move to charstring class
static WCHAR *asciiToUnicode(const CHAR *in) {

	int32_t	size=MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,in,-1,NULL,0);
	if (!size) {
		return NULL;
	}

	WCHAR	*out=new WCHAR[size];
	if (!MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,in,-1,out,size)) {
		delete[] out;
		out=NULL;
	}
	return out;
}

// FIXME: move to charstring class
static CHAR *unicodeToAscii(const WCHAR *in) {

	BOOL	useddefaultchar;
	int32_t	size=WideCharToMultiByte(CP_ACP,0,in,-1,NULL,0,NULL,NULL);
	if (!size) {
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

static uid_t	uid=0;
struct namesid {
	char	*name;
	char	*sid;
};
static dictionary< uid_t, namesid * >	uidmap;
// FIXME: clean up with init and exit methods like environment class

static uid_t addUidMapping(const char *name, const char *sid) {

	// check for existing mapping (by name only)
	for (linkedlistnode< dictionarynode< uid_t, namesid * > *>
				*node=uidmap.getList()->getFirst(); 
				node; node=node->getNext()) {
		namesid	*ns=node->getValue()->getValue();
		if (!charstring::compare(name,ns->name)) {
			// reset the sid
			delete[] ns->sid;
			ns->sid=charstring::duplicate(sid);
			return node->getValue()->getKey();
		}
	}

	// create a new entry
	namesid	*ns=new namesid;
	ns->name=charstring::duplicate(name);
	ns->sid=charstring::duplicate(sid);

	if (uemutex) {
		uemutex->lock();
	}
	uid_t	u=uid;
	uid++;
	if (uemutex) {
		uemutex->unlock();
	}

	uidmap.setValue(u,ns);
	return u;
}
#endif


userentry::userentry() {
	pvt=new userentryprivate;
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	pvt->_pwd=NULL;
	#if defined(RUDIMENTS_HAVE_GETPWNAM_R) && \
		defined(RUDIMENTS_HAVE_GETPWUID_R)
		bytestring::zero(&pvt->_pwdbuffer,sizeof(pvt->_pwdbuffer));
		pvt->_pwdcharbuffer=NULL;
	#endif
	#ifdef RUDIMENTS_HAVE_SHADOW
		pvt->_sp=NULL;
		#ifdef RUDIMENTS_HAVE_GETSPNAM_R
			bytestring::zero(&pvt->_spbuffer,
						sizeof(pvt->_spbuffer));
			pvt->_spcharbuffer=NULL;
		#endif
	#endif
#else
	pvt->_name=NULL;
	pvt->_password=NULL;
	pvt->_primarygroupid=-1;
	pvt->_realname=NULL;
	pvt->_homedir=NULL;
	pvt->_uid=(uid_t)-1;
#endif
	pvt->_sid=NULL;
}

userentry::userentry(const userentry &p) {
	pvt=new userentryprivate;
	initialize(p.getName());
}

userentry &userentry::operator=(const userentry &p) {
	if (this!=&p) {
		initialize(p.getName());
	}
	return *this;
}

userentry::~userentry() {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	#if defined(RUDIMENTS_HAVE_GETPWNAM_R) && \
		defined(RUDIMENTS_HAVE_GETPWUID_R)
		delete[] pvt->_pwdcharbuffer;
	#endif
	#ifdef RUDIMENTS_HAVE_SHADOW
		#ifdef RUDIMENTS_HAVE_GETSPNAM_R
			delete[] pvt->_spcharbuffer;
		#endif
	#endif
	delete[] pvt->_sid;
#else
	delete[] pvt->_name;
	delete[] pvt->_password;
	delete[] pvt->_realname;
	delete[] pvt->_homedir;
	LocalFree(pvt->_sid);
#endif
	delete pvt;
}

const char *userentry::getName() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return (pvt->_pwd)?pvt->_pwd->pw_name:NULL;
#else
	return pvt->_name;
#endif
}

const char *userentry::getPassword() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return (pvt->_pwd)?pvt->_pwd->pw_passwd:NULL;
#else
	return pvt->_password;
#endif
}

const char *userentry::getEncryptedPassword() const {
	#ifdef RUDIMENTS_HAVE_SHADOW
		return (pvt->_sp)?pvt->_sp->sp_pwdp:NULL;
	#else
		return NULL;
	#endif
}

long userentry::getLastChangeDate() const {
	#ifdef RUDIMENTS_HAVE_SHADOW
		return (pvt->_sp)?pvt->_sp->sp_lstchg:-1;
	#else
		return -1;
	#endif
}

int32_t userentry::getDaysBeforeChangeAllowed() const {
	#ifdef RUDIMENTS_HAVE_SHADOW
		return (pvt->_sp)?pvt->_sp->sp_min:-1;
	#else
		return -1;
	#endif
}

int32_t userentry::getDaysBeforeChangeRequired() const {
	#ifdef RUDIMENTS_HAVE_SHADOW
		return (pvt->_sp)?pvt->_sp->sp_max:-1;
	#else
		return -1;
	#endif
}

int32_t userentry::getDaysBeforeExpirationWarning() const {
	#if defined(RUDIMENTS_HAVE_SHADOW) && defined(RUDIMENTS_HAVE_SP_WARN)
		return (pvt->_sp)?pvt->_sp->sp_warn:-1;
	#else
		return -1;
	#endif
}

int32_t userentry::getDaysOfInactivityAllowed() const {
	#if defined(RUDIMENTS_HAVE_SHADOW) && defined(RUDIMENTS_HAVE_SP_INACT)
		return (pvt->_sp)?pvt->_sp->sp_inact:-1;
	#else
		return -1;
	#endif
}

int32_t userentry::getExpirationDate() const {
	#if defined(RUDIMENTS_HAVE_SHADOW) && defined(RUDIMENTS_HAVE_SP_EXPIRE)
		return (pvt->_sp)?pvt->_sp->sp_expire:-1;
	#else
		return -1;
	#endif
}

int32_t userentry::getFlag() const {
	#if defined(RUDIMENTS_HAVE_SHADOW) && defined(RUDIMENTS_HAVE_SP_FLAG)
		return (pvt->_sp)?pvt->_sp->sp_flag:-1;
	#else
		return -1;
	#endif
}

uid_t userentry::getUserId() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return (pvt->_pwd)?pvt->_pwd->pw_uid:(uid_t)-1;
#else
	return pvt->_uid;
#endif
}

const char *userentry::getSid() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	if (!pvt->_sid) {
		if (pvt->_pwd) {
			pvt->_sid=charstring::parseNumber(
						(int64_t)pvt->_pwd->pw_uid);
		} else {
			pvt->_sid=charstring::duplicate("-1");
		}
	}
	return pvt->_sid;
#else
	return pvt->_sid;
#endif
}

gid_t userentry::getPrimaryGroupId() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return (pvt->_pwd)?pvt->_pwd->pw_gid:(gid_t)-1;
#else
	return (gid_t)pvt->_primarygroupid;
#endif
}

const char *userentry::getRealName() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return (pvt->_pwd)?pvt->_pwd->pw_gecos:NULL;
#else
	return pvt->_realname;
#endif
}

const char *userentry::getHomeDirectory() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return (pvt->_pwd)?pvt->_pwd->pw_dir:NULL;
#else
	return pvt->_homedir;
#endif
}

const char *userentry::getShell() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return (pvt->_pwd)?pvt->_pwd->pw_shell:NULL;
#else
	// Under windows, users don't have default shells.  As far as I know,
	// the command line is always the same.  You can run other shells
	// but they're not tied to a user.
	return NULL;
#endif
}

bool userentry::platformSupportsFormalSid() {
	#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
		return false;
	#else
		return true;
	#endif
}

bool userentry::needsMutex() {
	#if (!defined(RUDIMENTS_HAVE_GETPWNAM_R) || \
		!defined(RUDIMENTS_HAVE_GETPWUID_R)) || \
		defined(RUDIMENTS_HAVE_NETUSERGETINFO)
		return true;
	#else
		return false;
	#endif
}

void userentry::setMutex(threadmutex *mtx) {
	#if (!defined(RUDIMENTS_HAVE_GETPWNAM_R) || \
		!defined(RUDIMENTS_HAVE_GETPWUID_R)) || \
		defined(RUDIMENTS_HAVE_NETUSERGETINFO)
		uemutex=mtx;
	#endif
}

bool userentry::initialize(const char *username) {
	return initialize(username,0);
}

bool userentry::initialize(uid_t userid) {
	return initialize(NULL,userid);
}

bool userentry::initialize(const char *username, uid_t userid) {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO

	// init return value
	bool	success=false;

	// init buffers
	delete[] pvt->_sid;
	pvt->_sid=NULL;
	#if defined(RUDIMENTS_HAVE_GETPWNAM_R) && \
		defined(RUDIMENTS_HAVE_GETPWUID_R)
		if (pvt->_pwd) {
			pvt->_pwd=NULL;
			delete[] pvt->_pwdcharbuffer;
			pvt->_pwdcharbuffer=NULL;
		}
	#else
		pvt->_pwd=NULL;
	#endif
	#if defined(RUDIMENTS_HAVE_GETSPNAM_R)
		if (pvt->_sp) {
			pvt->_sp=NULL;
			delete[] pvt->_spcharbuffer;
			pvt->_spcharbuffer=NULL;
		}
	#elif defined(RUDIMENTS_HAVE_GETSPNAM) 
		pvt->_sp=NULL;
	#endif

	// get password info
	#if defined(RUDIMENTS_HAVE_GETPWNAM_R) && \
		defined(RUDIMENTS_HAVE_GETPWUID_R)
		// getpwnam_r and getpwuid_r are goofy.
		// They will retrieve an arbitrarily large amount of data, but
		// require that you pass them a pre-allocated buffer.  If the
		// buffer is too small, they returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		int64_t	inc=sys::getSuggestedPasswordEntryBufferSize();
		int64_t	max=inc*32;
		for (int64_t size=inc; size<max; size=size+inc) {

			pvt->_pwdcharbuffer=new char[size];
			#if defined(RUDIMENTS_HAVE_GETPWNAM_R_5) && \
				defined(RUDIMENTS_HAVE_GETPWUID_R_5)
			if (!((username)
				?(getpwnam_r(username,
						&pvt->_pwdbuffer,
						pvt->_pwdcharbuffer,size,
						&pvt->_pwd))
				:(getpwuid_r(userid,
						&pvt->_pwdbuffer,
						pvt->_pwdcharbuffer,size,
						&pvt->_pwd)))) {
				success=(pvt->_pwd!=NULL);
				break;
			}
			#elif defined(RUDIMENTS_HAVE_GETPWNAM_R_4) && \
				defined(RUDIMENTS_HAVE_GETPWUID_R_4)
			if ((username)
				?(pvt->_pwd=getpwnam_r(username,
							&pvt->_pwdbuffer,
							pvt->_pwdcharbuffer,
							size))
				:(pvt->_pwd=getpwuid_r(userid,
							&pvt->_pwdbuffer,
							pvt->_pwdcharbuffer,
							size))) {
				success=true;
				break;
			}
			#endif
			delete[] pvt->_pwdcharbuffer;
			pvt->_pwdcharbuffer=NULL;
			pvt->_pwd=NULL;
			if (error::getErrorNumber()!=ENOMEM) {
				return false;
			}
		}
	#else
		success=(!(uemutex && !uemutex->lock()) &&
			((pvt->_pwd=((username)
				?getpwnam(username)
				:getpwuid(userid)))!=NULL) &&
			!(uemutex && !uemutex->unlock()));
	#endif

	if (!success) {
		return false;
	}

	// get shadow info (but don't fail if this info isn't available)
	#if defined(RUDIMENTS_HAVE_GETSPNAM_R)
		// getspnam_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int32_t size=1024; size<MAXBUFFER; size=size+1024) {
			pvt->_spcharbuffer=new char[size];
			#if defined(RUDIMENTS_HAVE_GETSPNAM_R_5)
			if (!getspnam_r(getName(),&pvt->_spbuffer,
					pvt->_spcharbuffer,size,&pvt->_sp)) {
				break;
			}
			#elif defined(RUDIMENTS_HAVE_GETSPNAM_R_4)
			if ((pvt->_sp=getspnam_r(getName(),
					&pvt->_spbuffer,
					pvt->_spcharbuffer,size))) {
				break;
			}
			#endif
			delete[] pvt->_spcharbuffer;
			pvt->_spcharbuffer=NULL;
			pvt->_sp=NULL;
			if (error::getErrorNumber()!=ENOMEM) {
				break;
			}
		}
	#elif defined(RUDIMENTS_HAVE_GETSPNAM) 
		if (!_uemutex || _uemutex->lock()) {
			pvt->_sp=getspnam(const_cast<char *>(getName()));
		}
		if (_uemutex) {
			_uemutex->unlock();
		}
	#endif

	return success;

#else

	#if _WIN32_WINNT>=0x0500

	delete[] pvt->_name;
	pvt->_name=NULL;
	pvt->_primarygroupid=-1;
	delete[] pvt->_realname;
	pvt->_realname=NULL;
	delete[] pvt->_password;
	pvt->_password=NULL;
	delete[] pvt->_homedir;
	pvt->_homedir=NULL;
	LocalFree(pvt->_sid);
	pvt->_sid=NULL;
	pvt->_uid=(uid_t)-1;

	if (username) {	

		// get the user's SID
		DWORD		sidsize=0;
		DWORD		dnsize=0;
		SID_NAME_USE	peuse;
		LookupAccountName(NULL,username,
					NULL,&sidsize,
					NULL,&dnsize,&peuse);
		PSID	sid=(PSID)new BYTE[sidsize];
		bytestring::zero(sid,sidsize);
		CHAR	*dn=new CHAR[dnsize];
		bytestring::zero(dn,dnsize);
		bool	failed=(LookupAccountName(NULL,username,
						sid,&sidsize,
						dn,&dnsize,&peuse)==FALSE ||
				IsValidSid(sid)==FALSE ||
				ConvertSidToStringSid(sid,&pvt->_sid)==FALSE);
		delete[] (BYTE *)sid;
		delete[] dn;
		if (failed) {
			return false;
		}

		// convert username to unicode...
		WCHAR	*usernamew=asciiToUnicode(username);
		if (!usernamew) {
			return false;
		}

		// get user info
		USER_INFO_2	*buffer=NULL;
		if (NetUserGetInfo(NULL,usernamew,2,(BYTE **)&buffer)!=
								NERR_Success) {
			delete[] usernamew;
			return false;
		}
		delete[] usernamew;

		// convert the unicode values to ascii
		pvt->_name=unicodeToAscii(buffer->usri2_name);
		pvt->_password=unicodeToAscii(buffer->usri2_password);
		pvt->_realname=unicodeToAscii(buffer->usri2_full_name);
		pvt->_homedir=unicodeToAscii(buffer->usri2_home_dir);
		NetApiBufferFree(buffer);

		// get primary group
		// FIXME:
		// On non-servers, there are no local groups and the primary
		// group is None.  On servers (>Win2K) NetUserGetInfo can be
		// used with a level of 3 and get the primary group RID in the
		// resulting USER_INFO_3 struct.  It should then somehow be
		// possible to convert that into a group name.  How do I tell
		// if I'm on a server?  What should be done for <=Win2K?
		pvt->_primarygroupid=groupentry::getGroupId("None");

		// add mapping
		pvt->_uid=addUidMapping(pvt->_name,pvt->_sid);

	} else {

		// look up the uid in the map
		namesid	*ns;
		if (uidmap.getValue(userid,&ns)) {
			return initialize(ns->name);
		}
		return false;
	}
	return true;

	#else

	// FIXME: implement for WinNT
	return false;

	#endif
#endif
}

char *userentry::getName(uid_t userid) {
	userentry	pwd;
	return (pwd.initialize(userid))?
			charstring::duplicate(pwd.getName()):NULL;
}

uid_t userentry::getUserId(const char *username) {
	userentry	pwd;
	return (pwd.initialize(username))?pwd.getUserId():(uid_t)-1;
}

char *userentry::getSid(const char *username) {
	userentry	pwd;
	return (pwd.initialize(username))?
			charstring::duplicate(pwd.getSid()):NULL;
}
