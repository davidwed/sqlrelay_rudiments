#include <rudiments/file.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	file	f;
	char	buffer[1024];

	// for each file specified on the command line...
	for (int32_t i=1; i<argc; i++) {

		// open the file
		if (!f.open(argv[i],O_RDONLY)) {
			continue;
		}

		// read chunks from the file and print each chunk..
		ssize_t	bytesread=0;
		do {

			// attempt to read 1024 bytes into the buffer
			bytesread=f.read(buffer,1024);

			// bytesread will be the number of bytes that were
			// actually read, 0 at EOF, or a negative number
			// if an error occurred
			if (bytesread>0) {

				// print the buffer
				stdoutput.write(buffer,bytesread);
			}

		// exit if we read fewer than 1024 bytes
		} while (bytesread==1024);
	}
}
