// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS
	#include <strings.h>
#endif

inline groupentry::groupentry() {
	grp=NULL;
	buffer=NULL;
}

inline groupentry::~groupentry() {
	delete[] buffer;
}

inline char *groupentry::getName() const {
	return grp->gr_name;
}

inline char *groupentry::getPassword() const {
	return grp->gr_passwd;
}

inline gid_t groupentry::getGroupId() const {
	return grp->gr_gid;
}

inline char **groupentry::getMembers() const {
	return grp->gr_mem;
}

inline char *groupentry::getPassword(const char *groupname) {
	groupentry	grp;
	return (grp.initialize(groupname))?strdup(grp.getPassword()):NULL;
}

inline gid_t groupentry::getGroupId(const char *groupname) {
	groupentry	grp;
	return (grp.initialize(groupname))?grp.getGroupId():-1;
}

inline char **groupentry::getMembers(const char *groupname) {
	groupentry	grp;
	if (!grp.initialize(groupname)) {
		return NULL;
	}
	int	counter;
	for (counter=0; grp.getMembers()[counter]; counter++);
	char	**members=new char *[counter+1];
	members[counter]=NULL;
	for (int i=0; i<counter; i++) {
		members[i]=strdup(grp.getMembers()[i]);
	}
	return members;
}

inline char *groupentry::getName(gid_t groupid) {
	groupentry	grp;
	return (grp.initialize(groupid))?strdup(grp.getName()):NULL;
}

inline char *groupentry::getPassword(gid_t groupid) {
	groupentry	grp;
	return (grp.initialize(groupid))?strdup(grp.getPassword()):NULL;
}

inline char **groupentry::getMembers(gid_t groupid) {
	groupentry	grp;
	if (!grp.initialize(groupid)) {
		return NULL;
	}
	int	counter;
	for (counter=0; grp.getMembers()[counter]; counter++);
	char	**members=new char *[counter+1];
	members[counter]=NULL;
	for (int i=0; i<counter; i++) {
		members[i]=strdup(grp.getMembers()[i]);
	}
	return members;
}
