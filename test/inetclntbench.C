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
		printf("usage: inetclntbench [buffer size] [bytesperiteration] [iterations]\n");
		exit(0);
	}

	uint32_t	buffersize=charstring::toUnsignedInt32(argv[1]);
	uint32_t	bytesperiteration=charstring::toUnsignedInt32(argv[2]);
	uint32_t	iterations=charstring::toUnsignedInt32(argv[3]);

	char	*bytes=new char[bytesperiteration];
	if (!bytes) {
		printf("buffer allocation failed\n");
		exit(0);
	}
	for (uint32_t i=0; i<bytesperiteration; i++) {
		bytes[i]='C';
	}

	// create an inet socket client
	inetclientsocket	clnt;

	// connect to a server on localhost, listening on port 8000
	if (clnt.connect("localhost",8000,-1,-1,1,1)<0) {
		printf("connect failed: %s\n",strerror(errno));
		exit(1);
	}

	clnt.setWriteBufferSize(buffersize);

	time_t	start=time(NULL);
	for (uint32_t i=0; i<iterations; i++) {
		clnt.write(bytes,bytesperiteration);
	}
	clnt.flushWriteBuffer(-1,-1);
	time_t	end=time(NULL);
	printf("total time=%d\n",end-start);

	// close the connection to the server
	clnt.close();
}
