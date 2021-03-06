// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/groupentry.h>

#ifndef __VMS

#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/sys.h>
#include <rudiments/stdio.h>
#include <rudiments/error.h>

#if defined(RUDIMENTS_HAVE_NETGROUPGETINFO)
	#include <rudiments/dictionary.h>
#elif defined(RUDIMENTS_HAVE_GRP_H)
	#include <grp.h>
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

class groupentryprivate {
	friend class groupentry;
	private:
		#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO
			group	*_grp;
			#if defined(RUDIMENTS_HAVE_GETGRNAM_R) && \
				defined(RUDIMENTS_HAVE_GETGRGID_R)
				group		_grpbuffer;
				char		*_buffer;
			#endif
			char		*_sidstr;
			const char	*_sid;
			size_t		_sidsize;
		#else
			char		*_name;
			char		**_members;
			DWORD		_membercount;
			CHAR		*_sidstr;
			PSID		_sid;
			DWORD		_sidsize;
			gid_t		_gid;
		#endif
};

// LAME: not in the class
#if (!defined(RUDIMENTS_HAVE_GETGRNAM_R) || !defined(RUDIMENTS_HAVE_GETGRUID_R))
static threadmutex	*gemutex;
#endif

#ifdef RUDIMENTS_HAVE_NETGROUPGETINFO
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

static gid_t	gid=0;
struct namesid {
	char	*name;
	char	*sidstr;
	PSID	sid;
	DWORD	sidsize;
};
static dictionary< gid_t, namesid * >	gidmap;
// FIXME: clean up with init and exit methods like environment class

static gid_t addGidMapping(const char *name,
				const char *sidstr, PSID sid, DWORD sidsize) {

	// check for existing mapping (by name only)
	for (linkedlistnode< dictionarynode< gid_t, namesid * > *>
				*node=gidmap.getList()->getFirst(); 
				node; node=node->getNext()) {
		namesid	*ns=node->getValue()->getValue();
		if (!charstring::compare(name,ns->name)) {
			// reset the sid
			delete[] ns->sidstr;
			ns->sidstr=charstring::duplicate(sidstr);
			delete[] (BYTE *)ns->sid;
			ns->sid=bytestring::duplicate(sid,sidsize);
			ns->sidsize=sidsize;
			return node->getValue()->getKey();
		}
	}

	// create a new entry
	namesid	*ns=new namesid;
	ns->name=charstring::duplicate(name);
	ns->sidstr=charstring::duplicate(sidstr);
	ns->sid=bytestring::duplicate(sid,sidsize);
	ns->sidsize=sidsize;

	if (gemutex) {
		gemutex->lock();
	}
	gid_t	g=gid;
	gid++;
	if (gemutex) {
		gemutex->unlock();
	}

	gidmap.setValue(g,ns);
	return g;
}
#endif

groupentry::groupentry() {
	pvt=new groupentryprivate;
#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO
	pvt->_grp=NULL;
	#if defined(RUDIMENTS_HAVE_GETGRNAM_R) && \
		defined(RUDIMENTS_HAVE_GETGRGID_R)
		bytestring::zero(&pvt->_grpbuffer,sizeof(pvt->_grpbuffer));
		pvt->_buffer=NULL;
	#endif
#else
	pvt->_name=NULL;
	pvt->_members=NULL;
	pvt->_membercount=0;
	pvt->_gid=(gid_t)-1;
#endif
	pvt->_sidstr=NULL;
	pvt->_sid=NULL;
	pvt->_sidsize=0;
}

groupentry::groupentry(const groupentry &g) {
	pvt=new groupentryprivate;
	initialize(g.getName());
}

groupentry &groupentry::operator=(const groupentry &g) {
	if (this!=&g) {
		initialize(g.getName());
	}
	return *this;
}

