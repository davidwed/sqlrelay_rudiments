// Copyright (c) 2004  David Muse
// See the file COPYING for more information

#include <rudiments/directory.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include <rudiments/error.h>
#include "test.cpp"

#ifdef _WIN32
const char	*dir="C:\\";
#else
const char	*dir="/usr/local";
#endif

int main(int argc, const char **argv) {


	// clean up, just in case
	file::remove("testdir/testfile1");
	file::remove("testdir/testfile2");
	directory::remove("testdir");


	// create
	stdoutput.printf("create...\n");
	test("create testdir",
			directory::create("testdir",
				permissions::evalPermString("rwxr-xr-x")));
	file	f;
	test("create testdir/testfile1",
			f.create("testdir/testfile1",
				permissions::evalPermString("rw-r--r--")));
	f.close();
	test("create testdir/testfile2",
			f.create("testdir/testfile2",
				permissions::evalPermString("rw-r--r--")));
	f.close();
	stdoutput.printf("\n");


	// contents by index
	stdoutput.printf("contents...\n");
	directory	d;
	test("open",d.open("testdir"));
	d.rewind();
	char	*name;
	name=d.read();
	test(".",!charstring::compare(name,"."));
	delete[] name;
	name=d.read();
	test("..",!charstring::compare(name,".."));
	delete[] name;
	name=d.read();
	test("testfile1",!charstring::compare(name,"testfile1"));
	delete[] name;
	name=d.read();
	test("testfile2",!charstring::compare(name,"testfile2"));
	delete[] name;

	d.rewind();
	name=d.read();
	test(".",!charstring::compare(name,"."));
	delete[] name;
	name=d.read();
	test("..",!charstring::compare(name,".."));
	delete[] name;
	name=d.read();
	test("testfile1",!charstring::compare(name,"testfile1"));
	delete[] name;
	name=d.read();
	test("testfile2",!charstring::compare(name,"testfile2"));
	delete[] name;

	d.rewind();
	name=d.read();
	test(".",!charstring::compare(name,"."));
	delete[] name;
	test("(skip)",d.skip());
	test("(skip)",d.skip());
	name=d.read();
	test("testfile2",!charstring::compare(name,"testfile2"));
	delete[] name;

	d.close();
	stdoutput.printf("\n");


	// contents by index
	stdoutput.printf("contents by index...\n");
	test("open",d.open("testdir"));
	test("count",d.getChildCount()==4);
	name=d.getChildName(0);
	test(".",!charstring::compare(name,"."));
	delete[] name;
	name=d.getChildName(1);
	test("..",!charstring::compare(name,".."));
	delete[] name;
	name=d.getChildName(2);
	test("testfile1",!charstring::compare(name,"testfile1"));
	delete[] name;
	name=d.getChildName(3);
	test("testfile2",!charstring::compare(name,"testfile2"));
	delete[] name;
	d.close();
	stdoutput.printf("\n");


	// current dir/change dir
	stdoutput.printf("current dir/change dir...\n");
	char	*cwd=directory::getCurrentWorkingDirectory();
	const char	*dir=charstring::findLast(cwd,"/test");
	test("/test",!charstring::compare(dir,"/test"));
	delete[] cwd;
	directory::changeDirectory("testdir");
	cwd=directory::getCurrentWorkingDirectory();
	dir=charstring::findLast(cwd,"/test/testdir");
	test("/test/testdir",!charstring::compare(dir,"/test/testdir"));
	delete[] cwd;
	directory::changeDirectory("..");
	cwd=directory::getCurrentWorkingDirectory();
	dir=charstring::findLast(cwd,"/test");
	test("/test",!charstring::compare(dir,"/test"));
	delete[] cwd;
	stdoutput.printf("\n");


	// remove
	stdoutput.printf("remove...\n");
	test("remove testdir/testfile1",file::remove("testdir/testfile1"));
	test("remove testdir/testfile2",file::remove("testdir/testfile2"));
	test("remove testdir",directory::remove("testdir"));
	stdoutput.printf("\n");


	// path-related
	stdoutput.printf("path-related...\n");
	cwd=directory::getCurrentWorkingDirectory();
	test("maxFileNameLength valid",
			directory::maxFileNameLength(cwd)>0);
	test("maxPathLength valid",
			directory::maxPathLength(cwd)>0);
	d.open(cwd);
stdoutput.printf("mfnl=%d/%d\n",
		directory::maxFileNameLength(cwd),
		d.maxFileNameLength());
stdoutput.printf("mpl=%d/%d\n",
		directory::maxPathLength(cwd),
		d.maxPathLength());
stdoutput.printf("c=%d/%d\n",
		directory::canAccessLongFileNames(cwd),
		d.canAccessLongFileNames());
	test("maxFileNameLength cross-check",
			directory::maxFileNameLength(cwd)==
			d.maxFileNameLength());
	test("maxPathLength cross-check",
			directory::maxPathLength(cwd)==
			d.maxPathLength());
	test("canAccessLongFileNames cross-check",
			directory::canAccessLongFileNames(cwd)==
			d.canAccessLongFileNames());
	d.close();
	delete[] cwd;

	// chroot?
}
