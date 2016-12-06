// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/listener.h>
#include <rudiments/unixsocketserver.h>
#include <rudiments/unixsocketclient.h>
#include <rudiments/file.h>
#include <rudiments/directory.h>
#include <rudiments/process.h>
#include <rudiments/snooze.h>
#include <rudiments/stdio.h>
#include "test.cpp"

void listen() {

	file::remove("listener.sck");
	unixsocketserver	unixsock;
	unixsock.listen("listener.sck",0000,15);
	delete unixsock.accept();

	snooze::macrosnooze(1);

	unixsock.close();
}

void unixclient() {
	unixsocketclient	clnt;
	clnt.connect("listener.sck",-1,-1,1,1);
}

int main(int argc, const char **argv) {

	if (argc==1) {

char	ch;
stdinput.read(&ch);

		stringbuffer	cmd;
		char	*pwd=directory::getCurrentWorkingDirectory();
		cmd.append(pwd)->append("/listener");
		#ifdef _WIN32
			cmd.append(".exe");
		#endif
		delete[] pwd;
		const char	*args2[]={"listener","unix",NULL};
		process::spawn(cmd.getString(),args2,true);

		listen();

	} else if (!charstring::compare(argv[1],"unix")) {

		snooze::macrosnooze(1);
		unixclient();
	}
}
