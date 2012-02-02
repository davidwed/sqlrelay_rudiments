// Copyright (c) 2004  David Muse
// See the file COPYING for more information

#include <rudiments/directory.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	directory::create("testdir",permissions::evalPermString("rwxr-xr-x"));
	file	f;
	f.create("testdir/testfile1",permissions::evalPermString("rw-r--r--"));
	f.close();
	f.create("testdir/testfile2",permissions::evalPermString("rw-r--r--"));
	f.close();


	printf("Contents of testdir:\n");
	directory	d;
	d.open("testdir");

	char	*name;
	for (off64_t index=0; (name=d.getChildName(index)); index++) {
		printf("%lld: %s\n",index,name);
		delete[] name;
	}

	printf("%lld entries\n",d.getChildCount());

	char	*cwd=directory::getCurrentWorkingDirectory();
	printf("cwd=%s\n",cwd);
	delete[] cwd;

	directory::changeDirectory("testdir");

	cwd=directory::getCurrentWorkingDirectory();
	printf("cwd=%s\n",cwd);
	delete[] cwd;

	directory::changeDirectory("..");

	cwd=directory::getCurrentWorkingDirectory();
	printf("cwd=%s\n",cwd);
	delete[] cwd;

	file::remove("testdir/testfile1");
	file::remove("testdir/testfile2");
	directory::remove("testdir");

	printf("maxFileNameLength(/usr/local)=%lld\n",
			directory::maxFileNameLength("/usr/local"));
	printf("maxPathLength(/usr/local)=%lld\n",
			directory::maxPathLength("/usr/local"));
	printf("canAccessLongFileNames(/usr/local)=%d\n",
			directory::canAccessLongFileNames("/usr/local"));
}