// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/listener.h>
#include <rudiments/inetserversocket.h>
#include <rudiments/unixserversocket.h>
#include <rudiments/permissions.h>
#include <stdio.h>

int	main() {

	inetserversocket	inetsock;
	inetsock.listenOnSocket(NULL,8000,15);

	unixserversocket	unixsock;
	unixsock.listenOnSocket("/tmp/lsnr.socket",0000,15);

	listener	pool;
	pool.addFileDescriptor(inetsock.getFileDescriptor());
	pool.addFileDescriptor(unixsock.getFileDescriptor());

	for (;;) {

		int	fd=pool.waitForData(-1,-1);

		transport	*sock;
		if (fd==inetsock.getFileDescriptor()) {
			sock=inetsock.acceptClientConnection();
			printf("inetsock: ");
		} else if (fd==unixsock.getFileDescriptor()) {
			sock=unixsock.acceptClientConnection();
			printf("unixsock: ");
		}

		char	buffer[6];
		buffer[5]=(char)NULL;
		sock->read(buffer,5);
		printf("%s\n",buffer);

		sock->write("hello",5);
		sock->close();
	}
}
