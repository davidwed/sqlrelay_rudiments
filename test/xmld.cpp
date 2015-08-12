// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/xmldom.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// display the contents of xmls.xml
	xmldom	x;
	if (!x.parseFile("xmls.xml")) {
		stdoutput.printf("failed to parse xmls.xml\n");
	}

	xmldomnode	*testnode=x.getRootNode()->getFirstTagChild("test");
	testnode->deleteChild(1);

	stringbuffer	*xml=x.getRootNode()->xml();
	stdoutput.printf("%s\n",xml->getString());
	x.getRootNode()->cascadeOnDelete();
	delete xml;

	x.writeFile("test.xml",permissions::evalPermString("rw-r--r--"));

	stdoutput.printf("\n\n===============================\n\n");

	x.parseString("<xml>\n<tag1/><tag2/></xml>");
	x.getRootNode()->getFirstTagChild("xml")->
				getFirstTagChild("tag1")->
				appendXml("<tag11/><tag12/>");
	x.getRootNode()->getFirstTagChild("xml")->
				getFirstTagChild("tag2")->
				appendXml("<tag21/><tag22/>");
	x.getRootNode()->print(&stdoutput);
}
