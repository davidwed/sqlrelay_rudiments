// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/inetclientsocket.h>

#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#include <errno.h>
#include <time.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	if (argc<3) {
		printf("usage: inetclntbench [buffer size] [iterations]\n");
		exit(0);
	}

	unsigned long	buffersize=charstring::toUnsignedLong(argv[1]);
	unsigned long	iterations=charstring::toUnsignedLong(argv[2]);

	// create an inet socket client
	inetclientsocket	clnt;

	// connect to a server on localhost, listening on port 8000
	if (clnt.connect("localhost",8000,-1,-1,1,1)<0) {
		printf("connect failed: %s\n",strerror(errno));
		exit(1);
	}

	unsigned char	writebuffer[buffersize];
	for (unsigned long index=0; index<buffersize; index++) {
		writebuffer[index]='h';
	}

	// write "hello" to the server 10000000 times
	time_t	start=time(NULL);
	for (unsigned long i=0; i<iterations; i++) {
		clnt.write(writebuffer,buffersize);
	}
	time_t	end=time(NULL);
	printf("total time=%d\n",end-start);

	// read 10 bytes from the server and display them
	char	buffer[11];
	int	sizeread=clnt.read(buffer,10);
	buffer[sizeread]=(char)NULL;
	printf("%s\n",buffer);

	// close the connection to the server
	clnt.close();
}
