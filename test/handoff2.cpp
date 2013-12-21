// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/unixsocketclient.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/file.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>
#include <rudiments/error.h>

int main(int argc, const char **argv) {

	unixsocketclient clnt;
	clnt.connect("handoff.sck",-1,-1,0,1);

	for (;;) {

		int32_t	fd;
		if (!clnt.receiveFileDescriptor(&fd)) {
			stdoutput.printf("receive file descriptor failed\n");
			process::exit(1);
		}

		int32_t	sock;
		if (!clnt.receiveSocket(&sock)) {
			stdoutput.printf("receive socket failed\n");
			process::exit(1);
		}

		char	buf[4];
		file	f;
		f.setFileDescriptor(fd);
		f.setPositionRelativeToBeginning(0);
		if (f.read(buf,3)!=3) {
			stdoutput.printf("read from file descriptor failed\n");
		}
		buf[3]='\0';
		stdoutput.printf("%s\n",buf);

		inetsocketclient	clientsock;
		clientsock.setFileDescriptor(sock);
		clientsock.write("hello");
		clientsock.close();
	}
}
