// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/daemonprocess.h>
#include <rudiments/permissions.h>
#include <rudiments/inetserversocket.h>

#include <rudiments/private/config.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

class myserver : public daemonprocess, public inetserversocket {
	public:
			myserver() : daemonprocess(), inetserversocket() {}
		void	listen();
};


void	myserver::listen() {

	int	pid=checkForPidFile("/tmp/svr.pidfile");
	if (pid>-1) {
		printf("Sorry, an instance of this server is already running with process id: %d\n",pid);
		return;
	}

	createPidFile("/tmp/svr.pidfile",permissions::ownerReadWrite());
	detach();

	listenOnSocket(NULL,8000,15);

	for (;;) {
		transport	*clientsock=acceptClientConnection();
		char	buffer[6];
		buffer[5]=(char)NULL;
		int	sizeread=clientsock->read(buffer,5);
		printf("%s\n",buffer);
		clientsock->write("hello",5);
		clientsock->close();
		delete clientsock;
	}
}

myserver	*mysvr;

RETSIGTYPE	shutDown() {
	printf("shutting down\n");
	mysvr->close();
	delete mysvr;
	unlink("/tmp/svr.pidfile");
	exit(0);
}

int	main() {

	mysvr=new myserver();
	mysvr->handleShutDown((RETSIGTYPE *)shutDown);
	mysvr->listen();
	delete mysvr;
	exit(0);
}
