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
#include <rudiments/error.h>
#include <rudiments/process.h>
#include <rudiments/snooze.h>
#include <rudiments/stdio.h>
#include "test.cpp"

void handoff1() {

	// create a file containing "bye"
	file::remove("file.txt");
	file	f;
	test("handoff1 - create file",
		f.create("file.txt",permissions::evalPermString("rw-r--r--")));
	test("handoff1 - write to file",f.write("bye",3)==3);

	// open a unix socket
	unixsocketserver	handoffsock;
	test("handoff1 - unix socket",
			handoffsock.listen("handoff.sck",0000,15));

	// open an inet socket
	inetsocketserver	serversock;
	test("handoff1 - inet socket",
			serversock.listen(NULL,8001,15));

	// add the sockets to a listener
	listener	pool;
	pool.addReadFileDescriptor(&serversock);
	pool.addReadFileDescriptor(&handoffsock);

	// wait for a connection, it should be the handoff2 process
	test("handoff1 - listen for handoff2",
			pool.listen(-1,-1)==RESULT_SUCCESS);
	filedescriptor	*fd=pool.getReadReadyList()->getFirst()->getValue();
	test("handoff1 - ready list",
			pool.getReadReadyList()->getLength()==1);
	test("handoff1 - is handoff2",fd==&handoffsock);
	unixsocketclient	*handoffclientsock=
				(unixsocketclient *)handoffsock.accept();

	// wait for a connection, it should be the handoffclient process
	test("handoff1 - listen for handoffclient",
			pool.listen(-1,-1)==RESULT_SUCCESS);
	fd=pool.getReadReadyList()->getFirst()->getValue();
	test("handoff1 - ready list",
			pool.getReadReadyList()->getLength()==1);
	test("handoff1 - is handoffclient",fd==&serversock);
	inetsocketclient	*clientsock=
				(inetsocketclient *)serversock.accept();

	// pass the file to handoff2
	test("handoff1 - pass fd",
		handoffclientsock->passFileDescriptor(f.getFileDescriptor()));

	// pass handoffclient to handoff2
	test("handoff1 - pass socket",
		handoffclientsock->passSocket(clientsock->getFileDescriptor()));

	snooze::macrosnooze(2);

	// clean up
	serversock.close();
#ifndef _WIN32
	// FIXME: crashes on windows
	handoffsock.close();
#endif
	file::remove("handoff.sck");
}

void handoff2() {

	// connect to handoff1
	unixsocketclient clnt;
	test("handoff2 - connect",
		clnt.connect("handoff.sck",-1,-1,0,1)==RESULT_SUCCESS);


	// receive the file
	int32_t	fd;
	test("handoff2 - receive fd",clnt.receiveFileDescriptor(&fd));

	// check the contents
	char	buf[4];
	file	f;
	f.setFileDescriptor(fd);
	f.setPositionRelativeToBeginning(0);
	test("handoff2 - read from fd",f.read(buf,3)==3);
	buf[3]='\0';
	test("handoff2 - contents",!charstring::compare(buf,"bye"));

	// clean up
	file::remove("file.txt");


	// receive handoffclient
	int32_t	sock;
	test("handoff2 - receive socket",clnt.receiveSocket(&sock));

	// send the client a hello
	inetsocketclient	clientsock;
	clientsock.setFileDescriptor(sock);
	test("handoff2 - write to socket",clientsock.write("hello",5)==5);
	test("handoff2 - close",clientsock.close());
}

void handoffclient() {

	// connect to handoff1
	inetsocketclient	clnt;
	test("handoffclient - connect",
		clnt.connect("127.0.0.1",8001,-1,-1,0,1)==RESULT_SUCCESS);

	// read hello
	char	buffer[6];
	test("handoffclient - read",clnt.read(buffer,5)==5);
	buffer[5]='\0';
	test("handoffclient - contents",!charstring::compare(buffer,"hello"));
	test("handoffclient - close",clnt.close());
}

int main(int argc, const char **argv) {

	// FIXME: this really ought to work on SCO OSR5
	#ifdef RUDIMENTS_HAVE_BAD_SCO_MSGHDR
		return 1;
	#endif

	if (argc==1) {

		header("handoff");

		// spawn the handoff servers
		stringbuffer	cmd;
		char	*pwd=directory::getCurrentWorkingDirectory();
		cmd.append(pwd)->append("/handoff");
		#ifdef _WIN32
			cmd.append(".exe");
		#endif
		delete[] pwd;

		const char	*args1[]={"handoff","1",NULL};
		process::spawn(cmd.getString(),args1,true);

		const char	*args2[]={"handoff","2",NULL};
		process::spawn(cmd.getString(),args2,true);

		// give them a few seconds...
		snooze::macrosnooze(3);

		// connect as a client
		handoffclient();

		snooze::macrosnooze(1);
		stdoutput.printf("\n");

	} else if (!charstring::compare(argv[1],"1")) {

		handoff1();

	} else if (!charstring::compare(argv[1],"2")) {

		// give handoff1 a second
		snooze::macrosnooze(1);

		handoff2();
	}
}
