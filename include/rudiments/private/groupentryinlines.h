// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

groupentry::groupentry() {
	grp=NULL;
	buffer=NULL;
}

groupentry::~groupentry() {
	delete grp;
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