groupentry::~groupentry() {
#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO
	#if defined(RUDIMENTS_HAVE_GETGRNAM_R) && \
		defined(RUDIMENTS_HAVE_GETGRGID_R)
		delete[] pvt->_buffer;
	#endif
	delete[] pvt->_sidstr;
#else
	delete[] pvt->_name;
	for (DWORD i=0; i<pvt->_membercount; i++) {
		delete[] pvt->_members[i];
	}
	delete[] pvt->_members;
	LocalFree(pvt->_sidstr);
	delete[] (BYTE *)pvt->_sid;
#endif
	delete pvt;
}

const char *groupentry::getName() const {
#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO
	return (pvt->_grp)?pvt->_grp->gr_name:NULL;
#else
	return pvt->_name;
#endif
}

gid_t groupentry::getGroupId() const {
#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO
	return (pvt->_grp)?pvt->_grp->gr_gid:(gid_t)-1;
#else
	return pvt->_gid;
#endif
}

const char * const *groupentry::getMembers() const {
#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO
	// If a group has no members then pvt->_grp->gr_mem will be an
	// array containing a single NULL or empty member.  Rather than
	// returning that, just return NULL.
	if (pvt->_grp && pvt->_grp->gr_mem &&
		!charstring::isNullOrEmpty(pvt->_grp->gr_mem[0])) {
		return pvt->_grp->gr_mem;
	}
	return NULL;
#else
	return pvt->_members;
#endif
}

const char *groupentry::getSidString() const {
#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO
	if (!pvt->_sidstr) {
		if (pvt->_grp) {
			pvt->_sidstr=charstring::parseNumber(
						(int64_t)pvt->_grp->gr_gid);
		}
		pvt->_sid=pvt->_sidstr;
		pvt->_sidsize=charstring::length(pvt->_sidstr);
	}
	return pvt->_sidstr;
#else
	return pvt->_sidstr;
#endif
}

const void *groupentry::getSid() const {
#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO
	if (!pvt->_sidstr) {
		if (pvt->_grp) {
			pvt->_sidstr=charstring::parseNumber(
						(int64_t)pvt->_grp->gr_gid);
		}
		pvt->_sid=pvt->_sidstr;
		pvt->_sidsize=charstring::length(pvt->_sidstr);
	}
	return pvt->_sid;
#else
	return pvt->_sid;
#endif
}

uint64_t groupentry::getSidSize() const {
#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO
	if (!pvt->_sidstr) {
		if (pvt->_grp) {
			pvt->_sidstr=charstring::parseNumber(
						(int64_t)pvt->_grp->gr_gid);
		}
		pvt->_sid=pvt->_sidstr;
		pvt->_sidsize=charstring::length(pvt->_sidstr);
	}
	return pvt->_sidsize;
#else
	return pvt->_sidsize;
#endif
}

bool groupentry::platformSupportsFormalSid() {
	#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO
		return false;
	#else
		return true;
	#endif
}

bool groupentry::needsMutex() {
	#if (!defined(RUDIMENTS_HAVE_GETGRNAM_R) || \
		!defined(RUDIMENTS_HAVE_GETGRUID_R)) || \
		defined(RUDIMENTS_HAVE_NETGROUPGETINFO)
		return true;
	#else
		return false;
	#endif
}

void groupentry::setMutex(threadmutex *mtx) {
	#if (!defined(RUDIMENTS_HAVE_GETGRNAM_R) || \
		!defined(RUDIMENTS_HAVE_GETGRUID_R)) || \
		defined(RUDIMENTS_HAVE_NETGROUPGETINFO)
		gemutex=mtx;
	#endif
}

bool groupentry::initialize(const char *groupname) {
	return initialize(groupname,(gid_t)-1);
}

bool groupentry::initialize(gid_t groupid) {
	return initialize(NULL,groupid);
}

