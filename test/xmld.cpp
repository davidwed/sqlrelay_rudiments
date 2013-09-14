// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/xmldom.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// display the contents of xmls.xml
	xmldom	x;
	x.parseFile("xmls.xml");

	xmldomnode	*testnode=x.getRootNode()->getFirstTagChild("test");
	testnode->deleteChild(1);

	stringbuffer	*xml=x.getRootNode()->xml();
	stdoutput.printf("%s\n",xml->getString());
	x.getRootNode()->cascadeOnDelete();
	delete xml;

	x.writeFile("test.xml",permissions::evalPermString("rw-r--r--"));
}
