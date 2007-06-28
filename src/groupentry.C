// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/groupentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

#include <stdio.h>
#include <stdlib.h>

#define MAXBUFFER (32*1024)

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class groupentryprivate {
	friend class groupentry;
	private:
		#ifndef MINGW32
			group		*_grp;
			#if defined(RUDIMENTS_HAVE_GETGRNAM_R) && \
				defined(RUDIMENTS_HAVE_GETGRGID_R)
				group		_grpbuffer;
				char		*_buffer;
			#endif
		#else
			LPGROUP_INFO_3	_buffer;
			char		*_name;
		#endif
};

// LAME: not in the class
#if (!defined(RUDIMENTS_HAVE_GETGRNAM_R) || !defined(RUDIMENTS_HAVE_GETGRUID_R))
static mutex	*_gemutex;
#endif

groupentry::groupentry() {
	pvt=new groupentryprivate;
	pvt->_grp=NULL;
#ifndef MINGW32
	#if defined(RUDIMENTS_HAVE_GETGRNAM_R) && \
		defined(RUDIMENTS_HAVE_GETGRGID_R)
		rawbuffer::zero(&pvt->_grpbuffer,sizeof(pvt->_grpbuffer));
		pvt->_buffer=NULL;
	#endif
#else
	pvt->_buffer=NULL;
	pvb->_name=NULL;
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
#ifndef MINGW32
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
#endif
}

const char *groupentry::getName() const {
#ifndef MINGW32
	return pvt->_grp->gr_name;
#else
	return pvt->_name;
#endif
}

const char *groupentry::getPassword() const {
#ifndef MINGW32
	return pvt->_grp->gr_passwd;
#else
	// FIXME:
	return "";
#endif
}

gid_t groupentry::getGroupId() const {
#ifndef MINGW32
	return pvt->_grp->gr_gid;
#else
	// FIXME:
	return 0;
#endif
}

#ifndef MINGW32
static char *members[]={NULL};
#endif

const char * const *groupentry::getMembers() const {
#ifndef MINGW32
	return pvt->_grp->gr_mem;
#else
	// FIXME:
	return members;
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

#ifndef MINGW32
bool groupentry::initialize(const char *groupname, gid_t groupid) {

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
		for (int size=1024; size<MAXBUFFER; size=size+1024) {

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
}
#else
bool groupentry::initialize(const char *groupname, gid_t groupid) {

	if (pvt->_buffer) {
		NetApiBufferFree(pvt->_buffer);
	}
	delete[] pvt->_name;
	pvt->_name=NULL;

	if (groupname) {	
		if (NetGroupGetInfo(NULL,groupname,3,
				reinterpret_cast<LPBYTE *>(&pvt->_buffer))!=
				NERR_SUCCESS) {
			return false;
		}
		pvt->_name=new char[charstring::length(
					pvt->_buffer->usri23_name)]+1;
		wsprintf(pvt->_name,"%s",pvt->_buffer->usri23_name);
	} else {
#error implement me...
	}
	return true;
}
#endif

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
		int	counter;
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
		int	counter;
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

	if (!pvt->_grp) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Password: %s\n",getPassword());
	printf("Group Id: %d\n",getGroupId());
	printf("Members:\n");
	for (int i=0; getMembers()[i]; i++) {
		printf("	%s\n",getMembers()[i]);
	}
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
