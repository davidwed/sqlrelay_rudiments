// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/groupentry.h>
#include <rudiments/userentry.h>
#include <rudiments/file.h>
#include <rudiments/filesystem.h>
#include <rudiments/permissions.h>
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>
#include <rudiments/process.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("file");

	file::remove("testfile.txt");


	file	fl;
	test("create",fl.create("testfile.txt",
			permissions::evalPermString("rw-rw----")));
	test("write",fl.write("hello")==5);
	test("get properties",fl.getCurrentProperties());
	test("close",fl.close());
	test("exists",file::exists("testfile.txt"));
	test("reopen",fl.open("testfile.txt",O_RDWR));

	uid_t	uid=fl.getOwnerUserId();
	char	*username=userentry::getName(uid);
	test("user",uid==process::getUserId());

	gid_t	gid=fl.getOwnerGroupId();
	char	*groupname=groupentry::getName(gid);
	test("group",gid==process::getGroupId());

	test("chown",fl.changeOwner(username,groupname));
	delete[] username;
	delete[] groupname;

	test("size",fl.getSize()==5);

	filesystem	fs;
	fs.initialize(fl.getFileDescriptor());
	test("block size",fl.getBlockSize()==fs.getBlockSize());
	test("blocks",fl.getBlockCount()!=0);

	test("is a socket",!fl.isSocket());
	test("is a symlink",!fl.isSymbolicLink());
	test("is a regular file",fl.isRegularFile());
	test("is a block device",!fl.isBlockDevice());
	test("is a directory",!fl.isDirectory());
	test("is a character device",!fl.isCharacterDevice());
	test("is a fifo",!fl.isFifo());

	datetime	dt;
	dt.getSystemDateAndTime();

	datetime	atime;
	atime.initialize(fl.getLastAccessTime());
	test("access time",atime.getEpoch()-dt.getEpoch()<=1);

	datetime	mtime;
	mtime.initialize(fl.getLastModificationTime());
	test("modification time",mtime.getEpoch()-dt.getEpoch()<=1);

	datetime	ctime;
	mtime.initialize(fl.getLastChangeTime());
	test("change time",ctime.getEpoch()-dt.getEpoch()<=1);

	test("inode",fl.getInode()!=0);
	test("hard links",fl.getNumberOfHardLinks()==1);

	const char	*path="/usr/local/firstworks/include/rudiments/file.h";
	char	*dirname=file::dirname(path);
	test("dirname",!charstring::compare(dirname,
			"/usr/local/firstworks/include/rudiments"));
	delete[] dirname;

	char	*basename=file::basename(path);
	test("basename",!charstring::compare(basename,"file.h"));
	delete[] basename;

	basename=file::basename(path,".h");
	test("basename again",!charstring::compare(basename,"file"));
	delete[] basename;

	test("key",file::generateKey("testfile.txt",1)!=0);


	file::remove("testfile.txt");
}
