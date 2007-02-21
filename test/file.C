// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/groupentry.h>
#include <rudiments/passwdentry.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/datetime.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {


	// remove the file (in case it already exists)
	file::remove("testfile");


	// create a new file called "testfile" with rw-rw---- permissions
	// and initial contents "hello"
	file	fl;
	fl.create("testfile",permissions::evalPermString("rw-rw----"),"hello");

	printf("testfile:\n");
	
	// check for existence
	if (file::exists("testfile")) {
		printf("	exists\n");
	} else {
		printf("	does not exist\n");
	}

	// display the permissions of the file
	mode_t	mode=fl.getPermissions();
	printf("	permissions: %s\n",permissions::evalPermOctal(mode));


	// display the name of the user that owns the file
	uid_t	uid=fl.getOwnerUserId();
	char	*username;
	passwdentry::getName(uid,&username);
	printf("	user       : %s\n",username);
	delete[] username;


	// display the name of the group that owns the file
	gid_t	gid=fl.getOwnerGroupId();
	char	*groupname;
	groupentry::getName(gid,&groupname);
	printf("	group      : %s\n",groupname);
	delete[] groupname;


	// display the size of the file in bytes
	off64_t	size=fl.getSize();
	printf("	size       : %ld\n",size);


	// display the size of the file in blocks
	blkcnt_t	blocks=fl.getBlockCount();
	printf("	blocks     : %ld\n",blocks);


	// display the file type
	printf("	is a socket: %d\n",fl.isSocket());
	printf("	is a symlink: %d\n",fl.isSymbolicLink());
	printf("	is a regular file: %d\n",fl.isRegularFile());
	printf("	is a block device: %d\n",fl.isBlockDevice());
	printf("	is a directory: %d\n",fl.isDirectory());
	printf("	is a character device: %d\n",fl.isCharacterDevice());
	printf("	is a fifo: %d\n",fl.isFifo());


	// display the last time the file was accessed
	time_t	atime=fl.getLastAccessTime();
	char	*atimestr=datetime::getString(atime);
	printf("	last access      : %s\n",atimestr);
	delete[] atimestr;


	// display the last time the file was modified
	time_t	mtime=fl.getLastModificationTime();
	char	*mtimestr=datetime::getString(mtime);
	printf("	last modification: %s\n",mtimestr);
	delete[] mtimestr;


	// display the last time the file was changed
	time_t	ctime=fl.getLastChangeTime();
	char	*ctimestr=datetime::getString(ctime);
	printf("	last change      : %s\n",ctimestr);
	delete[] ctimestr;


	// display the device that the file resides on
	dev_t	dev=fl.getDevice();
	printf("	device           : %d\n",dev);


	// display the type of the device that the file resides on
	dev_t	devtype=fl.getDeviceType();
	printf("	device type      : %d\n",devtype);


	// display the file's first inode
	ino_t	inode=fl.getInode();
	printf("	inode            : %d\n",inode);


	// display the number of hard links to the file
	nlink_t	nlink=fl.getNumberOfHardLinks();
	printf("	hard links : %ld\n",nlink);


	char	*path="/usr/local/firstworks/include/rudiments/file.h";
	char	*dirname=file::dirname(path);
	printf("dirname(%s)=%s\n",path,dirname);
	delete[] dirname;

	char	*basename=file::basename(path);
	printf("basename(%s)=%s\n",path,basename);
	delete[] basename;

	basename=file::basename(path,".h");
	printf("basename(%s,\".h\")=%s\n",path,basename);
	delete[] basename;

	printf("key=%d\n",file::generateKey("/",1));

	printf("maxLinks(%s)=%d\n",path,file::maxLinks(path));

	printf("canChangeOwner(%s)=%d\n",path,file::canChangeOwner(path));
}
