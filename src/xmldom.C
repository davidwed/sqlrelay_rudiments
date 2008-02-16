// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/xmldom.h>
#include <rudiments/charstring.h>
#include <rudiments/linkedlist.h>
#include <rudiments/filesystem.h>

#include <stdlib.h>

// for getpagesize()
#include <unistd.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class xmldomprivate {
	friend class xmldom;
	private:
		xmldomnode			*_nullnode;
		xmldomnode			*_rootnode;
		xmldomnode			*_currentparent;
		xmldomnode			*_currentattribute;
		stringlist			_strlist;
		linkedlist<unsigned long>	_refcountlist;
};

xmldom::xmldom() : xmlsax() {
	pvt=new xmldomprivate;
	pvt->_nullnode=xmldomnode::createNullNode(this);
	pvt->_rootnode=pvt->_nullnode;
	pvt->_currentparent=NULL;
	pvt->_currentattribute=NULL;
}

xmldom::~xmldom() {
	if (!pvt->_rootnode->isNullNode()) {
		delete pvt->_rootnode;
	}
	delete pvt->_nullnode;
	for (stringlistnode *node=pvt->_strlist.getNodeByIndex(0);
					node; node=node->getNext()) {
		delete[] node->getData();
	}
	delete pvt;
}

bool xmldom::parseFile(const char *filename) {
	reset();
	return xmlsax::parseFile(filename);
}

bool xmldom::parseString(const char *string) {
	reset();
	return xmlsax::parseString(string);
}

void xmldom::reset() {
	if (!pvt->_rootnode->isNullNode()) {
		pvt->_rootnode->cascadeOnDelete();
		delete pvt->_rootnode;
		pvt->_rootnode=pvt->_nullnode;
	}
	pvt->_currentparent=NULL;
	pvt->_currentattribute=NULL;
}

bool xmldom::writeFile(const char *filename, mode_t perms) const {
	filesystem	fs;
	off64_t	optblocksize;
	if (fs.initialize(filename)) {
		optblocksize=fs.getOptimumTransferBlockSize();
	} else {
		optblocksize=getpagesize();
	}
	file	fl;
	if (!fl.open(filename,O_WRONLY|O_CREAT|O_TRUNC,perms)) {
		return false;
	}
	fl.setWriteBufferSize(optblocksize);
	stringbuffer	*xml=pvt->_rootnode->xml();
	bool	retval=true;
	int	length=charstring::length(xml->getString());
	if (fl.write(xml->getString(),length)!=length) {
		retval=false;
	}
	fl.flushWriteBuffer(-1,-1);
	if (!fl.close()) {
		retval=false;
	}
	return retval;
}

void xmldom::createRootNode() {
	pvt->_rootnode=new xmldomnode(this,pvt->_nullnode);
	pvt->_rootnode->setName("document");
	pvt->_rootnode->setType(ROOT_XMLDOMNODETYPE);
	pvt->_currentparent=pvt->_rootnode;
}

xmldomnode *xmldom::getRootNode() const {
	return (pvt->_rootnode)?pvt->_rootnode:pvt->_nullnode;
}

bool xmldom::tagStart(const char *name) {
	pvt->_currentattribute=NULL;
	if (pvt->_rootnode->isNullNode()) {
		createRootNode();
	}
	xmldomnode	*tagnode=new xmldomnode(this,pvt->_nullnode);
	tagnode->setName(name);
	tagnode->setType(TAG_XMLDOMNODETYPE);
	pvt->_currentparent->insertChild(tagnode,
				pvt->_currentparent->getChildCount());
	pvt->_currentparent=tagnode;
	return true;
}

bool xmldom::attributeName(const char *name) {
	pvt->_currentattribute=new xmldomnode(this,pvt->_nullnode);
	pvt->_currentattribute->setName(name);
	pvt->_currentattribute->setType(ATTRIBUTE_XMLDOMNODETYPE);
	pvt->_currentparent->insertAttribute(pvt->_currentattribute,
				pvt->_currentparent->getAttributeCount());
	return true;
}

bool xmldom::attributeValue(const char *value) {
	if (!pvt->_currentattribute) {
		if (!attributeName(value)) {
			return false;
		}
	}
	pvt->_currentattribute->setValue(value);
	pvt->_currentattribute=NULL;
	return true;
}

bool xmldom::text(const char *string) {
	pvt->_currentattribute=NULL;
	xmldomnode	*textnode=new xmldomnode(this,pvt->_nullnode);
	textnode->setName("text");
	textnode->setValue(string);
	textnode->setType(TEXT_XMLDOMNODETYPE);
	pvt->_currentparent->insertChild(textnode,
				pvt->_currentparent->getChildCount());
	return true;
}

bool xmldom::tagEnd(const char *name) {
	pvt->_currentattribute=NULL;
	pvt->_currentparent=pvt->_currentparent->getParent();
	return true;
}

bool xmldom::comment(const char *string) {
	pvt->_currentattribute=NULL;
	xmldomnode	*commentnode=new xmldomnode(this,pvt->_nullnode);
	commentnode->setName("comment");
	commentnode->setValue(string);
	commentnode->setType(COMMENT_XMLDOMNODETYPE);
	pvt->_currentparent->insertChild(commentnode,
				pvt->_currentparent->getChildCount());
	return true;
}

bool xmldom::cdata(const char *string) {
	pvt->_currentattribute=NULL;
	xmldomnode	*cdatanode=new xmldomnode(this,pvt->_nullnode);
	cdatanode->setName("cdata");
	cdatanode->setValue(string);
	cdatanode->setType(CDATA_XMLDOMNODETYPE);
	pvt->_currentparent->insertChild(cdatanode,
				pvt->_currentparent->getChildCount());
	return true;
}

const char *xmldom::cacheString(const char *string) {
	if (!string) {
		return NULL;
	}
	unsigned long	index=0;
	for (stringlistnode *node=pvt->_strlist.getNodeByIndex(0);
					node; node=node->getNext()) {
		const char	*data=node->getData();
		if (!charstring::compare(string,data)) {
			linkedlistnode<unsigned long>	*refnode=
				pvt->_refcountlist.getNodeByIndex(index);
			refnode->setData(refnode->getData()+1);
			return data;
		}
		index++;
	}
	char	*copy=charstring::duplicate(string);
	pvt->_strlist.append(copy);
	pvt->_refcountlist.append(1);
	return copy;
}

void xmldom::unCacheString(const char *string) {
	if (!string) {
		return;
	}
	unsigned long	index=0;
	for (stringlistnode *node=pvt->_strlist.getNodeByIndex(0);
					node; node=node->getNext()) {
		char	*data=node->getData();
		if (!charstring::compare(string,data)) {
			linkedlistnode<unsigned long>	*refnode=
				pvt->_refcountlist.getNodeByIndex(index);
			refnode->setData(refnode->getData()-1);
			if (refnode->getData()<=0) {
				pvt->_refcountlist.removeByIndex(index);
				pvt->_strlist.removeByIndex(index);
				delete[] data;
				return;
			}
		}
		index++;
	}
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
