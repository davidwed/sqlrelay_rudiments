// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/xmldom.h>
#ifndef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/xmldominlines.h>
#endif

#include <sys/stat.h>
#include <fcntl.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

bool xmldom::parseFile(const char *filename) {
	reset();
	return xmlsax::parseFile(filename);
}

bool xmldom::parseString(const char *string) {
	reset();
	return xmlsax::parseFile(string);
}

void xmldom::reset() {
	if (!rootnode->isNullNode()) {
		rootnode->cascadeOnDelete();
		delete rootnode;
		rootnode=nullnode;
	}
	currentparent=NULL;
	currentattribute=NULL;
}

bool xmldom::writeFile(const char *filename, mode_t perms) const {
	file	fl;
	if (!fl.open(filename,O_WRONLY|O_CREAT|O_TRUNC,perms)) {
		return false;
	}
	stringbuffer	*xml=rootnode->xml();
	bool	retval=true;
	int	length=strlen(xml->getString());
	if (fl.write(xml->getString(),length)!=length) {
		retval=false;
	}
	if (!fl.close()) {
		retval=false;
	}
	return retval;
}

void xmldom::createRootNode() {
	rootnode=new xmldomnode(nullnode);
	rootnode->setName("document");
	rootnode->setType(ROOT_XMLDOMNODETYPE);
	currentparent=rootnode;
}

bool xmldom::tagStart(char *name) {
	currentattribute=NULL;
	if (rootnode->isNullNode()) {
		createRootNode();
	}
	xmldomnode	*tagnode=new xmldomnode(nullnode);
	tagnode->setName(name);
	tagnode->setType(TAG_XMLDOMNODETYPE);
	currentparent->insertChild(tagnode,currentparent->getChildCount());
	currentparent=tagnode;
	return true;
}

bool xmldom::attributeName(char *name) {
	currentattribute=new xmldomnode(nullnode);
	currentattribute->setName(name);
	currentattribute->setType(ATTRIBUTE_XMLDOMNODETYPE);
	currentparent->insertAttribute(currentattribute,
				currentparent->getAttributeCount());
	return true;
}

bool xmldom::attributeValue(char *value) {
	if (!currentattribute) {
		if (!attributeName(value)) {
			return false;
		}
	}
	currentattribute->setValue(value);
	currentattribute=NULL;
	return true;
}

bool xmldom::text(char *string) {
	currentattribute=NULL;
	xmldomnode	*textnode=new xmldomnode(nullnode);
	textnode->setName("text");
	textnode->setValue(string);
	textnode->setType(TEXT_XMLDOMNODETYPE);
	currentparent->insertChild(textnode,currentparent->getChildCount());
	return true;
}

bool xmldom::tagEnd(char *name) {
	currentattribute=NULL;
	currentparent=currentparent->getParent();
	return true;
}

bool xmldom::comment(char *string) {
	currentattribute=NULL;
	xmldomnode	*commentnode=new xmldomnode(nullnode);
	commentnode->setName("comment");
	commentnode->setValue(string);
	commentnode->setType(COMMENT_XMLDOMNODETYPE);
	currentparent->insertChild(commentnode,currentparent->getChildCount());
	return true;
}

bool xmldom::cdata(char *string) {
	currentattribute=NULL;
	xmldomnode	*cdatanode=new xmldomnode(nullnode);
	cdatanode->setName("cdata");
	cdatanode->setValue(string);
	cdatanode->setType(CDATA_XMLDOMNODETYPE);
	currentparent->insertChild(cdatanode,currentparent->getChildCount());
	return true;
}
