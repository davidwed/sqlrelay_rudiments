// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/xmldom.h>

#include <stdio.h>

int main(int argv, const char **argc) {

	xmldom	x;
	x.parseFile("xmls.xml");
	stringbuffer	*xml=x.getRootNode()->xml();
	printf("%s\n",xml->getString());
	x.getRootNode()->cascadeOnDelete();
	delete xml;
}
