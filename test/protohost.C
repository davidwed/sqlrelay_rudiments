// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/hostentry.h>
#include <rudiments/protocolentry.h>

int main(int argv, const char **argc) {

	hostentry	he;
	he.initialize("localhost");
	he.print();

	protocolentry	pe;
	pe.initialize("tcp");
	pe.print();
}
