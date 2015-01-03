// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/rpcentry.h>
#include <rudiments/stdio.h>

void print(rpcentry *re) {
	stdoutput.printf("	Name:		%s\n",re->getName());
	stdoutput.printf("	Number:		%d\n",re->getNumber());
	stdoutput.printf("	Alias list:\n");
	int	i;
	for (i=0; re->getAliasList() && re->getAliasList()[i]; i++) {
		stdoutput.printf("		%s\n",re->getAliasList()[i]);
	}
	stdoutput.printf("\n");
}

int main(int argc, const char **argv) {

	// uninitialized
	rpcentry	re;
	stdoutput.printf("uninitialized:\n");
	print(&re);

	// get the rpc information for "portmap"
	re.initialize("portmap");
	stdoutput.printf("portmap:\n");
	print(&re);

	// get the protocol information for protocol number 100000
	re.initialize(100000);
	stdoutput.printf("100000:\n");
	print(&re);

	// null-safety
	re.initialize((const char *)NULL);
	stdoutput.printf("NULL:\n");
	print(&re);
}
