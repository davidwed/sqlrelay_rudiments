// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/permissions.h>
#ifndef ENABLE_INLINES
	#include <rudiments/private/permissionsinlines.h>
#endif

#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

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
