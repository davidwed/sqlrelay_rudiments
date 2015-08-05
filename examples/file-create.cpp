#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	file	f;

	// Create a file using the create() method, with read-write
	// permissions for everyone.
	if (f.create("testfile1",permissions::evalPermString("rw-rw-rw-"))) {
		stdoutput.write("testfile1 created\n");
	} else {
		stdoutput.write("failed to create testfile1\n");
	}


	// Create a file using the open() method and O_CREAT flag,
	// with read-write permissions for everyone.
	if (f.open("testfile2",O_CREAT,
				permissions::evalPermString("rw-rw-rw-"))) {
		stdoutput.write("testfile2 created\n");
	} else {
		stdoutput.write("failed to create testfile2\n");
	}


	// An attempt to create a file that already exists will just open
	// the file.
	if (f.create("testfile1",permissions::evalPermString("rw-rw-rw-"))) {
		stdoutput.write("testfile1 opened\n");
	} else {
		stdoutput.write("failed to open testfile1\n");
	}


	// If O_EXCL and O_CREAT are used together, then an attempt to create a
	// file that already exists will fail.
	if (f.open("testfile2",O_CREAT|O_EXCL,
				permissions::evalPermString("rw-rw-rw-"))) {
		stdoutput.write("testfile2 opened\n");
	} else {
		stdoutput.write("failed to open testfile2 "
				"(this is the correct behavior)\n");
	}
}
