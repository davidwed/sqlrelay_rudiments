// Copyright (c) 2002 David Muse
// See the COPYING file for more information

INLINE dtd::dtd() {
	xmld=new xmldom();
	xmldtd=new xmldom();
}

INLINE dtd::~dtd() {
	delete xmldtd;
	delete xmld;
}

INLINE int dtd::parseFile(const char *filename) {
	return (xmld->parseFile(filename) && parseDtd());
}

INLINE int dtd::parseString(const char *string) {
	return (xmld->parseString(string) && parseDtd());
}

INLINE stringbuffer *dtd::xml() const {
	return xmldtd->getRootNode()->xml();
}
