#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/datetime.h>
#include <unistd.h>
#include <stdio.h>

int main(int argv, const char **argc) {

	// remove the file (in case it already exists)
	unlink("testfile");

	// create a new file called "testfile" with rw-rw---- permissions
	// and initial contents "hello"
	file	fl;
	fl.create("testfile",permissions::evalPermString("rw-rw----"),"hello");

	printf("testfile:\n");

	// display the permissions of the file
	mode_t	mode=fl.getPermissions();
	printf("	permissions: %s\n",permissions::evalPermOctal(mode));

	// get the 
	uid_t	uid=fl.getOwnerUserId();
	char	*username;
	passwdentry::getName(uid,&username);
	printf("	user       : %s\n",username);
	delete[] username;

	gid_t	gid=fl.getOwnerGroupId();
	char	*groupname;
	groupentry::getName(gid,&groupname);
	printf("	group      : %s\n",groupname);
	delete[] groupname;

	off_t	size=fl.getSize();
	printf("	size       : %d\n",size);

	blkcnt_t	blocks=fl.getBlockCount();
	printf("	blocks     : %d\n",blocks);

	printf("	is a socket: %d\n",fl.isSocket());
	printf("	is a symlink: %d\n",fl.isSymbolicLink());
	printf("	is a regular file: %d\n",fl.isRegularFile());
	printf("	is a block device: %d\n",fl.isBlockDevice());
	printf("	is a directory: %d\n",fl.isDirectory());
	printf("	is a character device: %d\n",fl.isCharacterDevice());
	printf("	is a fifo: %d\n",fl.isFifo());

	time_t	atime=fl.getLastAccessTime();
	char	*atimestr=datetime::getString(atime);
	printf("	last access      : %s\n",atimestr);
	delete[] atimestr;

	time_t	mtime=fl.getLastModificationTime();
	char	*mtimestr=datetime::getString(mtime);
	printf("	last modification: %s\n",mtimestr);
	delete[] mtimestr;

	time_t	ctime=fl.getLastChangeTime();
	char	*ctimestr=datetime::getString(ctime);
	printf("	last change      : %s\n",ctimestr);
	delete[] ctimestr;

	dev_t	dev=fl.getDevice();
	printf("	device           : %d\n",dev);

	dev_t	devtype=fl.getDeviceType();
	printf("	device type      : %d\n",devtype);

	ino_t	inode=fl.getInode();
	printf("	inode            : %d\n",inode);

	nlink_t	nlink=fl.getNumberOfHardLinks();
	printf("	hard links : %d\n",nlink);
}
