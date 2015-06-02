// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/permissions.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>
#include <rudiments/file.h>
#include <rudiments/userentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/stdio.h>
#include <rudiments/process.h>
#ifdef RUDIMENTS_HAVE_SETENTRIESINACL
	#include <rudiments/bytestring.h>
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_STAT_H
	#include <sys/stat.h>
#endif
#ifdef RUDIMENTS_HAVE_IO_H
	#include <io.h>
#endif
#ifdef RUDIMENTS_HAVE_SDDL_H
	#include <sddl.h>
#endif
#ifdef RUDIMENTS_HAVE_ACLAPI_H
	#include <aclapi.h>
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

#ifdef RUDIMENTS_HAVE_GETACE

	// Object-specific rights
	// see http://msdn.microsoft.com/en-us/magazine/cc982153.aspx
	#define _CC	0x00000001	// "read"
	#define _DC	0x00000002	// "write"
	#define _LC	0x00000004	// "append"
	#define _SW	0x00000008	// "read extended attributes"
	#define _RP	0x00000010	// "write extended attributes"
	#define _WP	0x00000020	// "execute"
	#define _DT	0x00000040	// "delete child"
	#define _LO	0x00000080	// "read standard attributes"
	#define _CR	0x00000100	// "write standard attributes"

	// normalized rights masks
	#define	_ALL	GENERIC_ALL
	#define _READ	(READ_CONTROL|GENERIC_READ|_CC|_SW|_LO)
	#define _WRITE	(DELETE|WRITE_DAC|WRITE_OWNER| \
				GENERIC_WRITE|_DC|_LC|_RP|_DT|_CR| \
				SYNCHRONIZE)
	#define _EXEC	(GENERIC_EXECUTE|_WP)
#endif


