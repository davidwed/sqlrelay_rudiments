// Copyright (c) 2015  David Muse
// See the file COPYING for more information

#include <rudiments/url.h>
#include <rudiments/stdio.h>

// test urls...
const char * urls[]={
	"http://localhost/~dmuse/src/rudiments/test/url.cpp",
	"scp://[/home/dmuse/userpwd]@localhost/home/dmuse/src/rudiments/test/url.cpp",
	NULL
};

int main(int argc, const char **argv) {

	url	u;

	for (const char * const *url=urls; *url; url++) {

		// print out the url
		stdoutput.printf("%s\n\n",*url);

		// open the url
		if (u.open(*url,O_RDONLY)) {

			// get and print it in 1024 byte chunks...
			char	buffer[1024];
			for (;;) {
				ssize_t	s=u.read(buffer,sizeof(buffer));
				stdoutput.write(buffer,s);
				if (s<1) {
					break;
				}
			}

		} else {
			stdoutput.printf("failed to open");
		}

		// banner
		stdoutput.printf("\n");
		for (uint16_t i=0; i<80; i++) {
			stdoutput.printf("=");
		}
		stdoutput.printf("\n");
	}
}
