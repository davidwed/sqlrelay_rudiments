// Copyright (c) 2004  David Muse
// See the file COPYING for more information

#include <rudiments/directory.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include <rudiments/error.h>
#include "test.cpp"

/*#ifdef _WIN32
const char	*dir="C:\\";
#else
const char	*dir="/usr/local";
#endif*/

int main(int argc, const char **argv) {

	header("directory");

	// clean up, just in case
	#ifdef _WIN32
	file::remove("testdir\\testfile1");
	file::remove("testdir\\testfile2");
	#else
	file::remove("testdir/testfile1");
	file::remove("testdir/testfile2");
	#endif
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

	const char *filenames[]={".","..","testfile1","testfile2",NULL};

	// contents
	stdoutput.printf("contents...\n");
	directory	d;
	test("open",d.open("testdir"));
	d.rewind();
	char	*name=d.read();
	test("testfile",charstring::inSet(name,filenames));
	delete[] name;
	name=d.read();
	test("testfile",charstring::inSet(name,filenames));
	delete[] name;
	name=d.read();
	test("testfile",charstring::inSet(name,filenames));
	delete[] name;
	name=d.read();
	test("testfile",charstring::inSet(name,filenames));
	delete[] name;

	d.rewind();
	name=d.read();
	test("testfile",charstring::inSet(name,filenames));
	delete[] name;
	name=d.read();
	test("testfile",charstring::inSet(name,filenames));
	delete[] name;
	name=d.read();
	test("testfile",charstring::inSet(name,filenames));
	delete[] name;
	name=d.read();
	test("testfile",charstring::inSet(name,filenames));
	delete[] name;

	d.close();
	stdoutput.printf("\n");


	// contents by index
	stdoutput.printf("contents by index...\n");
	test("open",d.open("testdir"));
	test("count",d.getChildCount()==4);
	uint64_t	i=0;
	name=d.getChildName(i++);
	test("testfile",charstring::inSet(name,filenames));
	delete[] name;
	name=d.getChildName(i++);
	test("testfile",charstring::inSet(name,filenames));
	delete[] name;
	name=d.getChildName(i++);
	test("testfile",charstring::inSet(name,filenames));
	delete[] name;
	name=d.getChildName(i++);
	test("testfile",charstring::inSet(name,filenames));
	delete[] name;
	d.close();
	stdoutput.printf("\n");


	// current dir/change dir
	stdoutput.printf("current dir/change dir...\n");
	char	*cwd=directory::getCurrentWorkingDirectory();
	#ifdef _WIN32
	const char	*dir=charstring::findLast(cwd,"\\test");
	test("\\test",!charstring::compare(dir,"\\test"));
	#else
	const char	*dir=charstring::findLast(cwd,"/test");
	test("/test",!charstring::compare(dir,"/test"));
	#endif
	delete[] cwd;
	directory::changeDirectory("testdir");
	cwd=directory::getCurrentWorkingDirectory();
	#ifdef _WIN32
	dir=charstring::findLast(cwd,"\\test\\testdir");
	test("\\test\\testdir",!charstring::compare(dir,"\\test\\testdir"));
	#else
	dir=charstring::findLast(cwd,"/test/testdir");
	test("/test/testdir",!charstring::compare(dir,"/test/testdir"));
	#endif
	delete[] cwd;
	directory::changeDirectory("..");
	cwd=directory::getCurrentWorkingDirectory();
	#ifdef _WIN32
	dir=charstring::findLast(cwd,"\\test");
	test("\\test",!charstring::compare(dir,"\\test"));
	#else
	dir=charstring::findLast(cwd,"/test");
	test("/test",!charstring::compare(dir,"/test"));
	#endif
	delete[] cwd;
	stdoutput.printf("\n");


	// remove
	stdoutput.printf("remove...\n");
	#ifdef _WIN32
	test("remove testdir\\testfile1",file::remove("testdir\\testfile1"));
	test("remove testdir\\testfile2",file::remove("testdir\\testfile2"));
	#else
	test("remove testdir/testfile1",file::remove("testdir/testfile1"));
	test("remove testdir/testfile2",file::remove("testdir/testfile2"));
	#endif
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
	test("maxFileNameLength cross-check",
			directory::maxFileNameLength(cwd)==
			d.maxFileNameLength());
	test("maxPathLength cross-check",
			directory::maxPathLength(cwd)==
			d.maxPathLength());
	test("canExceedMaxFileNameLength cross-check",
			directory::canExceedMaxFileNameLength(cwd)==
			d.canExceedMaxFileNameLength());
	d.close();
	delete[] cwd;

	// chroot?

	stdoutput.printf("\n");
}
