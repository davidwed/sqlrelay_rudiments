#include <rudiments/file.h>
#include <rudiments/permissions.h>

int main(int argc, const char **argv) {

	// open/create a file
	file	f;
	f.open("testfile",O_WRONLY|O_CREAT,
				permissions::evalPermString("rw-rw-rw-"));

	// write some text to it
	f.write("this is some text\n");

	// truncate the file
	f.truncate();

	// write some different text to it
	f.write("this is some different text\n");
}
