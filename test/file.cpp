// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/groupentry.h>
#include <rudiments/userentry.h>
#include <rudiments/file.h>
#include <rudiments/directory.h>
#include <rudiments/permissions.h>
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>
#include <rudiments/process.h>
#include <rudiments/sys.h>
#include <rudiments/snooze.h>
#include <rudiments/error.h>
#include "test.cpp"

const char	*testfiletxt="testfile.txt";

int main(int argc, const char **argv) {

	char	*osname=sys::getOperatingSystemName();

	// file must be on a local fs on SCO for lock tests to work
	if (!charstring::compare(osname,"UnixWare") ||
		!charstring::compare(osname,"SCO_SV")) {
		testfiletxt="/tmp/testfile.txt";
	}

	file	fl;

	if (argc==1) {

		header("file");

		file::remove(testfiletxt);

		error::clearError();
		test("create without perms",
				!fl.create(testfiletxt,0));
		test("error",error::getErrorNumber()==EINVAL);
		error::clearError();
		test("create without perms",
				!fl.open(testfiletxt,O_RDWR|O_CREAT));
		test("error",error::getErrorNumber()==EINVAL);
		error::clearError();
		test("create without perms",
				!fl.open(testfiletxt,O_RDWR|O_CREAT,0));
		test("error",error::getErrorNumber()==EINVAL);
		test("create",
			fl.open(testfiletxt,O_RDWR|O_CREAT,
				permissions::evalPermString("rw-rw----")));
		fl.close();
		test("create when already exists",
			fl.open(testfiletxt,O_RDWR|O_CREAT,
				permissions::evalPermString("rw-rw----")));
		fl.close();
		test("create with excl",
			!fl.open(testfiletxt,O_RDWR|O_CREAT|O_EXCL,
				permissions::evalPermString("rw-rw----")));
		file::remove(testfiletxt);
		test("create",
			fl.create(testfiletxt,
				permissions::evalPermString("rw-rw----")));
		test("write",fl.write("hello")==5);
		test("get properties",fl.getCurrentProperties());
		test("close",fl.close());
		test("exists",file::exists(testfiletxt));
		test("reopen",fl.open(testfiletxt,O_RDWR));

		uid_t	uid=fl.getOwnerUserId();
		char	*username=userentry::getName(uid);
		test("user",uid==process::getUserId());

		gid_t	gid=fl.getOwnerGroupId();
		char	*groupname=groupentry::getName(gid);
		test("group",gid==process::getGroupId());

		// not supported on syllable
		if (charstring::compare(osname,"syllable")) {
			test("chown",fl.changeOwner(username,groupname));
		}

		delete[] username;
		delete[] groupname;

		test("size",fl.getSize()==5);

		test("block size",fl.getBlockSize()>0);

		// not supported on syllable
		if (charstring::compare(osname,"syllable")) {
			test("blocks",fl.getBlockCount()!=0);
		}

		test("is a socket",!fl.isSocket());
		test("is a symlink",!fl.isSymbolicLink());
		test("is a regular file",fl.isRegularFile());
		test("is a block device",!fl.isBlockDevice());
		test("is a directory",!fl.isDirectory());
		test("is a character device",!fl.isCharacterDevice());
		test("is a fifo",!fl.isFifo());

		datetime	dt;
		dt.getSystemDateAndTime();

		// these aren't reliable over nfs/cifs...
		/*datetime	atime;
		atime.initialize(fl.getLastAccessTime());
		test("access time",atime.getEpoch()-dt.getEpoch()<=1);

		datetime	mtime;
		mtime.initialize(fl.getLastModificationTime());
		test("modification time",mtime.getEpoch()-dt.getEpoch()<=1);

		datetime	ctime;
		mtime.initialize(fl.getLastChangeTime());
		test("change time",ctime.getEpoch()-dt.getEpoch()<=1);*/

		test("inode",fl.getInode()!=0);
		test("hard links",fl.getNumberOfHardLinks()==1);

		const char	*path=
			"/usr/local/firstworks/include/rudiments/file.h";
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

		test("key",file::generateKey(testfiletxt,1)!=0);


		fl.open(testfiletxt,O_RDWR);
		test("lock 1",fl.lockFile(
				#if defined(F_RDLCK) && defined(F_WRLCK)
					F_RDLCK|F_WRLCK
				#else
					0
				#endif
					));

		stringbuffer	cmd;
		char	*pwd=directory::getCurrentWorkingDirectory();
		cmd.append(pwd)->append("/file");
		#ifdef _WIN32
			cmd.append(".exe");
		#endif
		delete[] pwd;
		const char	*args1[]={"file","child",NULL};
		process::spawn(cmd.getString(),args1,true);

		snooze::macrosnooze(1);

		test("unlock 1",fl.unlockFile());

		snooze::macrosnooze(1);
		stdoutput.printf("\n");

		fl.close();

		file::remove(testfiletxt);

	} else {

		fl.open(testfiletxt,O_RDWR);

		test("try lock 2",!fl.tryLockFile(
				#if defined(F_RDLCK) && defined(F_WRLCK)
					F_RDLCK|F_WRLCK
				#else
					0
				#endif
					));

		test("lock 2",fl.lockFile(
				#if defined(F_RDLCK) && defined(F_WRLCK)
					F_RDLCK|F_WRLCK
				#else
					0
				#endif
					));
	}

	delete[] osname;
}
