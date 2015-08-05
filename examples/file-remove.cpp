#include <rudiments/file.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {
	
	// remove testfile1
	if (file::remove("testfile1")) {
		stdoutput.write("testfile1 removed\n");
	} else {
		stdoutput.write("failed to remove testfile1\n");
	}

	// remove testfile2
	if (file::remove("testfile2")) {
		stdoutput.write("testfile2 removed\n");
	} else {
		stdoutput.write("failed to remove testfile2\n");
	}
}
