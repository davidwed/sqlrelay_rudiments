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

inline int groupentry::getPassword(const char *groupname, char **password) {
	groupentry	grp;
	if (grp.initialize(groupname)) {
		*password=strdup(grp.getPassword());
		return 1;
	}
	return 0;
}

inline int groupentry::getGroupId(const char *groupname, gid_t *groupid) {
	groupentry	grp;
	if (grp.initialize(groupname)) {
		*groupid=grp.getGroupId();
		return 1;
	}
	return 0;
}

inline int groupentry::getMembers(const char *groupname, char ***members) {
	groupentry	grp;
	if (grp.initialize(groupname)) {
		int	counter;
		for (counter=0; grp.getMembers()[counter]; counter++);
		char	**memb=new char *[counter+1];
		memb[counter]=NULL;
		for (int i=0; i<counter; i++) {
			memb[i]=strdup(grp.getMembers()[i]);
		}
		*members=memb;
		return 1;
	}
	return 0;
}

inline int groupentry::getName(gid_t groupid, char **name) {
	groupentry	grp;
	if (grp.initialize(groupid)) {
		*name=strdup(grp.getName());
		return 1;
	}
	return 0;
}

inline int groupentry::getPassword(gid_t groupid, char **password) {
	groupentry	grp;
	if (grp.initialize(groupid)) {
		*password=strdup(grp.getPassword());
		return 1;
	}
	return 0;
}

inline int groupentry::getMembers(gid_t groupid, char ***members) {
	groupentry	grp;
	if (grp.initialize(groupid)) {
		int	counter;
		for (counter=0; grp.getMembers()[counter]; counter++);
		char	**memb=new char *[counter+1];
		memb[counter]=NULL;
		for (int i=0; i<counter; i++) {
			memb[i]=strdup(grp.getMembers()[i]);
		}
		*members=memb;
		return 1;
	}
	return 0;
}
