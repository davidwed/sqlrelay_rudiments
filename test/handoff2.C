// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/unixclientsocket.h>
#include <rudiments/permissions.h>
#include <errno.h>
#include <strings.h>

int	main() {

	unixclientsocket clnt;
	clnt.connectToServer("/tmp/handoff.socket",0,1);

	for (;;) {
		int		descriptor;
		clnt.receiveFileDescriptor(&descriptor);
		transport	*clientsock=new transport(descriptor);
		clientsock->write("hello");
		clientsock->close();
		delete clientsock;
	}
}
