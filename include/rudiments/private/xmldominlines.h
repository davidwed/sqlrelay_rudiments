// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE xmldom::xmldom() : xmlsax() {
	nullnode=xmldomnode::createNullNode();
	rootnode=nullnode;
	currentparent=NULL;
	currentattribute=NULL;
}

RUDIMENTS_INLINE xmldom::~xmldom() {
	if (!rootnode->isNullNode()) {
		delete rootnode;
	}
	delete nullnode;
}

RUDIMENTS_INLINE bool xmldom::parseFile(const char *filename) {
	reset();
	return xmlsax::parseFile(filename);
}

RUDIMENTS_INLINE bool xmldom::parseString(const char *string) {
	reset();
	return xmlsax::parseFile(string);
}

RUDIMENTS_INLINE xmldomnode *xmldom::getRootNode() const {
	return (rootnode)?rootnode:nullnode;
}
