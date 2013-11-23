// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/permissions.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>
#include <rudiments/file.h>

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_STAT_H
	#include <sys/stat.h>
#endif
#ifdef RUDIMENTS_HAVE_IO_H
	#include <io.h>
#endif


// Some platforms don't define some of these because they don't support
// them.  However, every platform I've tried so far interprets the permissions
// bitmap the same way, and ignores unsupported bits.  Lets hope that's true
// in general.
#ifndef S_ISUID
	#define	S_ISUID 04000
#endif
#ifndef S_ISGID
	#define	S_ISGID 02000
#endif
#ifndef S_ISVTX
	#define	S_ISVTX 01000
#endif
#ifndef S_IRUSR
	#define	S_IRUSR	0400
#endif
#ifndef S_IWUSR
	#define	S_IWUSR	0200
#endif
#ifndef S_IXUSR
	#define	S_IXUSR	0100
#endif
#ifndef S_IRGRP
	#define	S_IRGRP	(S_IRUSR>>3)
#endif
#ifndef S_IWGRP
	#define	S_IWGRP	(S_IWUSR>>3)
#endif
#ifndef S_IXGRP
	#define	S_IXGRP	(S_IXUSR>>3)
#endif
#ifndef S_IROTH
	#define	S_IROTH	(S_IRGRP>>3)
#endif
#ifndef S_IWOTH
	#define	S_IWOTH	(S_IWGRP>>3)
#endif
#ifndef S_IXOTH
	#define	S_IXOTH	(S_IXGRP>>3)
#endif


bool permissions::setFilePermissions(const char *filename, mode_t perms) {
	#ifdef _WIN32
		// windows doesn't support anything like this
		error::setErrorNumber(ENOSYS);
		return false;
		//return !_chmod(filename,perms);
	#else
		file	fl;
		return (fl.open(filename,O_RDWR) &&
			setFilePermissions(fl.getFileDescriptor(),perms));
	#endif
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
	return S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH;
}

mode_t permissions::everyoneReadWriteExecute() {
	return S_IRUSR|S_IWUSR|S_IXUSR
			|S_IRGRP|S_IWGRP|S_IXGRP
			|S_IROTH|S_IWOTH|S_IXOTH;
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
	return S_IRGRP;
}

mode_t permissions::groupWrite() {
	return S_IWGRP;
}

mode_t permissions::groupExecute() {
	return S_IXGRP;
}

mode_t permissions::groupReadWrite() {
	return S_IRGRP|S_IWGRP;
}

mode_t permissions::groupReadExecute() {
	return S_IRGRP|S_IXGRP;
}

mode_t permissions::groupReadWriteExecute() {
	return S_IRGRP|S_IWGRP|S_IXGRP;
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
	return S_IROTH|S_IWOTH;
}

mode_t permissions::othersReadExecute() {
	return S_IROTH|S_IXOTH;
}

mode_t permissions::othersReadWriteExecute() {
	return S_IROTH|S_IWOTH|S_IXOTH;
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
	for (int16_t i=8; i>=0; i--) {
		uint8_t	pos=i%3;
		permstring[i]=(shift&1)?((pos==2)?'x':(pos==1)?'w':'r'):'-';
		shift=shift>>1;
	}
	return permstring;
}

char *permissions::permStringToSDDL(const char *permstring, bool directory) {
	return permOctalToSDDL(evalPermString(permstring),directory);
}

char *permissions::permOctalToSDDL(mode_t permoctal, bool directory) {

	// see http://msdn.microsoft.com/en-us/magazine/cc982153.aspx
	//
	// sddl format:
	// D:P,PAI(ace)(ace)...(ace)
	//
	// S - SACL (System ACL)
	// D - DACL (Discretionary ACL)
	//
	// P - protected: ignore perms from higher
	//			up the inheritance tree
	// AI - children inherit permissions (for directories only)
	//
	// access control entities (1 per account_sid):
	// (ace) - (ace_type;ace_flags;rights;object_guid; \
	//			inherit_object_guid;account_sid)
	//	ace_types:
	//		A: Allowed
	//		...
	//	ace_flags:
	//		CI: subdirectories (containers) will inherit this ace
	//		OI: files (objects) will inherit this ace
	//		...
	//	generic rights:
	//		SD - delete
	//		RC - read security descriptor
	//		GR - generic read
	//		GW - generic write
	//		GX - generic execute
	//		WD - write dacl
	//		WO - write ownership
	//		...
	//	object_guid:
	//		(usually not specified)
	//	inherit_object_guid:
	//		(usually not specified)
	//	account_sid:
	//		WD - world
	//		(otherwise need the actual sid)
	//
	// 777 - rwxrwxrwx -
	// D:PAI(A;OICI;GRGWGX;;;WD)(A;OICI;GRGWGX;;;gsid)(A;OICI;GRGWGX;;;usid)

	// ACE's are interpreted left to right.
	// They should be specified world, then group, then user because:
	//	 world perms are applied to the group and user
	//	 group perms are applied to the user


	//char	*permstring=new char[66];
	char	*permstring=new char[512];
	charstring::copy(permstring,"D:P");
	if (directory) {
		charstring::append(permstring,"AI");
	}

	mode_t	shift=permoctal;
	for (int16_t i=0; i<9; i++) {
		if (i==0 || i==3 || i==6) {
			charstring::append(permstring,"(A;");
			if (directory) {
				charstring::append(permstring,"OICI");
			}
			charstring::append(permstring,";");
		}
		uint8_t	pos=i%3;
		charstring::append(permstring,
			(shift&1)?((pos==0)?
				"GX":(pos==1)?"GWSDWDWO":"GRRC"):"");
		shift=shift>>1;
		if (i==2) {
			charstring::append(permstring,";;;WD)");
		} else if (i==5) {
			// FIXME: get the current user's sid
			// GetUserName
			// LookupAccountName
			// ConvertSidToStringSid
			charstring::append(permstring,";;;S-1-5-21-1873234618-1269098444-2064074030-513)");
		} else if (i==8) {
			// FIXME: get the current group's sid
			charstring::append(permstring,";;;S-1-5-21-1873234618-1269098444-2064074030-1001)");
		}
	}
	return permstring;
}
