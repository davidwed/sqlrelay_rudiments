#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	file	f;

	// Create a file, or open it for write if it already exists.
	if (f.open("testfile1",O_WRONLY|O_CREAT,
				permissions::evalPermString("rw-rw-rw-"))) {
		stdoutput.write("created/opened testfile1\n");
	} else {
		stdoutput.write("failed to create/open testfile1\n");
	}

	// Attempt to create a file, and fail if it already exists.
	// (the previously open file will be closed automatically)
	if (f.open("testfile1",O_WRONLY|O_CREAT|O_EXCL,
				permissions::evalPermString("rw-rw-rw-"))) {
		stdoutput.write("created/opened testfile1\n");
	} else {
		stdoutput.write("failed to create/open testfile1 "
				"(this is the correct behavior)\n");
	}

	// Open a file for read, starting at the beginning of the file.
	// (the previously open file will be closed automatically)
	if (f.open("testfile1",O_RDONLY)) {
		stdoutput.write("opened testfile1 for read\n");
	} else {
		stdoutput.write("failed to open testfile1 for read\n");
	}

	// Open a file for write, starting at the beginning of the file.
	// (the previously open file will be closed automatically)
	if (f.open("testfile1",O_WRONLY)) {
		stdoutput.write("opened testfile1 for write\n");
	} else {
		stdoutput.write("failed to open testfile1 for write\n");
	}

	// You can also close the file manually.
	f.close();

	// Open a file for read and write,
	// starting at the beginning of the file.
	if (f.open("testfile1",O_RDWR)) {
		stdoutput.write("opened testfile1 for read/write\n");
	} else {
		stdoutput.write("failed to open testfile1 for read/write\n");
	}

	// Close the file manually.
	f.close();

	// Open a file for write, starting at the end of the file.
	if (f.open("testfile1",O_WRONLY|O_APPEND)) {
		stdoutput.write("opened testfile1 for append\n");
	} else {
		stdoutput.write("failed to open testfile1 for append\n");
	}

	// Close the file manually.
	f.close();

	// Open a file for write,
	// first removing the current contents of the file.
	if (f.open("testfile1",O_WRONLY|O_TRUNC)) {
		stdoutput.write("opened testfile1 for append\n");
	} else {
		stdoutput.write("failed to open testfile1 for append\n");
	}

	// when main() exits, f will be deleted and the file will be closed.
}
