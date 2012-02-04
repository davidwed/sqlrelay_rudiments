// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/permissions.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>
#include <rudiments/file.h>

#include <stdlib.h>
#include <sys/stat.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

bool permissions::setFilePermissions(const char *filename, mode_t perms) {
	file	fl;
	return (fl.open(filename,O_RDWR) &&
		setFilePermissions(fl.getFileDescriptor(),perms));
}

bool permissions::setFilePermissions(int32_t fd, mode_t perms) {
	#if defined(RUDIMENTS_HAVE_FCHMOD)
		int32_t	result;
		do {
			result=fchmod(fd,perms);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(_WIN32)
		// windows doesn't support anything like this
		error::setErrorNumber(ENOSYS);
		return false;
	#else
		// other platforms should support something like this
		#error no fchmod or anything like it
	#endif
}

mode_t permissions::everyoneReadWrite() {
	return S_IRUSR|S_IWUSR
	#if defined(S_IRGRP) && \
		defined(S_IWGRP) && \
		defined(S_IROTH) && \
		defined(S_IWOTH)
		|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
	#endif
	;
}

mode_t permissions::everyoneReadWriteExecute() {
	return S_IRUSR|S_IWUSR|S_IXUSR
	#if defined(S_IRGRP) && \
		defined(S_IWGRP) && \
		defined(S_IXGRP) && \
		defined(S_IROTH) && \
		defined(S_IWOTH) && \
		defined(S_IXOTH)
		|S_IRGRP|S_IWGRP|S_IXGRP
		|S_IROTH|S_IWOTH|S_IXOTH
	#endif
	;
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
	return S_IRUSR|S_IWUSR;
}

mode_t permissions::ownerReadExecute() {
	return S_IRUSR|S_IXUSR;
}

mode_t permissions::ownerReadWriteExecute() {
	return S_IRUSR|S_IWUSR|S_IXUSR;
}

mode_t permissions::groupRead() {
	#ifdef S_IRGRP
		return S_IRGRP;
	#else
		return 0;
	#endif
}

mode_t permissions::groupWrite() {
	#ifdef S_IWGRP
		return S_IWGRP;
	#else
		return 0;
	#endif
}

mode_t permissions::groupExecute() {
	#ifdef S_IXGRP
		return S_IXGRP;
	#else
		return 0;
	#endif
}

mode_t permissions::groupReadWrite() {
	#if defined(S_IRGRP) && defined(S_IWGRP)
		return S_IRGRP|S_IWGRP;
	#else
		return 0;
	#endif
}

mode_t permissions::groupReadExecute() {
	#if defined(S_IRGRP) && defined(S_IXGRP)
		return S_IRGRP|S_IXGRP;
	#else
		return 0;
	#endif
}

mode_t permissions::groupReadWriteExecute() {
	#if defined(S_IRGRP) && defined(S_IWGRP) && defined(S_IXGRP)
		return S_IRGRP|S_IWGRP|S_IXGRP;
	#else
		return 0;
	#endif
}

mode_t permissions::othersRead() {
	#if defined(S_IROTH)
		return S_IROTH;
	#else
		return 0;
	#endif
}

mode_t permissions::othersWrite() {
	#if defined(S_IWOTH)
		return S_IWOTH;
	#else
		return 0;
	#endif
}

mode_t permissions::othersExecute() {
	#if defined(S_IXOTH)
		return S_IXOTH;
	#else
		return 0;
	#endif
}

mode_t permissions::othersReadWrite() {
	#if defined(S_IROTH) && defined(S_IWOTH)
		return S_IROTH|S_IWOTH;
	#else
		return 0;
	#endif
}

mode_t permissions::othersReadExecute() {
	#if defined(S_IROTH) && defined(S_IXOTH)
		return S_IROTH|S_IXOTH;
	#else
		return 0;
	#endif
}

mode_t permissions::othersReadWriteExecute() {
	#if defined(S_IROTH) && defined(S_IWOTH) && defined(S_IXOTH)
		return S_IROTH|S_IWOTH|S_IXOTH;
	#else
		return 0;
	#endif
}

mode_t permissions::saveInSwapSpace() {
	#ifdef S_ISVTX
		return S_ISVTX;
	#else
		return 0;
	#endif
}

mode_t permissions::setUserId() {
	#ifdef S_ISUID
		return S_ISUID;
	#else
		return 0;
	#endif
}

mode_t permissions::setGroupId() {
	#ifdef S_ISGID
		return S_ISGID;
	#else
		return 0;
	#endif
}

mode_t permissions::evalPermString(const char *permstring) {
	mode_t	retval=0;
	if (charstring::length(permstring)==9) {

		// handle user permissions
		if (permstring[0]=='r') {
			retval=retval|ownerRead();
		}
		if (permstring[1]=='w') {
			retval=retval|ownerWrite();
		}
		if (permstring[2]=='x') {
			retval=retval|ownerExecute();
		} else if (permstring[2]=='X' || permstring[2]=='S') {
			retval=retval|ownerExecute();
			retval=retval|setUserId();
		}

		// handle group permissions
		if (permstring[3]=='r') {
			retval=retval|groupRead();
		}
		if (permstring[4]=='w') {
			retval=retval|groupWrite();
		}
		if (permstring[5]=='x') {
			retval=retval|groupExecute();
		} else if (permstring[5]=='X' || permstring[5]=='S') {
			retval=retval|groupExecute();
		}

		// handle others permissions
		if (permstring[6]=='r') {
			retval=retval|othersRead();
		}
		if (permstring[7]=='w') {
			retval=retval|othersWrite();
		}
		if (permstring[8]=='x') {
			retval=retval|othersExecute();

		// handle sticky bit
		} else if (permstring[5]=='t') {
			retval=retval|saveInSwapSpace();
		}
	}
	return retval;
}

char *permissions::evalPermOctal(mode_t permoctal) {

	char	*permstring=new char[10];
	permstring[9]='\0';

	mode_t	shift=permoctal;
	for (uint8_t i=8; i>=0; i--) {
		uint8_t	pos=i%3;
		permstring[i]=(shift&1)?((pos==2)?'x':(pos==1)?'w':'r'):'-';
		shift=shift>>1;
	}
	return permstring;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
