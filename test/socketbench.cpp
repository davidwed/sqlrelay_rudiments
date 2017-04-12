// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/listener.h>
#include <rudiments/inetsocketserver.h>
#include <rudiments/unixsocketserver.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/unixsocketclient.h>
#include <rudiments/file.h>
#include <rudiments/directory.h>
#include <rudiments/permissions.h>
#include <rudiments/datetime.h>
#include <rudiments/snooze.h>
#include <rudiments/commandline.h>
#include <rudiments/stdio.h>

bool		disablenagle=true;
uint32_t	socketreadbuffer=65536;
uint32_t	socketwritebuffer=65536;
size_t		readbuffer=65536;
size_t		writebuffer=65536;

void populateBytes(unsigned char *bytes, uint32_t bytecount) {
	unsigned char	c=0;
	for (uint32_t i=0; i<bytecount; i++) {
		bytes[i]=c;
		c++;
	}
}

void listen() {

	stdoutput.printf("listener {\n");

	// listen on inet socket port 8000
	inetsocketserver	inetsock;
	stdoutput.printf("	inet socket:	");
	if (inetsock.listen(NULL,8000,15)) {
		stdoutput.printf("success\n");
	} else {
		stdoutput.printf("failed\n}\n");
		return;
	}

	// listen on unix socket "listener.sck"
	unixsocketserver	unixsock;
	stdoutput.printf("	unix socket:	");
	if (unixsock.listen("listener.sck",0000,15)) {
		stdoutput.printf("success\n");
	} else {
		stdoutput.printf("failed\n}\n");
		return;
	}

	stdoutput.printf("}\n");

	// create a listener and add the 2 sockets to it
	listener	pool;
	pool.addReadFileDescriptor(&inetsock);
	pool.addReadFileDescriptor(&unixsock);

	// read/write buffer
	unsigned char	bytes[65536];

	// loop, waiting for connections
	for (;;) {

		stdoutput.printf("session {\n");

		// wait for a client to connect to one of the sockets
		if (pool.listen(-1,-1)!=RESULT_SUCCESS) {
			stdoutput.printf("	listen failed\n}\n");
			return;
		}

		filedescriptor	*fd=pool.getReadReadyList()->
						getFirst()->getValue();

		// figure out which socket the client connected to
		filedescriptor	*clientsock=NULL;
		if (fd==&inetsock) {
			stdoutput.printf("	inet socket\n");
			clientsock=inetsock.accept();
			if (disablenagle) {
				clientsock->dontUseNaglesAlgorithm();
			}
		} else if (fd==&unixsock) {
			stdoutput.printf("	unix socket\n");
			clientsock=unixsock.accept();
		}
		if (!clientsock) {
			stdoutput.printf("	accept failed\n}\n");
			return;
		}

		// set buffer sizes
		if (socketreadbuffer) {
			clientsock->setSocketReadBufferSize(socketreadbuffer);
		}
		if (socketwritebuffer) {
			clientsock->setSocketWriteBufferSize(socketwritebuffer);
		}

		// get loop count
		uint32_t	loopcount;
		if (clientsock->read(&loopcount)!=sizeof(uint32_t)) {
			stdoutput.printf("	read loopcount failed\n}\n");
			return;
		}

		// get byte count
		uint32_t	bytecount;
		if (clientsock->read(&bytecount)!=sizeof(uint32_t)) {
			stdoutput.printf("	"
					"read bytecount failed\n}\n");
			return;
		}

		bool	fail=false;
		for (;;) {

			// loop, reading from the client
			stdoutput.printf("	reading %d times...\n",
								loopcount);
			for (uint32_t i=0; i<loopcount && !fail; i++) {
				if (clientsock->read(bytes,
						bytecount)!=bytecount) {
					stdoutput.printf("	"
						"read bytes failed\n");
					fail=true;
				}
			}
			if (fail) {
				break;
			}
			stdoutput.printf("	read    %lld bytes\n",
				(uint64_t)bytecount*(uint64_t)loopcount);

			// loop, writing back to the client
			stdoutput.printf("	writing %d times...\n",
								loopcount);
			for (uint32_t i=0; i<loopcount && !fail; i++) {
				if (clientsock->write(bytes,
						bytecount)!=bytecount) {
					stdoutput.printf("	"
						"write bytes failed\n");
					fail=true;
				}
			}
			if (fail) {
				break;
			}
			clientsock->flushWriteBuffer(-1,-1);
			stdoutput.printf("	wrote   %lld bytes\n",
				(uint64_t)bytecount*(uint64_t)loopcount);
		}

		// close the socket and clean up
		clientsock->close();
		delete clientsock;

		stdoutput.printf("}\n");
	}

	snooze::macrosnooze(1);

	// clean up
	inetsock.close();
	unixsock.close();
	file::remove("listener.sck");
}

