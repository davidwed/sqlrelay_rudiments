// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/xmldom.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// display the contents of xmls.xml
	xmldom	x;
	x.parseString("<xml val=\"&quot;&gt;&lt;&apos;&#200;\"/>");

	xmldomnode	*xmlnode=x.getRootNode()->getFirstTagChild("xml");
	stdoutput.printf("Attribute: %s\n",xmlnode->getAttributeValue("val"));
	stdoutput.printf("Node: %s\n",xmlnode->xml()->getString());
}
