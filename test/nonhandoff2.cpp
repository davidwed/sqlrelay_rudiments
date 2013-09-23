// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/inetsocketserver.h>
#include <rudiments/permissions.h>

int main(int argc, const char **argv) {

	inetsocketserver	svr;
	svr.listen(NULL,11000,15);

	for (;;) {
		svr.waitForNonBlockingRead(-1,-1);
		filedescriptor	*clientsock=svr.accept();
		clientsock->write("hello");
		clientsock->close();
		delete clientsock;
	}
}
