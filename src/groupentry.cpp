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

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
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
static mutex	*_gemutex;
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

void groupentry::setMutex(mutex *mtx) {
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

		// get the size of the unicode buffer
		int32_t	groupnamewsize=MultiByteToWideChar(CP_ACP,
							MB_PRECOMPOSED,
							groupname,
							-1,NULL,0);
		if (!groupnamewsize) {
			return false;
		}

		// create the buffer
		WCHAR	*groupnamew=new WCHAR[groupnamewsize];

		// perform the actual conversion
		groupnamewsize=MultiByteToWideChar(CP_ACP,
							MB_PRECOMPOSED,
							groupname,-1,
							groupnamew,
							groupnamewsize);
		if (!groupnamewsize) {
			delete[] groupnamew;
			return false;
		}

		// get the group info
		if (NetGroupGetInfo(NULL,groupnamew,2,
				reinterpret_cast<LPBYTE *>(&pvt->_buffer))!=
				NERR_Success) {
			delete[] groupnamew;
			return false;
		}

		// convert the name back to ascii
		pvt->_name=new char[groupnamewsize];
		BOOL	useddefaultchar;
		if (!WideCharToMultiByte(CP_ACP,0,
						pvt->_buffer->grpi2_name,
						groupnamewsize,
						pvt->_name,
						groupnamewsize,
						"?",
						&useddefaultchar)) {
			delete[] pvt->_name;
			pvt->_name=NULL;
			delete[] groupnamew;
			return false;
		}

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

bool groupentry::getPassword(const char *groupname, char **password) {
	groupentry	grp;
	if (grp.initialize(groupname)) {
		*password=charstring::duplicate(grp.getPassword());
		return true;
	}
	return false;
}

bool groupentry::getGroupId(const char *groupname, gid_t *groupid) {
	groupentry	grp;
	if (grp.initialize(groupname)) {
		*groupid=grp.getGroupId();
		return true;
	}
	return false;
}

bool groupentry::getMembers(const char *groupname, char ***members) {
	groupentry	grp;
	if (grp.initialize(groupname)) {
		int32_t	counter;
		for (counter=0; grp.getMembers()[counter]; counter++);
		char	**memb=new char *[counter+1];
		memb[counter]=NULL;
		for (int i=0; i<counter; i++) {
			memb[i]=charstring::duplicate(grp.getMembers()[i]);
		}
		*members=memb;
		return true;
	}
	return false;
}

bool groupentry::getName(gid_t groupid, char **name) {
	groupentry	grp;
	if (grp.initialize(groupid)) {
		*name=charstring::duplicate(grp.getName());
		return true;
	}
	return false;
}

bool groupentry::getPassword(gid_t groupid, char **password) {
	groupentry	grp;
	if (grp.initialize(groupid)) {
		*password=charstring::duplicate(grp.getPassword());
		return true;
	}
	return false;
}

bool groupentry::getMembers(gid_t groupid, char ***members) {
	groupentry	grp;
	if (grp.initialize(groupid)) {
		int32_t	counter;
		for (counter=0; grp.getMembers()[counter]; counter++);
		char	**memb=new char *[counter+1];
		memb[counter]=NULL;
		for (int i=0; i<counter; i++) {
			memb[i]=charstring::duplicate(grp.getMembers()[i]);
		}
		*members=memb;
		return true;
	}
	return false;
}

void groupentry::print() const {

#ifdef RUDIMENTS_HAVE_NETGROUPGETINFO
	if (!pvt->_buffer) {
		return;
	}
#else
	if (!pvt->_grp) {
		return;
	}
#endif

	stdoutput.printf("Name: %s\n",getName());
	stdoutput.printf("Password: %s\n",getPassword());
	stdoutput.printf("Group Id: %ld\n",(long)getGroupId());
	stdoutput.printf("Members:\n");
	for (int i=0; getMembers()[i]; i++) {
		stdoutput.printf("	%s\n",getMembers()[i]);
	}
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
