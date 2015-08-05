#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	file		f;
	uint32_t	i;
	uint32_t	j;
	char		c;

	// open/create the file
	f.open("testfile",O_WRONLY|O_CREAT|O_TRUNC,
				permissions::evalPermString("rw-rw-rw-"));

	// write 1mb of characters to the file, unbuffered
	stdoutput.write("writing unbuffered...\n");
	for (i=0; i<1024*1024; i++) {
		f.write('a');
	}
	stdoutput.write("done\n");

	// truncate the file
	f.truncate();

	// write 1mb of characters to the file, buffered
	stdoutput.write("writing buffered...\n");
	f.setWriteBufferSize(4096);
	for (i=0; i<1024*1024; i++) {
		f.write('a');
	}
	f.flushWriteBuffer(-1,-1);
	stdoutput.write("done\n");

	// read 1mb of characters from the file, unbuffered (10 times)
	stdoutput.write("reading unbuffered...\n");
	for (i=0; i<10; i++) {
		f.setPositionRelativeToBeginning(0);
		for (j=0; j<1024*1024; j++) {
			f.read(&c);
		}
	}
	stdoutput.write("done\n");

	// read 1mb of characters from the file, buffered (10 times)
	stdoutput.write("reading buffered...\n");
	for (i=0; i<10; i++) {
		f.setPositionRelativeToBeginning(0);
		for (j=0; j<1024*1024; j++) {
			f.read(&c);
		}
	}
	stdoutput.write("done\n");
}
