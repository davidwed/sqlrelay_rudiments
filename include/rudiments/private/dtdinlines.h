// Copyright (c) 2002 David Muse
// See the COPYING file for more information

inline dtd::dtd() {
	xmld=new xmldom();
	xmldtd=new xmldom();
}

inline dtd::~dtd() {
	delete xmld;
	delete xmldtd;
}

inline int dtd::parseFile(const char *filename) {
	return (xmld->parseFile(filename) && parseDtd());
}

inline int dtd::parseString(const char *string) {
	return (xmld->parseString(string) && parseDtd());
}

inline stringbuffer *dtd::xml() const {
	return xmldtd->getRootNode()->xml();
}
