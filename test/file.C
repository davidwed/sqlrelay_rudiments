#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/datetime.h>
#include <unistd.h>
#include <stdio.h>

main() {

	unlink("testfile");

	file	fl;
	fl.create("testfile",permissions::evalPermString("rw-rw----"),"hello");

	printf("testfile:\n");

	mode_t	mode;
	fl.getPermissions(&mode);
	printf("	permissions: %s\n",permissions::evalPermOctal(mode));

	uid_t	uid;
	fl.getOwnerUserId(&uid);
	char	*username=passwdentry::getName(uid);
	printf("	user       : %s\n",username);
	delete[] username;

	gid_t	gid;
	fl.getOwnerGroupId(&gid);
	char	*groupname=groupentry::getName(gid);
	printf("	group      : %s\n",groupname);
	delete[] groupname;

	off_t	size;
	fl.getSize(&size);
	printf("	size       : %d\n",size);

	blkcnt_t	blocks;
	fl.getBlockCount(&blocks);
	printf("	blocks     : %d\n",blocks);

	printf("	is a socket: %d\n",fl.isSocket());
	printf("	is a symlink: %d\n",fl.isSymbolicLink());
	printf("	is a regular file: %d\n",fl.isRegularFile());
	printf("	is a block device: %d\n",fl.isBlockDevice());
	printf("	is a directory: %d\n",fl.isDirectory());
	printf("	is a character device: %d\n",fl.isCharacterDevice());
	printf("	is a fifo: %d\n",fl.isFifo());

	time_t	atime;
	fl.getLastAccessTime(&atime);
	char	*atimestr=datetime::getString(atime);
	printf("	last access      : %s\n",atimestr);
	delete[] atimestr;

	time_t	mtime;
	fl.getLastModificationTime(&mtime);
	char	*mtimestr=datetime::getString(mtime);
	printf("	last modification: %s\n",mtimestr);
	delete[] mtimestr;

	time_t	ctime;
	fl.getLastChangeTime(&ctime);
	char	*ctimestr=datetime::getString(ctime);
	printf("	last change      : %s\n",ctimestr);
	delete[] ctimestr;

	dev_t	dev;
	fl.getDevice(&dev);
	printf("	device           : %d\n",dev);

	dev_t	devtype;
	fl.getDeviceType(&devtype);
	printf("	device type      : %d\n",devtype);

	ino_t	inode;
	fl.getInode(&inode);
	printf("	inode            : %d\n",inode);

	nlink_t	nlink;
	fl.getNumberOfHardLinks(&nlink);
	printf("	hard links : %d\n",nlink);
}
