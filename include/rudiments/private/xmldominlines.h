// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

inline xmldom::xmldom() : xmlsax() {
	nullnode=xmldomnode::createNullNode();
	rootnode=nullnode;
	currentparent=NULL;
	currentattribute=NULL;
}

inline xmldom::~xmldom() {
	if (!rootnode->isNullNode()) {
		delete rootnode;
	}
	delete nullnode;
}

inline int xmldom::parseFile(const char *filename) {
	return xmlsax::parseFile(filename);
}

inline int xmldom::parseString(const char *string) {
	return xmlsax::parseFile(string);
}

inline xmldomnode *xmldom::getRootNode() const {
	return (rootnode)?rootnode:nullnode;
}
