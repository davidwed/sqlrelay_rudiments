// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

INLINE xmldom::xmldom() : xmlsax() {
	nullnode=xmldomnode::createNullNode();
	rootnode=nullnode;
	currentparent=NULL;
	currentattribute=NULL;
}

INLINE xmldom::~xmldom() {
	if (!rootnode->isNullNode()) {
		delete rootnode;
	}
	delete nullnode;
}

INLINE int xmldom::parseFile(const char *filename) {
	return xmlsax::parseFile(filename);
}

INLINE int xmldom::parseString(const char *string) {
	return xmlsax::parseFile(string);
}

INLINE xmldomnode *xmldom::getRootNode() const {
	return (rootnode)?rootnode:nullnode;
}
