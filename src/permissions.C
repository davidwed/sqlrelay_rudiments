// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/permissions.h>

#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#include <sys/stat.h>


bool permissions::setFilePermissions(const char *filename, mode_t perms) {
	return (!chmod(filename,perms));
}

bool permissions::setFilePermissions(int fd, mode_t perms) {
	return (!fchmod(fd,perms));
}

mode_t permissions::everyoneReadWrite() {
	return S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
}

mode_t permissions::everyoneReadWriteExecute() {
	return S_IRUSR | S_IWUSR | S_IXUSR | 
		S_IRGRP | S_IWGRP | S_IXGRP | 
		S_IROTH | S_IWOTH | S_IXOTH;
}

mode_t permissions::ownerRead() {
	return S_IRUSR;
}

mode_t permissions::ownerWrite() {
	return S_IWUSR;
}

mode_t permissions::ownerExecute() {
	return S_IXUSR;
}

mode_t permissions::ownerReadWrite() {
	return S_IRUSR | S_IWUSR;
}

mode_t permissions::ownerReadExecute() {
	return S_IRUSR | S_IXUSR;
}

mode_t permissions::ownerReadWriteExecute() {
	return S_IRUSR | S_IWUSR | S_IXUSR;
}

mode_t permissions::groupRead() {
	return S_IRGRP;
}

mode_t permissions::groupWrite() {
	return S_IWGRP;
}

mode_t permissions::groupExecute() {
	return S_IXGRP;
}

mode_t permissions::groupReadWrite() {
	return S_IRGRP | S_IWGRP;
}

mode_t permissions::groupReadExecute() {
	return S_IRGRP | S_IXGRP;
}

mode_t permissions::groupReadWriteExecute() {
	return S_IRGRP | S_IWGRP | S_IXGRP;
}

mode_t permissions::othersRead() {
	return S_IROTH;
}

mode_t permissions::othersWrite() {
	return S_IWOTH;
}

mode_t permissions::othersExecute() {
	return S_IXOTH;
}

mode_t permissions::othersReadWrite() {
	return S_IROTH | S_IWOTH;
}

mode_t permissions::othersReadExecute() {
	return S_IROTH | S_IXOTH;
}

mode_t permissions::othersReadWriteExecute() {
	return S_IROTH | S_IWOTH | S_IXOTH;
}

mode_t permissions::saveInSwapSpace() {
	return S_ISVTX;
}

mode_t permissions::setUserId() {
	return S_ISUID;
}

mode_t permissions::setGroupId() {
	return S_ISGID;
}

mode_t permissions::evalPermString(const char *permstring) {
	mode_t	retval=0;
	if (strlen(permstring)==9) {

		// handle user permissions
		if (permstring[0]=='r') {
			retval=retval|S_IRUSR;
		}
		if (permstring[1]=='w') {
			retval=retval|S_IWUSR;
		}
		if (permstring[2]=='x') {
			retval=retval|S_IXUSR;
		} else if (permstring[2]=='X' || permstring[2]=='S') {
			retval=retval|S_IXUSR;
			retval=retval|S_ISUID;
		}

		// handle group permissions
		if (permstring[3]=='r') {
			retval=retval|S_IRGRP;
		}
		if (permstring[4]=='w') {
			retval=retval|S_IWGRP;
		}
		if (permstring[5]=='x') {
			retval=retval|S_IXGRP;
		} else if (permstring[5]=='X' || permstring[5]=='S') {
			retval=retval|S_IXGRP;
		}

		// handle others permissions
		if (permstring[6]=='r') {
			retval=retval|S_IROTH;
		}
		if (permstring[7]=='w') {
			retval=retval|S_IWOTH;
		}
		if (permstring[8]=='x') {
			retval=retval|S_IXOTH;

		// handle sticky bit
		} else if (permstring[5]=='t') {
			retval=retval|S_ISVTX;
		}
	}
	return retval;
}

char *permissions::evalPermOctal(mode_t permoctal) {

	char	*permstring=new char[10];
	permstring[9]=(char)NULL;

	mode_t	shift=permoctal;
	for (int i=8; i>=0; i--) {
		int	pos=i%3;
		permstring[i]=(shift&1)?((pos==2)?'x':(pos==1)?'w':'r'):'-';
		shift=shift>>1;
	}
	return permstring;
}
