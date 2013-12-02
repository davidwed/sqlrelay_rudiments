// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/passwdentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/sys.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_HAVE_NETUSERGETINFO
	#include <rudiments/dictionary.h>
	#include <rudiments/groupentry.h>
	#ifdef RUDIMENTS_HAVE_WINDOWS_H
		#include <windows.h>
	#endif
	#include <lm.h>
	#include <sddl.h>
#else
	#include <pwd.h>
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
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
			char	*_sid;
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
static threadmutex	*pemutex;
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
	for (linkedlistnode< dictionarynode< uid_t, namesid * > *> *node=
					uidmap.getList()->getFirstNode(); 
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

	if (pemutex) {
		pemutex->lock();
	}
	uid_t	u=uid;
	uid++;
	if (pemutex) {
		pemutex->unlock();
	}

	uidmap.setValue(u,ns);
	return u;
}
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
	pvt->_name=NULL;
	pvt->_password=NULL;
	pvt->_primarygroupid=-1;
	pvt->_realname=NULL;
	pvt->_homedir=NULL;
	pvt->_uid=(uid_t)-1;
#endif
	pvt->_sid=NULL;
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

const char *passwdentry::getName() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return (pvt->_pwd)?pvt->_pwd->pw_name:NULL;
#else
	return pvt->_name;
#endif
}

const char *passwdentry::getPassword() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return (pvt->_pwd)?pvt->_pwd->pw_passwd:NULL;
#else
	return pvt->_password;
#endif
}

uid_t passwdentry::getUserId() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return (pvt->_pwd)?pvt->_pwd->pw_uid:(uid_t)-1;
#else
	return pvt->_uid;
#endif
}

const char *passwdentry::getSid() const {
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

gid_t passwdentry::getPrimaryGroupId() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return (pvt->_pwd)?pvt->_pwd->pw_gid:(gid_t)-1;
#else
	return (gid_t)pvt->_primarygroupid;
#endif
}

const char *passwdentry::getRealName() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return (pvt->_pwd)?pvt->_pwd->pw_gecos:NULL;
#else
	return pvt->_realname;
#endif
}

const char *passwdentry::getHomeDirectory() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return (pvt->_pwd)?pvt->_pwd->pw_dir:NULL;
#else
	return pvt->_homedir;
#endif
}

const char *passwdentry::getShell() const {
#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
	return (pvt->_pwd)?pvt->_pwd->pw_shell:NULL;
#else
	// Under windows, users don't have default shells.  As far as I know,
	// the command line is always the same.  You can run other shells
	// but they're not tied to a user.
	return NULL;
#endif
}

bool passwdentry::platformSupportsFormalSid() {
	#ifndef RUDIMENTS_HAVE_NETUSERGETINFO
		return false;
	#else
		return true;
	#endif
}

bool passwdentry::needsMutex() {
	#if (!defined(RUDIMENTS_HAVE_GETPWNAM_R) || \
		!defined(RUDIMENTS_HAVE_GETPWUID_R)) || \
		defined(RUDIMENTS_HAVE_NETUSERGETINFO)
		return true;
	#else
		return false;
	#endif
}

void passwdentry::setMutex(threadmutex *mtx) {
	#if (!defined(RUDIMENTS_HAVE_GETPWNAM_R) || \
		!defined(RUDIMENTS_HAVE_GETPWUID_R)) || \
		defined(RUDIMENTS_HAVE_NETUSERGETINFO)
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

	delete[] pvt->_sid;
	pvt->_sid=NULL;

	#if defined(RUDIMENTS_HAVE_GETPWNAM_R) && \
		defined(RUDIMENTS_HAVE_GETPWUID_R)
		delete[] pvt->_sid;
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
		int64_t	inc=sys::getSuggestedPasswordEntryBufferSize();
		int64_t	max=inc*32;
		for (int64_t size=inc; size<max; size=size+inc) {

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
		rawbuffer::zero(sid,sidsize);
		CHAR	*dn=new CHAR[dnsize];
		rawbuffer::zero(dn,dnsize);
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
#endif
}

char *passwdentry::getName(uid_t userid) {
	passwdentry	pwd;
	if (pwd.initialize(userid)) {
		return charstring::duplicate(pwd.getName());
	}
	return NULL;
}

uid_t passwdentry::getUserId(const char *username) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		return pwd.getUserId();
	}
	return (uid_t)-1;
}

char *passwdentry::getSid(const char *username) {
	passwdentry	pwd;
	if (pwd.initialize(username)) {
		return charstring::duplicate(pwd.getSid());
	}
	return NULL;
}