bool permissions::setFilePermissions(const char *filename, mode_t perms) {
	#if defined(RUDIMENTS_HAVE_CHMOD)
		return !chmod(filename,perms);
	#elif defined(RUDIMENTS_HAVE_SETSECURITYINFO)

		// is this a file or directory?
		DWORD	fileattr=GetFileAttributes(filename);
		if (fileattr==INVALID_FILE_ATTRIBUTES) {
			return false;
		}

		// determine the attrs
		// FILE_FLAG_BACKUP_SEMANTICS must be used when opening a
		// directory, for some reason
		bool	isdir=false;
		DWORD	attrs=FILE_ATTRIBUTE_NORMAL;
		if (fileattr&FILE_ATTRIBUTE_DIRECTORY) {
			isdir=true;
			attrs=FILE_FLAG_BACKUP_SEMANTICS;
		}

		// convert the perms to a dacl
		PACL	dacl=(PACL)permOctalToDacl(perms,isdir);

		// open the file/directory
		HANDLE	fh=CreateFile(filename,
					GENERIC_WRITE|WRITE_DAC,
					FILE_SHARE_DELETE|
					FILE_SHARE_READ|
					FILE_SHARE_WRITE,
					NULL,OPEN_EXISTING,
					attrs,NULL);
		if (fh==INVALID_HANDLE_VALUE) {
			return false;
		}

		// set the permissions
		bool	success=(SetSecurityInfo(fh,
						SE_FILE_OBJECT,
						DACL_SECURITY_INFORMATION,
						NULL,NULL,dacl,NULL)==
						ERROR_SUCCESS);

		// close the file/directory
		CloseHandle(fh);

		// clean up
		LocalFree(dacl);

		return success;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool permissions::setFilePermissions(int32_t fd, mode_t perms) {
	#if defined(RUDIMENTS_HAVE_FCHMOD)
		int32_t	result;
		error::clearError();
		do {
			result=fchmod(fd,perms);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_SETSECURITYINFO)

		// get the file handle
		HANDLE	fh=(HANDLE)filedescriptor::
				getHandleFromFileDescriptor(fd);

		// is this a file or directory?
		BY_HANDLE_FILE_INFORMATION	bhfi;
		if (!GetFileInformationByHandle(fh,&bhfi)) {
			return false;
		}

		// determine the attrs
		bool	isdir=false;
		if (bhfi.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) {
			isdir=true;
		}

		// convert the perms to a dacl
		PACL	dacl=(PACL)permOctalToDacl(perms,isdir);

		// set the permissions
		bool	success=(SetSecurityInfo(fh,
						SE_FILE_OBJECT,
						DACL_SECURITY_INFORMATION,
						NULL,NULL,dacl,NULL)==
						ERROR_SUCCESS);

		// clean up
		LocalFree(dacl);

		return success;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
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

void *permissions::permStringToDacl(const char *permstring, bool directory) {
	return permOctalToDacl(evalPermString(permstring),directory);
}

void *permissions::permOctalToDacl(mode_t permoctal, bool directory) {

	#ifdef RUDIMENTS_HAVE_SETENTRIESINACL

		#if _WIN32_WINNT>=0x0500

		// FIXME: how?
		// P - protected: ignore perms from higher
		//			up the inheritance tree
		// AI - children inherit permissions (for directories only)
		/*charstring::copy(sddl,"D:P");
		if (directory) {
			charstring::append(sddl,"AI");
		}*/

		// define inheritance...
		DWORD	inheritance=NO_INHERITANCE;
		if (directory) {
			inheritance=OBJECT_INHERIT_ACE|CONTAINER_INHERIT_ACE;
		}

		// create acl entries for world, group and owner
		EXPLICIT_ACCESS	ea[3];
		bytestring::zero(&ea,sizeof(ea));

		// world
		ea[0].grfAccessMode=SET_ACCESS;
		ea[0].grfInheritance=inheritance;
		ea[0].Trustee.MultipleTrusteeOperation=NO_MULTIPLE_TRUSTEE;
		ea[0].Trustee.TrusteeForm=TRUSTEE_IS_SID;
		ea[0].Trustee.TrusteeType=TRUSTEE_IS_WELL_KNOWN_GROUP;
		SID_IDENTIFIER_AUTHORITY	worldsia=
						SECURITY_WORLD_SID_AUTHORITY;
		PSID				worldsid;
		AllocateAndInitializeSid(&worldsia,1,SECURITY_WORLD_RID,
						0,0,0,0,0,0,0,&worldsid);
		ea[0].Trustee.ptstrName=(LPSTR)worldsid;

		// group
		ea[1].grfAccessMode=SET_ACCESS;
		ea[1].grfInheritance=inheritance;
		ea[1].Trustee.MultipleTrusteeOperation=NO_MULTIPLE_TRUSTEE;
		ea[1].Trustee.TrusteeForm=TRUSTEE_IS_SID;
		ea[1].Trustee.TrusteeType=TRUSTEE_IS_GROUP;
		groupentry	grpent;
		grpent.initialize(process::getRealGroupId());
		PSID		groupsid=NULL;
		ConvertStringSidToSid(grpent.getSid(),&groupsid);
		ea[1].Trustee.ptstrName=(LPSTR)groupsid;

		// owner
		ea[2].grfAccessMode=SET_ACCESS;
		ea[2].grfInheritance=inheritance;
		ea[2].Trustee.MultipleTrusteeOperation=NO_MULTIPLE_TRUSTEE;
		ea[2].Trustee.TrusteeForm=TRUSTEE_IS_SID;
		ea[2].Trustee.TrusteeType=TRUSTEE_IS_USER;
		userentry	usrent;
		usrent.initialize(process::getRealUserId());
		PSID		ownersid=NULL;
		ConvertStringSidToSid(usrent.getSid(),&ownersid);
		ea[2].Trustee.ptstrName=(LPSTR)ownersid;

		// set access permissions
		DWORD	perms=0;
		mode_t	shift=permoctal;
		for (int16_t i=0; i<9; i++) {
			if (shift&1) {
				uint8_t	pos=i%3;
				if (pos==0) {
					// execute permissions
					perms|=_EXEC;
				} else if (pos==1) {
					// write permissions
					perms|=_WRITE;
				} else {
					// read permissions
					perms|=_READ;
				}
			}
			shift=shift>>1;
			if (i==2) {
				// set world permissions
				ea[0].grfAccessPermissions=perms;
				perms=0;
			} else if (i==5) {
				// set group permissions
				ea[1].grfAccessPermissions=perms;
				perms=0;
			} else if (i==8) {
				// set user permissions
				ea[2].grfAccessPermissions=perms;
				perms=0;
			}
		}

		// set entries in acl
		PACL	pacl=NULL;
		if (SetEntriesInAcl(3,ea,NULL,&pacl)!=ERROR_SUCCESS) {
			LocalFree(pacl);
			pacl=NULL;
		}

		// clean up
		LocalFree(groupsid);
		LocalFree(ownersid);

		// return the acl
		return (void *)pacl;

		#else

		// FIXME: implement for WinNT
		return NULL;

		#endif
	#else
		return NULL;
	#endif
}

char *permissions::daclToPermString(void *dacl) {
	return evalPermOctal(daclToPermOctal(dacl));
}

mode_t permissions::daclToPermOctal(void *dacl) {

	// init the return value
	mode_t	perms=0;

	#ifdef RUDIMENTS_HAVE_GETACE

		#if _WIN32_WINNT>=0x0500

		// get the user and convert to an sid
		userentry	usrent;
		PSID		usersid=NULL;
		if (!usrent.initialize(process::getRealUserId()) ||
			ConvertStringSidToSid(usrent.getSid(),
						&usersid)!=TRUE) {
			return perms;
		}

		// get the group and convert to an sid
		groupentry	grpent;
		PSID		groupsid=NULL;
		if (!grpent.initialize(process::getRealGroupId()) ||
			ConvertStringSidToSid(grpent.getSid(),
						&groupsid)!=TRUE) {
			return perms;
		}

		// get the sid for others
		// ("S-1-1-0" is the well known SID for "World")
		PSID	otherssid=NULL;
		if (ConvertStringSidToSid("S-1-1-0",&otherssid)!=TRUE) {
			return perms;
		}

		// cast the DACL properly
		PACL	d=(PACL)dacl;

		// run through the ACEs of the DACL
		for (DWORD i=0; i<d->AceCount; i++) {

			// get the ACE
			PVOID	ace=NULL;
			if (GetAce(d,i,&ace)==FALSE) {
				continue;
			}

			// get various ACE components
			ACCESS_ALLOWED_ACE	*aace=(ACCESS_ALLOWED_ACE *)ace;
			PSID			sid=(PSID)&aace->SidStart;
			DWORD			mask=aace->Mask;

			// which sid does this ACE apply to
			if (EqualSid(sid,usersid)) {

				if (aace->Header.AceType==
						ACCESS_ALLOWED_ACE_TYPE) {

					// update perms
					if (mask&_ALL) {
						perms|=ownerRead();
						perms|=ownerWrite();
						perms|=ownerExecute();
					}
					if (mask&_READ) {
						perms|=ownerRead();
					}
					if (mask&_WRITE) {
						perms|=ownerWrite();
					}
					if (mask&_EXEC) {
						perms|=ownerExecute();
					}

				} else if (aace->Header.AceType==
						ACCESS_DENIED_ACE_TYPE) {

					// update perms
					if (mask&_ALL) {
						perms&=~ownerRead();
						perms&=~ownerWrite();
						perms&=~ownerExecute();
					}
					if (mask&_READ) {
						perms&=~ownerRead();
					}
					if (mask&_WRITE) {
						perms&=~ownerWrite();
					}
					if (mask&_EXEC) {
						perms&=~ownerExecute();
					}
				}

			} else if (EqualSid(sid,groupsid)) {

				if (aace->Header.AceType==
						ACCESS_ALLOWED_ACE_TYPE) {

					// update perms
					if (mask&_ALL) {
						perms|=groupRead();
						perms|=groupWrite();
						perms|=groupExecute();
					}
					if (mask&_READ) {
						perms|=groupRead();
					}
					if (mask&_WRITE) {
						perms|=groupWrite();
					}
					if (mask&_EXEC) {
						perms|=groupExecute();
					}

				} else if (aace->Header.AceType==
						ACCESS_DENIED_ACE_TYPE) {

					// update perms
					if (mask&_ALL) {
						perms&=~groupRead();
						perms&=~groupWrite();
						perms&=~groupExecute();
					}
					if (mask&_READ) {
						perms&=~groupRead();
					}
					if (mask&_WRITE) {
						perms&=~groupWrite();
					}
					if (mask&_EXEC) {
						perms&=~groupExecute();
					}
				}

			} else if (EqualSid(sid,otherssid)) {

				if (aace->Header.AceType==
						ACCESS_ALLOWED_ACE_TYPE) {

					// update perms
					if (mask&_ALL) {
						perms|=othersRead();
						perms|=othersWrite();
						perms|=othersExecute();
					}
					if (mask&_READ) {
						perms|=othersRead();
					}
					if (mask&_WRITE) {
						perms|=othersWrite();
					}
					if (mask&_EXEC) {
						perms|=othersExecute();
					}

				} else if (aace->Header.AceType==
						ACCESS_DENIED_ACE_TYPE) {

					// update perms
					if (mask&_ALL) {
						perms&=~othersRead();
						perms&=~othersWrite();
						perms&=~othersExecute();
					}
					if (mask&_READ) {
						perms&=~othersRead();
					}
					if (mask&_WRITE) {
						perms&=~othersWrite();
					}
					if (mask&_EXEC) {
						perms&=~othersExecute();
					}
				}
			}
		}

		// clean up
		LocalFree(usersid);
		LocalFree(groupsid);
		LocalFree(otherssid);

		#else

		// FIXME: implement for WinNT

		#endif
	#endif

	// return permissions
	return perms;
}
