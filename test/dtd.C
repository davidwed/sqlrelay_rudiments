// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/dtd.h>

#include <stdio.h>

int main(int argv, const char **argc) {

	dtd	d;
	d.parseFile("dtd.dtd");
	printf("%s\n",d.xml()->getString());
}
