// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/listener.h>
#include <rudiments/inetsocketserver.h>
#include <rudiments/unixsocketserver.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/unixsocketclient.h>
#include <rudiments/directory.h>
#include <rudiments/permissions.h>
#include <rudiments/process.h>
#include <rudiments/error.h>
#include <rudiments/snooze.h>
#include <rudiments/stdio.h>
#include "test.cpp"

void listen() {

	// listen on inet socket port 8000
	inetsocketserver	inetsock;
	test("listener - inet socket",inetsock.listen(NULL,8000,15));

	// listen on unix socket "lsnr.sck"
	unixsocketserver	unixsock;
	test("listener - unix socket",unixsock.listen("lsnr.sck",0000,15));

	// create a listener and add the 2 sockets to it
	listener	pool;
	pool.addReadFileDescriptor(&inetsock);
	pool.addReadFileDescriptor(&unixsock);

	// loop, waiting for 2 connections
	for (uint16_t i=0; i<2; i++) {

		// wait for a client to connect to one of the sockets
		test("listener - result",pool.listen(-1,-1)==RESULT_SUCCESS);

		filedescriptor	*fd=pool.getReadReadyList()->
						getFirst()->getValue();
		test("listener - ready list",
				pool.getReadReadyList()->getLength()==1);

		// figure out which socket the client connected to
		filedescriptor	*clientsock;
		if (fd==&inetsock) {
			clientsock=inetsock.accept();
		} else if (fd==&unixsock) {
			clientsock=unixsock.accept();
		}
		test("listener - accept",clientsock);

		// read 5 bytes from the client and display it
		char	buffer[5];
		buffer[4]='\0';
		ssize_t	readlen=clientsock->read(buffer,4);
		test("listener - read length",readlen==4);
		if (i==0) {
			test("listener - read \"inet\"",
				!charstring::compare(buffer,"inet"));
		} else {
			test("listener - read \"unix\"",
				!charstring::compare(buffer,"unix"));
		}

		// write "listener" back to the client
		ssize_t	writelen=clientsock->write("listener",8);
		test("listener - write \"listener\"",writelen==8);

		// close the socket and clean up
		test("listener - close",clientsock->close());
		delete clientsock;
	}

	snooze::macrosnooze(1);

	stdoutput.printf("\n");
}

void inetclient() {

	// create an inet socket client
	inetsocketclient	clnt;

	// connect to the server
	test("inet client - connect",
			clnt.connect("127.0.0.1",8000,-1,-1,1,1)>=0);

	// write "inet" to the server
	test("inet client - write \"inet\"",clnt.write("inet",4)==4);

	// read 8 bytes from the server and display them
	char	buffer[9];
	test("inet client - read length",clnt.read(buffer,8)==8);
	buffer[8]='\0';
	test("inet client - read \"listener\"",
			!charstring::compare(buffer,"listener"));

	// close the connection to the server
	test("inet client - close",clnt.close());
}

void unixclient() {

	// create an unix socket client
	unixsocketclient	clnt;

	// connect to the server
	test("unix client - connect",
			clnt.connect("lsnr.sck",-1,-1,1,1)>=0);

	// write "hello" to the server
	test("unix client - write \"unix\"",clnt.write("unix",4)==4);

	// read 8 bytes from the server and display them
	char	buffer[9];
	test("unix client - read length",clnt.read(buffer,8)==8);
	buffer[8]='\0';
	test("unix client - read \"listener\"",
			!charstring::compare(buffer,"listener"));

	// close the connection to the server
	test("unix client - close",clnt.close());
}

int main(int argc, const char **argv) {

	if (argc==1) {

		header("listener");

		// spawn inet and unix clients
		stringbuffer	cmd;
		char	*pwd=directory::getCurrentWorkingDirectory();
		cmd.append(pwd)->append("/listener");
		#ifdef _WIN32
			cmd.append(".exe");
		#endif
		delete[] pwd;

		const char	*args1[]={"listener","inet",NULL};
		process::spawn(cmd.getString(),args1,true);

		const char	*args2[]={"listener","unix",NULL};
		process::spawn(cmd.getString(),args2,true);

		// listen for them to connect
		listen();

	} else if (!charstring::compare(argv[1],"inet")) {

		// give the server a second to start
		snooze::macrosnooze(1);

		inetclient();

	} else if (!charstring::compare(argv[1],"unix")) {

		// give the server a second to start
		// and give the other client a second too
		snooze::macrosnooze(2);

		unixclient();
	}
}