void session(socketclient *clnt) {

	// set buffer sizes
	if (socketreadbuffer) {
		clnt->setSocketReadBufferSize(socketreadbuffer);
	}
	if (socketwritebuffer) {
		clnt->setSocketWriteBufferSize(socketwritebuffer);
	}
	if (readbuffer) {
		clnt->setReadBufferSize(readbuffer);
	}
	if (writebuffer) {
		clnt->setWriteBufferSize(writebuffer);
	}

	// read/write buffer
	uint32_t	bytecount=65536;
	unsigned char	bytes[65536];
	populateBytes(bytes,sizeof(bytes));

	// send loop count
	uint32_t	loopcount=327680;
	if (clnt->write(loopcount)!=sizeof(uint32_t)) {
		stdoutput.printf("	write loopcount failed\n}\n");
		return;
	}

	// send byte count
	if (clnt->write(bytecount)!=sizeof(uint32_t)) {
		stdoutput.printf("	write bytecount failed\n}\n");
		return;
	}

	clnt->flushWriteBuffer(-1,-1);

	// try different chunk sizes
	for (uint32_t chunksize=65536; chunksize>=1024; chunksize/=2) {

		stdoutput.printf("	chunksize %d {\n",chunksize);

		// loop, writing to the server
		stdoutput.printf("		writing %d times...\n",
								loopcount);
		datetime	start;
		start.getSystemDateAndTime();
		for (uint32_t i=0; i<loopcount; i++) {
			uint32_t	byteswritten=0;
			while (byteswritten<bytecount) {
				if (clnt->write(bytes+byteswritten,
						chunksize)!=chunksize) {
					stdoutput.printf("	"
						"write bytes failed\n}\n");
					return;
				}
				byteswritten+=chunksize;
			}
		}
		clnt->flushWriteBuffer(-1,-1);
		datetime	end;
		end.getSystemDateAndTime();

		// Mbps
		uint32_t	sec=end.getEpoch()-start.getEpoch();
		int32_t		usec=end.getMicroseconds()-
					start.getMicroseconds();
 		if (usec<0) {
			sec--;
			usec=usec+1000000;
		}
		long double	totalsec=(long double)sec+
					(((long double)usec)/
					1000000.0);
		long double	mbps=(((long double)bytecount*
				(long double)loopcount)/1024.0/1024.0*8.0)/
				(long double)totalsec;
		stdoutput.printf("		wrote   %0.2Lf Mbps "
						"(%Lf seconds)\n",
						mbps,totalsec);


		// loop, reading back from the server
		stdoutput.printf("		reading %d times...\n",
								loopcount);
		start.getSystemDateAndTime();
		for (uint32_t i=0; i<loopcount; i++) {
			uint32_t	bytesread=0;
			while (bytesread<bytecount) {
				if (clnt->read(bytes+bytesread,
						chunksize)!=chunksize) {
					stdoutput.printf("	"
						"read bytes failed\n}\n");
					return;
				}
				bytesread+=chunksize;
			}
		}
		end.getSystemDateAndTime();

		// Mbps
		sec=end.getEpoch()-start.getEpoch();
		usec=end.getMicroseconds()-start.getMicroseconds();
 		if (usec<0) {
			sec--;
			usec=usec+1000000;
		}
		totalsec=(long double)sec+
				(((long double)usec)/
				1000000.0);
		mbps=(((long double)bytecount*
				(long double)loopcount)/1024.0/1024.0*8.0)/
				(long double)totalsec;
		stdoutput.printf("		read    %0.2Lf Mbps "
						"(%Lf seconds)\n",
						mbps,totalsec);

		stdoutput.printf("	}\n");
	}
}

void inetclient(const char *host) {

	stdoutput.printf("inetclient {\n");

	// create an inet socket client
	inetsocketclient	clnt;

	// connect to the server
	if (clnt.connect((charstring::isNullOrEmpty(host))?"127.0.0.1":host,
							8000,-1,-1,1,1)<0) {
		stdoutput.printf("	connect failed\n}\n");
		return;
	}

	// set socket options
	if (disablenagle) {
		clnt.dontUseNaglesAlgorithm();
	}

	// have a session with the server
	session(&clnt);

	// close the connection to the server
	clnt.close();

	stdoutput.printf("}\n");
}

void unixclient() {

	stdoutput.printf("unixclient {\n");

	// create an unix socket client
	unixsocketclient	clnt;

	// connect to the server
	if (clnt.connect("listener.sck",-1,-1,1,1)<0) {
		stdoutput.printf("	connect failed\n}\n");
		return;
	}

	// have a session with the server
	session(&clnt);

	// close the connection to the server
	clnt.close();

	stdoutput.printf("}\n");
}

int main(int argc, const char **argv) {

	commandline	cmdl(argc,argv);

	if (cmdl.found("readbuffer")) {
		readbuffer=charstring::toInteger(
				cmdl.getValue("readbuffer"));
	}
	if (cmdl.found("writebuffer")) {
		writebuffer=charstring::toInteger(
				cmdl.getValue("writebuffer"));
	}
	if (cmdl.found("socketreadbuffer")) {
		socketreadbuffer=charstring::toInteger(
				cmdl.getValue("socketreadbuffer"));
	}
	if (cmdl.found("socketwritebuffer")) {
		socketwritebuffer=charstring::toInteger(
				cmdl.getValue("socketwritebuffer"));
	}
	if (cmdl.found("dontdisablenagle")) {
		disablenagle=false;
	}

	stdoutput.printf("parameters {\n");
	stdoutput.printf("	readbuffer: 		%d\n",
							readbuffer);
	stdoutput.printf("	writebuffer: 		%d\n",
							writebuffer);
	stdoutput.printf("	socketreadbuffer: 	%d\n",
							socketreadbuffer);
	stdoutput.printf("	socketwritebuffer: 	%d\n",
							socketwritebuffer);
	stdoutput.printf("	disablenagle: 		%d\n",
							disablenagle);
	stdoutput.printf("}\n");
	

	if (cmdl.found("listener")) {
		listen();
	} else if (cmdl.found("inet")) {
		inetclient(cmdl.getValue("host"));
	} else if (cmdl.found("unix")) {
		unixclient();
	} else {
		stdoutput.printf("usage:	socketbench"
			" -listener|-inet|-unix\n"
			"			[-host host]\n"
			"			[-readbuffer size]\n"
			"			[-writebuffer size]\n"
			"			[-socketreadbuffer size]\n"
			"			[-socketwritebuffer size]\n"
			"			[-disablenagle]\n");
	}
}
