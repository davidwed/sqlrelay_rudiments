// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/groupentry.h>
#include <rudiments/userentry.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>
#include <rudiments/process.h>

int main(int argc, const char **argv) {

	// remove the file (in case it already exists)
	file::remove("testfile");


	// create a new file called "testfile" with rw-rw---- permissions
	// and initial contents "hello"
	file	fl;
	fl.create("testfile",permissions::evalPermString("rw-rw----"));
	fl.write("hello");
	fl.getCurrentProperties();

	stdoutput.printf("testfile:\n");
	
	// check for existence
	if (file::exists("testfile")) {
		stdoutput.printf("	exists\n");
	} else {
		stdoutput.printf("	does not exist\n");
	}

	// display the permissions of the file
	mode_t	mode=fl.getPermissions();
	stdoutput.printf("	permissions: %s\n",
				permissions::evalPermOctal(mode));

	// change permissions of the file
	stdoutput.printf("	change perms: %d\n",
		permissions::setFilePermissions(fl.getFileDescriptor(),
				permissions::evalPermString("rw-------")));
	fl.getCurrentProperties();
	mode=fl.getPermissions();
	stdoutput.printf("	new permissions: %s\n",
				permissions::evalPermOctal(mode));

	// close, reopen and change perms back
	fl.close();
	if (!fl.open("testfile",O_RDWR)) {
		file::remove("testfile");
		stdoutput.printf("open failed\n");
		process::exit(1);
	}
	stdoutput.printf("	change perms: %d\n",
		permissions::setFilePermissions(fl.getFileDescriptor(),
				permissions::evalPermString("rw-rw----")));
	fl.getCurrentProperties();
	mode=fl.getPermissions();
	stdoutput.printf("	new permissions: %s\n",
				permissions::evalPermOctal(mode));

	// display the name of the user that owns the file
	uid_t	uid=fl.getOwnerUserId();
	char	*username=userentry::getName(uid);
	stdoutput.printf("	user       : %s\n",username);
	delete[] username;


	// display the name of the group that owns the file
	gid_t	gid=fl.getOwnerGroupId();
	char	*groupname=groupentry::getName(gid);
	stdoutput.printf("	group      : %s\n",groupname);
	delete[] groupname;


	// display the size of the file in bytes
	off64_t	size=fl.getSize();
	stdoutput.printf("	size       : %lld\n",size);


	// display the block size of the file
	blkcnt_t	blocksize=fl.getBlockSize();
	stdoutput.printf("	block size : %ld\n",blocksize);

	// display the size of the file in blocks
	blkcnt_t	blocks=fl.getBlockCount();
	stdoutput.printf("	blocks     : %ld\n",blocks);


	// display the file type
	stdoutput.printf("	is a socket: %d\n",fl.isSocket());
	stdoutput.printf("	is a symlink: %d\n",fl.isSymbolicLink());
	stdoutput.printf("	is a regular file: %d\n",fl.isRegularFile());
	stdoutput.printf("	is a block device: %d\n",fl.isBlockDevice());
	stdoutput.printf("	is a directory: %d\n",fl.isDirectory());
	stdoutput.printf("	is a character device: %d\n",
						fl.isCharacterDevice());
	stdoutput.printf("	is a fifo: %d\n",fl.isFifo());


	// display the last time the file was accessed
	time_t	atime=fl.getLastAccessTime();
	char	*atimestr=datetime::getString(atime);
	stdoutput.printf("	last access      : %s\n",atimestr);
	delete[] atimestr;


	// display the last time the file was modified
	time_t	mtime=fl.getLastModificationTime();
	char	*mtimestr=datetime::getString(mtime);
	stdoutput.printf("	last modification: %s\n",mtimestr);
	delete[] mtimestr;


	// display the last time the file was changed
	time_t	ctime=fl.getLastChangeTime();
	char	*ctimestr=datetime::getString(ctime);
	stdoutput.printf("	last change      : %s\n",ctimestr);
	delete[] ctimestr;


	// display the device that the file resides on
	dev_t	dev=fl.getDevice();
	stdoutput.printf("	device           : %lld\n",dev);


	// display the type of the device that the file resides on
	dev_t	devtype=fl.getDeviceType();
	stdoutput.printf("	device type      : %lld\n",devtype);


	// display the file's first inode
	ino_t	inode=fl.getInode();
	stdoutput.printf("	inode            : %ld\n",inode);


	// display the number of hard links to the file
	nlink_t	nlink=fl.getNumberOfHardLinks();
	stdoutput.printf("	hard links : %d\n",nlink);


	const char	*path="/usr/local/firstworks/include/rudiments/file.h";
	char	*dirname=file::dirname(path);
	stdoutput.printf("dirname(%s)=%s\n",path,dirname);
	delete[] dirname;

	char	*basename=file::basename(path);
	stdoutput.printf("basename(%s)=%s\n",path,basename);
	delete[] basename;

	basename=file::basename(path,".h");
	stdoutput.printf("basename(%s,\".h\")=%s\n",path,basename);
	delete[] basename;

	stdoutput.printf("key=%d\n",file::generateKey("testfile",1));

	stdoutput.printf("maxLinks(%s)=%ld\n",path,file::maxLinks(path));

	stdoutput.printf("canChangeOwner(%s)=%d\n",
					path,file::canChangeOwner(path));

	// remove the file
	file::remove("testfile");
}
