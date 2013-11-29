// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/groupentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/sys.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_HAVE_NETGROUPGETINFO
	// for structs, functions
	#ifdef RUDIMENTS_HAVE_WINDOWS_H
		#include <windows.h>
	#endif
	#include <lm.h>
#else
	// for group, functions
	#include <grp.h>
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

class groupentryprivate {
	friend class groupentry;
	private:
		#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO
			group		*_grp;
			#if defined(RUDIMENTS_HAVE_GETGRNAM_R) && \
				defined(RUDIMENTS_HAVE_GETGRGID_R)
				group		_grpbuffer;
				char		*_buffer;
			#endif
		#else
			GROUP_INFO_2		*_buffer;
			char			*_name;
			GROUP_USERS_INFO_0	*_membersbuffer;
			char			**_members;
			DWORD			_membercount;
		#endif
};

// LAME: not in the class
#if (!defined(RUDIMENTS_HAVE_GETGRNAM_R) || !defined(RUDIMENTS_HAVE_GETGRUID_R))
static threadmutex	*_gemutex;
#endif

#ifdef RUDIMENTS_HAVE_NETGROUPGETINFO
static CHAR *asciiToUnicode(CHAR *in) {

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

static CHAR *unicodeToAscii(WCHAR *in) {

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
#endif

groupentry::groupentry() {
	pvt=new groupentryprivate;
#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO
	pvt->_grp=NULL;
	#if defined(RUDIMENTS_HAVE_GETGRNAM_R) && \
		defined(RUDIMENTS_HAVE_GETGRGID_R)
		rawbuffer::zero(&pvt->_grpbuffer,sizeof(pvt->_grpbuffer));
		pvt->_buffer=NULL;
	#endif
#else
	pvt->_buffer=NULL;
	pvt->_name=NULL;
	pvt->_members=NULL;
	pvt->_membercount=0;
#endif
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
	delete pvt;
#else
	if (pvt->_buffer) {
		NetApiBufferFree(pvt->_buffer);
	}
	delete[] pvt->_name;
	for (DWORD i=0; i<pvt->_membercount; i++) {
		delete[] pvt->_members[i];
	}
	delete[] pvt->_members;
#endif
}

const char *groupentry::getName() const {
#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO
	return pvt->_grp->gr_name;
#else
	return pvt->_name;
#endif
}

const char *groupentry::getPassword() const {
#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO
	return pvt->_grp->gr_passwd;
#else
	return "";
#endif
}

gid_t groupentry::getGroupId() const {
#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO
	return pvt->_grp->gr_gid;
#else
	return pvt->_buffer->grpi2_group_id;
#endif
}

const char * const *groupentry::getMembers() const {
#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO
	return pvt->_grp->gr_mem;
#else
	return pvt->_members;
#endif
}

bool groupentry::needsMutex() {
	#if !defined(RUDIMENTS_HAVE_GETGRNAM_R) || \
		!defined(RUDIMENTS_HAVE_GETGRUID_R)
		return true;
	#else
		return false;
	#endif
}

void groupentry::setMutex(threadmutex *mtx) {
	#if !defined(RUDIMENTS_HAVE_GETGRNAM_R) || \
		!defined(RUDIMENTS_HAVE_GETGRUID_R)
		_gemutex=mtx;
	#endif
}

bool groupentry::initialize(const char *groupname) {
	return initialize(groupname,0);
}

bool groupentry::initialize(gid_t groupid) {
	return initialize(NULL,groupid);
}

bool groupentry::initialize(const char *groupname, gid_t groupid) {
#ifndef RUDIMENTS_HAVE_NETGROUPGETINFO

	#if defined(RUDIMENTS_HAVE_GETGRNAM_R) && \
		defined(RUDIMENTS_HAVE_GETGRGID_R)
		if (pvt->_grp) {
			pvt->_grp=NULL;
			delete[] pvt->_buffer;
			pvt->_buffer=NULL;
		}
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
		pvt->_grp=NULL;
		return (!(_gemutex && !_gemutex->lock()) &&
			((pvt->_grp=((groupname)
				?getgrnam(groupname)
				:getgrgid(groupid)))!=NULL) &&
			!(_gemutex && !_gemutex->unlock()));
	#endif

#else

	// clear old buffers
	if (pvt->_buffer) {
		NetApiBufferFree(pvt->_buffer);
	}
	delete[] pvt->_name;
	pvt->_name=NULL;
	for (DWORD i=0; i<pvt->_membercount; i++) {
		delete[] pvt->_members[i];
	}
	delete[] pvt->_members;
	pvt->_membercount=0;

	if (groupname) {

		// convert groupname to unicode...
		WCHAR	*groupnamew=asciiToUnicode(groupname);
		if (!groupnamew) {
			return false;
		}

		// get the group info
		if (NetGroupGetInfo(NULL,groupnamew,2,(BYTE **)&pvt->_buffer)!=
								NERR_Success) {
			delete[] groupnamew;
			return false;
		}

		// convert the name back to ascii
		pvt->_name=unicodeToAscii(pvt->_buffer->grpi2_name);

		// get group members
		DWORD	totalentries;
		if (!NetGroupGetUsers(NULL,groupnamew,0,
				reinterpret_cast<LPBYTE *>(
						&pvt->_membersbuffer),
				MAX_PREFERRED_LENGTH,
				&pvt->_membercount,
				&totalentries,
				NULL)) {
			delete[] groupnamew;
			return false;
		}
		delete[] groupnamew;

		// copy unicode members to ascii members buffer
		pvt->_members=new char *[pvt->_membercount+1];
		pvt->_members[pvt->_membercount]=NULL;
		for (DWORD i=0; i<pvt->_membercount; i++) {
			size_t	len=charstring::length(
					reinterpret_cast<char *>(
					pvt->_membersbuffer[i].grui0_name));
			pvt->_members[i]=new char[len+1];
			if (!WideCharToMultiByte(CP_ACP,0,
						pvt->_membersbuffer[i].
								grui0_name,
						len,
						pvt->_members[i],
						len,
						"?",
						&useddefaultchar)) {
				return false;
			}
		}

		return true;
		
	} else {
		// windows doesn't appear to support this
		error::setErrorNumber(ENOSYS);
		return false;
	}
	return true;
#endif
}

gid_t groupentry::getGroupId(const char *groupname) {
	groupentry	grp;
	if (grp.initialize(groupname)) {
		return grp.getGroupId();
	}
	return (gid_t)-1;
}

char *groupentry::getName(gid_t groupid) {
	groupentry	grp;
	if (grp.initialize(groupid)) {
		return charstring::duplicate(grp.getName());
	}
	return NULL;
}
