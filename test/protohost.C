// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/hostentry.h>
#include <rudiments/protocolentry.h>

int	main() {

	hostentry	*he=new hostentry();
	he->initialize("localhost");
	he->print();
	delete he;

	protocolentry	*pe=new protocolentry();
	pe->initialize("tcp");
	pe->print();
	delete he;
}
