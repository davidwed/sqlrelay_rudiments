// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/unixclientsocket.h>

#include <rudiments/private/config.h>

#include <stdio.h>

int	main() {

	unixclientsocket	*clnt;
	clnt.connectToServer("/tmp/lsnr.socket",1,0);
	clnt.write("hello",5);
	char	buffer[11];
	int	sizeread=clnt.read(buffer,10);
	buffer[sizeread]=(char)NULL;
	printf("%s\n",buffer);
	clnt.close();
}
