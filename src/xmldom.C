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

void xmldom::reset() {
	if (!rootnode->isNullNode()) {
		rootnode->cascadeOnDelete();
		delete rootnode;
		rootnode=nullnode;
	}
	currentparent=NULL;
	currentattribute=NULL;
}

int xmldom::writeFile(const char *filename, mode_t perms) const {
	file	fl;
	if (!fl.open(filename,O_WRONLY|O_CREAT|O_TRUNC,perms)) {
		return 0;
	}
	stringbuffer	*xml=rootnode->xml();
	int	retval=1;
	int	length=strlen(xml->getString());
	if (fl.write(xml->getString(),length)!=length) {
		retval=0;
	}
	if (!fl.close()) {
		retval=0;
	}
	return retval;
}

void xmldom::createRootNode() {
	rootnode=new xmldomnode(nullnode);
	rootnode->setName("document");
	rootnode->setType(ROOT_XMLDOMNODETYPE);
	currentparent=rootnode;
}

int xmldom::tagStart(char *name) {
	currentattribute=NULL;
	if (rootnode->isNullNode()) {
		createRootNode();
	}
	xmldomnode	*tagnode=new xmldomnode(nullnode);
	tagnode->setName(name);
	tagnode->setType(TAG_XMLDOMNODETYPE);
	currentparent->insertChild(tagnode,currentparent->getChildCount());
	currentparent=tagnode;
	return 1;
}

int xmldom::attributeName(char *name) {
	currentattribute=new xmldomnode(nullnode);
	currentattribute->setName(name);
	currentattribute->setType(ATTRIBUTE_XMLDOMNODETYPE);
	currentparent->insertAttribute(currentattribute,
				currentparent->getAttributeCount());
	return 1;
}

int xmldom::attributeValue(char *value) {
	if (!currentattribute) {
		if (!attributeName(value)) {
			return 0;
		}
	}
	currentattribute->setValue(value);
	currentattribute=NULL;
	return 1;
}

int xmldom::text(char *string) {
	currentattribute=NULL;
	xmldomnode	*textnode=new xmldomnode(nullnode);
	textnode->setName("text");
	textnode->setValue(string);
	textnode->setType(TEXT_XMLDOMNODETYPE);
	currentparent->insertChild(textnode,currentparent->getChildCount());
	return 1;
}

int xmldom::tagEnd(char *name) {
	currentattribute=NULL;
	currentparent=currentparent->getParent();
	return 1;
}

int xmldom::comment(char *string) {
	currentattribute=NULL;
	xmldomnode	*commentnode=new xmldomnode(nullnode);
	commentnode->setName("comment");
	commentnode->setValue(string);
	commentnode->setType(COMMENT_XMLDOMNODETYPE);
	currentparent->insertChild(commentnode,currentparent->getChildCount());
	return 1;
}

int xmldom::cdata(char *string) {
	currentattribute=NULL;
	xmldomnode	*cdatanode=new xmldomnode(nullnode);
	cdatanode->setName("cdata");
	cdatanode->setValue(string);
	cdatanode->setType(CDATA_XMLDOMNODETYPE);
	currentparent->insertChild(cdatanode,currentparent->getChildCount());
	return 1;
}
