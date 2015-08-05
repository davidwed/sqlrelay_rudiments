#include <rudiments/file.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	file	f;

	// for each file specified on the command line...
	for (int32_t i=1; i<argc; i++) {

		// open the file
		if (!f.open(argv[i],O_RDONLY)) {
			continue;
		}

		// read lines from the file and print each line...
		ssize_t	bytesread=0;
		do {

			// attempt to read a line
			char	*line=NULL;
			bytesread=f.read(&line,"\n");

			// bytesread will be the number of bytes that were
			// actually read, 0 at EOF, or a negative number
			// if an error occurred
			if (bytesread>0) {

				// print the line
				stdoutput.write(line);
			}

			// clean up
			delete[] line;

		// exit on eof or error
		} while (bytesread>0);
	}
}
