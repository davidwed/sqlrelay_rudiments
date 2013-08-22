// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/inetclientsocket.h>
#include <rudiments/error.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>
#include <time.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	if (argc<3) {
		stdoutput.printf("usage: inetclntbench [buffer size] [bytesperiteration] [iterations]\n");
		process::exit(0);
	}

	uint32_t	buffersize=
				charstring::toUnsignedInteger(argv[1]);
	uint32_t	bytesperiteration=
				charstring::toUnsignedInteger(argv[2]);
	uint32_t	iterations=
				charstring::toUnsignedInteger(argv[3]);

	char	*bytes=new char[bytesperiteration];
	if (!bytes) {
		stdoutput.printf("buffer allocation failed\n");
		process::exit(0);
	}
	for (uint32_t i=0; i<bytesperiteration; i++) {
		bytes[i]='C';
	}

	// create an inet socket client
	inetclientsocket	clnt;

	// connect to a server on localhost, listening on port 8000
	if (clnt.connect("127.0.0.1",8000,-1,-1,1,1)<0) {
		stdoutput.printf("connect failed: %s\n",
					error::getErrorString());
		process::exit(1);
	}

	clnt.setWriteBufferSize(buffersize);

	time_t	start=time(NULL);
	for (uint32_t i=0; i<iterations; i++) {
		clnt.write(bytes,bytesperiteration);
	}
	clnt.flushWriteBuffer(-1,-1);
	time_t	end=time(NULL);
	stdoutput.printf("total time=%ld\n",end-start);

	// close the connection to the server
	clnt.close();
}
