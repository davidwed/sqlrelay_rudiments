// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE dtd::dtd() {
	xmld=new xmldom();
	xmldtd=new xmldom();
}

RUDIMENTS_INLINE dtd::~dtd() {
	delete xmldtd;
	delete xmld;
}

RUDIMENTS_INLINE int dtd::parseFile(const char *filename) {
	if (!xmld->parseFile(filename)) {
		printf("parse xml error\n%s\n",xmld->getError());
		return 0;
	}
	if (!parseDtd()) {
		printf("parse dtd error\n%s\n",xmldtd->getError());
		return 0;
	}
	return 1;
	//return (xmld->parseFile(filename) && parseDtd());
}

RUDIMENTS_INLINE int dtd::parseString(const char *string) {
	return (xmld->parseString(string) && parseDtd());
}

RUDIMENTS_INLINE stringbuffer *dtd::xml() const {
	return xmldtd->getRootNode()->xml();
}
