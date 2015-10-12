// Copyright (c) 2015  David Muse
// See the file COPYING for more information

#include <rudiments/url.h>
#include <rudiments/stdio.h>

// test urls...
const char * urls[]={
	"http://localhost/~dmuse/src/rudiments/test/url.cpp",
	"scp://[/home/dmuse/userpwd]@localhost/home/dmuse/src/rudiments/test/url.cpp",
	"http://www.google.com",
	NULL
};

url	u;

bool printUrl(const char *url) {

#if 0
	char	*contents=url::getContents(url);
	stdoutput.write(contents);
	return true;

#else
	// open the url
	if (u.open(url,O_RDONLY)) {

#if 0
		char	*contents=u.getContents();
		stdoutput.write(contents);
		return true;
#else
		// get and print it in 1024 byte chunks...
		char	buffer[1024];
		for (;;) {
			ssize_t	s=u.read(buffer,sizeof(buffer));
			stdoutput.write(buffer,s);
			if (s<(ssize_t)sizeof(buffer)) {
				stdoutput.write('\n');
				break;
			}
		}
#endif

		return true;
	}
	return false;
#endif
}

int main(int argc, const char **argv) {

	if (argc==2) {

		printUrl(argv[1]);
		
	} else {

		for (const char * const *url=urls; *url; url++) {

			// print out the url
			stdoutput.printf("%s\n\n",*url);

			if (!printUrl(*url)) {
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
}