bool groupentry::initialize(const char *groupname, gid_t groupid) {
#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO

	// (re)init buffers
	delete[] pvt->_sidstr;
	pvt->_sidstr=NULL;
	pvt->_sid=NULL;
	pvt->_sidsize=0;
	#if defined(RUDIMENTS_HAVE_GETGRNAM_R) && \
		defined(RUDIMENTS_HAVE_GETGRGID_R)
		if (pvt->_grp) {
			pvt->_grp=NULL;
			delete[] pvt->_buffer;
			pvt->_buffer=NULL;
		}
	#else
		pvt->_grp=NULL;
	#endif

	// catch invalid name/id here...
	// We have to do this rather than just letting one of the functions
	// below fail because on some systems (SCO OSR) the actually interpret
	// any negative group id as root and on others (SCO UnixWare) they
	// interpret -1 as root.  Ideally we'd check for any negative number,
	// but on most systems, gid is unsigned.  What a mess.
	if (charstring::isNullOrEmpty(groupname) && groupid==(gid_t)-1) {
		return false;
	}

	#if defined(RUDIMENTS_HAVE_GETGRNAM_R) && \
		defined(RUDIMENTS_HAVE_GETGRGID_R)
		// getgrnam_r and getgrgid_t are goofy.
		// They will retrieve an arbitrarily large amount of data, but
		// require that you pass them a pre-allocated buffer.  If the
		// buffer is too small, they return an ENOMEM and you have to
		// just make the buffer bigger and try again.
		int64_t	inc=sys::getSuggestedGroupEntryBufferSize();
		int64_t	max=inc*32;
		for (int64_t size=inc; size<max; size=size+inc) {

			pvt->_buffer=new char[size];
			#if defined(RUDIMENTS_HAVE_GETGRNAM_R_5) && \
				defined(RUDIMENTS_HAVE_GETGRGID_R_5)
			if (!((groupname)
				?(getgrnam_r(groupname,
						&pvt->_grpbuffer,
						pvt->_buffer,size,
						&pvt->_grp))
				:(getgrgid_r(groupid,
						&pvt->_grpbuffer,
						pvt->_buffer,size,
						&pvt->_grp)))) {
				return (pvt->_grp!=NULL);
			}
			#elif defined(RUDIMENTS_HAVE_GETGRNAM_R_4) && \
				defined(RUDIMENTS_HAVE_GETGRGID_R_4)
			if ((groupname)
				?(pvt->_grp=getgrnam_r(groupname,
							&pvt->_grpbuffer,
							pvt->_buffer,size))
				:(pvt->_grp=getgrgid_r(groupid,
							&pvt->_grpbuffer,
							pvt->_buffer,size))) {
				return true;
			}
			#endif
			delete[] pvt->_buffer;
			pvt->_buffer=NULL;
			pvt->_grp=NULL;
			if (error::getErrorNumber()!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		return (!(gemutex && !gemutex->lock()) &&
			((pvt->_grp=((groupname)
				?getgrnam(groupname)
				:getgrgid(groupid)))!=NULL) &&
			!(gemutex && !gemutex->unlock()));
	#endif

#else

	// (re)init buffers
	delete[] pvt->_name;
	pvt->_name=NULL;
	for (DWORD i=0; i<pvt->_membercount; i++) {
		delete[] pvt->_members[i];
	}
	delete[] pvt->_members;
	pvt->_members=NULL;
	pvt->_membercount=0;
	LocalFree(pvt->_sidstr);
	pvt->_sidstr=NULL;
	delete[] (BYTE *)pvt->_sid;
	pvt->_sid=NULL;
	pvt->_sidsize=0;
	pvt->_gid=(gid_t)-1;

	if (groupname) {

		// get the group's SID
		DWORD		dnsize=0;
		SID_NAME_USE	peuse;
		LookupAccountName(NULL,groupname,
					NULL,&pvt->_sidsize,
					NULL,&dnsize,&peuse);
		pvt->_sid=(PSID)new BYTE[pvt->_sidsize];
		bytestring::zero(pvt->_sid,pvt->_sidsize);
		CHAR	*dn=new CHAR[dnsize];
		bytestring::zero(dn,dnsize);
		bool	failed=(LookupAccountName(NULL,groupname,
						pvt->_sid,&pvt->_sidsize,
						dn,&dnsize,&peuse)==FALSE ||
				IsValidSid(pvt->_sid)==FALSE
				#if _WIN32_WINNT>=0x0500
				|| ConvertSidToStringSid(pvt->_sid,
						&pvt->_sidstr)==FALSE
				#endif
				);
		delete[] dn;
		if (failed) {
			return false;
		}

		// convert groupname to unicode...
		WCHAR	*groupnamew=asciiToUnicode(groupname);
		if (!groupnamew) {
			return false;
		}

		// get the group info
		GROUP_INFO_0	*buffer=NULL;
		if (NetGroupGetInfo(NULL,groupnamew,0,(BYTE **)&buffer)!=
								NERR_Success) {
			delete[] groupnamew;
			return false;
		}

		// convert the name back to ascii
		pvt->_name=unicodeToAscii(buffer->grpi0_name);
		NetApiBufferFree(buffer);

		// get group members
		DWORD			totalentries;
		GROUP_USERS_INFO_0	*membersbuffer=NULL;
		if (NetGroupGetUsers(NULL,groupnamew,0,
					(BYTE **)&membersbuffer,
					MAX_PREFERRED_LENGTH,
					&pvt->_membercount,
					&totalentries,
					NULL)!=NERR_Success) {
			delete[] groupnamew;
			return false;
		}
		delete[] groupnamew;

		// copy unicode members to ascii members buffer
		pvt->_members=new char *[pvt->_membercount+1];
		pvt->_members[pvt->_membercount]=NULL;
		for (DWORD i=0; i<pvt->_membercount; i++) {
			pvt->_members[i]=unicodeToAscii(
						membersbuffer[i].grui0_name);
		}
		NetApiBufferFree(membersbuffer);

		// add mapping
		pvt->_gid=addGidMapping(pvt->_name,pvt->_sidstr,
					pvt->_sid,pvt->_sidsize);

	} else {

		// look up the gid in the map
		namesid	*ns;
		if (gidmap.getValue(groupid,&ns)) {
			return initialize(ns->name);
		}
		return false;
	}
	return true;
#endif
}

gid_t groupentry::getGroupId(const char *groupname) {
	groupentry	grp;
	return (grp.initialize(groupname))?grp.getGroupId():(gid_t)-1;
}

char *groupentry::getName(gid_t groupid) {
	groupentry	grp;
	return (grp.initialize(groupid))?
			charstring::duplicate(grp.getName()):NULL;
}

char *groupentry::getSidString(const char *groupname) {
	groupentry	grp;
	return (grp.initialize(groupname))?
			charstring::duplicate(grp.getSidString()):NULL;
}

#else

groupentry::groupentry() {
}

groupentry::groupentry(const groupentry &g) {
}

groupentry &groupentry::operator=(const groupentry &g) {
	return *this;
}

groupentry::~groupentry() {
}

const char *groupentry::getName() const {
	return NULL;
}

gid_t groupentry::getGroupId() const {
	return 0;
}

const char * const *groupentry::getMembers() const {
	return NULL;
}

const char *groupentry::getSidString() const {
	return NULL;
}

bool groupentry::platformSupportsFormalSid() {
	return false;
}

bool groupentry::needsMutex() {
	return true;
}

void groupentry::setMutex(threadmutex *mtx) {
}

bool groupentry::initialize(const char *groupname) {
	return initialize(groupname,0);
}

bool groupentry::initialize(gid_t groupid) {
	return initialize(NULL,groupid);
}

bool groupentry::initialize(const char *groupname, gid_t groupid) {
	return true;
}

gid_t groupentry::getGroupId(const char *groupname) {
	return 0;
}

char *groupentry::getName(gid_t groupid) {
	return NULL;
}

char *groupentry::getSidString(const char *groupname) {
	return NULL;
}

#endif
