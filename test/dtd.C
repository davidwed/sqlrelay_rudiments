// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/dtd.h>

#include <stdio.h>

int main(int argc, const char **argv) {

	// display the contents of dtd.dtd
	dtd	d;
	d.parseFile("dtd.dtd");
	printf("%s\n",d.xml()->getRootNode()->xml()->getString());
}
