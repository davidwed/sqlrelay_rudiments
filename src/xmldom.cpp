// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/xmldom.h>
#include <rudiments/charstring.h>
#include <rudiments/linkedlist.h>
#include <rudiments/filesystem.h>
#include <rudiments/sys.h>

// FIXME: what needs this?
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

class xmldomprivate {
	friend class xmldom;
	private:
		xmldomnode		*_nullnode;
		xmldomnode		*_rootnode;
		xmldomnode		*_currentparent;
		xmldomnode		*_currentattribute;
		bool			_stringcacheenabled;
		dictionary< char *, uint64_t >	_strcache;

		xmldomnode		*_top;
		uint64_t		_topposition;
};

xmldom::xmldom() : xmlsax() {
	init(true);
}

xmldom::xmldom(bool stringcacheenabled) : xmlsax() {
	init(stringcacheenabled);
}

xmldom::xmldom(const xmldom &x) : xmlsax() {
	init(x.pvt->_stringcacheenabled);
	pvt->_rootnode=x.pvt->_rootnode->clone(this);
}

xmldom &xmldom::operator=(const xmldom &x) {
	if (this!=&x) {
		reset();
		pvt->_rootnode=x.pvt->_rootnode->clone(this);
	}
	return *this;
}

void xmldom::init(bool stringcacheenabled) {
	pvt=new xmldomprivate;
	pvt->_nullnode=xmldomnode::createNullNode(this);
	pvt->_rootnode=pvt->_nullnode;
	pvt->_currentparent=NULL;
	pvt->_currentattribute=NULL;
	pvt->_stringcacheenabled=stringcacheenabled;
	pvt->_top=NULL;
	pvt->_topposition=0;
}

xmldom::~xmldom() {
	if (!pvt->_rootnode->isNullNode()) {
		delete pvt->_rootnode;
	}
	delete pvt->_nullnode;
	for (linkedlistnode< dictionarynode< char *, uint64_t> *>
				*node=pvt->_strcache.getList()->getFirst();
				node; node=node->getNext()) {
		delete[] node->getValue()->getKey();
	}
	delete pvt;
}

bool xmldom::parseFile(const char *filename) {
	return parseFile(filename,NULL,0);
}

bool xmldom::parseFile(const char *filename,
				xmldomnode *parent, uint64_t position) {
	if (parent) {
		pvt->_currentparent=parent;
		pvt->_currentattribute=NULL;
		pvt->_top=parent;
		pvt->_topposition=position;
	} else {
		reset();
	}
	return xmlsax::parseFile(filename);
}

bool xmldom::parseString(const char *string) {
	return parseString(string,NULL,0);
}

bool xmldom::parseString(const char *string,
				xmldomnode *parent, uint64_t position) {
	if (parent) {
		pvt->_currentparent=parent;
		pvt->_currentattribute=NULL;
		pvt->_top=parent;
		pvt->_topposition=position;
	} else {
		reset();
	}
	return xmlsax::parseString(string);
}

void xmldom::reset() {
	if (!pvt->_rootnode->isNullNode()) {
		pvt->_rootnode->cascadeOnDelete();
		delete pvt->_rootnode;
		pvt->_rootnode=pvt->_nullnode;
	}
	for (linkedlistnode< dictionarynode< char *, uint64_t> *>
				*node=pvt->_strcache.getList()->getFirst();
				node; node=node->getNext()) {
		delete[] node->getValue()->getKey();
	}
	pvt->_strcache.clear();
	pvt->_currentparent=NULL;
	pvt->_currentattribute=NULL;
	pvt->_top=NULL;
	pvt->_topposition=0;
}

bool xmldom::stringCacheEnabled() {
	return pvt->_stringcacheenabled;
}

bool xmldom::writeFile(const char *filename, mode_t perms) const {
	filesystem	fs;
	off64_t	optblocksize;
	if (fs.initialize(filename)) {
		optblocksize=fs.getOptimumTransferBlockSize();
	} else {
		optblocksize=sys::getPageSize();
	}
	file	fl;
	if (!fl.open(filename,O_WRONLY|O_CREAT|O_TRUNC,perms)) {
		return false;
	}
	fl.setWriteBufferSize(optblocksize);
	stringbuffer	*xml=pvt->_rootnode->xml();
	xml->append('\n');
	bool	retval=true;
	ssize_t	length=charstring::length(xml->getString());
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
	pvt->_rootnode=new xmldomnode(this,pvt->_nullnode,
					ROOT_XMLDOMNODETYPE,
					NULL,"document",NULL);
	pvt->_currentparent=pvt->_rootnode;
}

xmldomnode *xmldom::getRootNode() const {
	return (pvt->_rootnode)?pvt->_rootnode:pvt->_nullnode;
}

xmldomnode *xmldom::getNullNode() const {
	return pvt->_nullnode;
}

bool xmldom::tagStart(const char *ns, const char *name) {
	pvt->_currentattribute=NULL;
	if (pvt->_rootnode->isNullNode()) {
		createRootNode();
	}
	xmldomnode	*tagnode=new xmldomnode(this,pvt->_nullnode,
						TAG_XMLDOMNODETYPE,
						ns,name,NULL);
	insertChild(tagnode);
	pvt->_currentparent=tagnode;
	return true;
}

bool xmldom::attributeName(const char *name) {
	pvt->_currentattribute=new xmldomnode(this,pvt->_nullnode,
						ATTRIBUTE_XMLDOMNODETYPE,
						NULL,name,NULL);
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
	insertChild(new xmldomnode(this,pvt->_nullnode,
					TEXT_XMLDOMNODETYPE,
					NULL,"text",string));
	return true;
}

bool xmldom::tagEnd(const char *ns, const char *name) {
	pvt->_currentattribute=NULL;
	pvt->_currentparent=pvt->_currentparent->getParent();
	return true;
}

bool xmldom::comment(const char *string) {
	pvt->_currentattribute=NULL;
	insertChild(new xmldomnode(this,pvt->_nullnode,
					COMMENT_XMLDOMNODETYPE,
					NULL,"comment",string));
	return true;
}

bool xmldom::cdata(const char *string) {
	pvt->_currentattribute=NULL;
	insertChild(new xmldomnode(this,pvt->_nullnode,
					CDATA_XMLDOMNODETYPE,
					NULL,"cdata",string));
	return true;
}

const char *xmldom::cacheString(const char *string) {
	if (!string) {
		return NULL;
	}
	dictionarynode< char *, uint64_t > 	*node=
				pvt->_strcache.getNode((char *)string);
	if (node) {
		node->setValue(node->getValue()+1);
		return node->getKey();
	}
	char	*copy=charstring::duplicate(string);
	pvt->_strcache.setValue(copy,1);
	return copy;
}

void xmldom::unCacheString(const char *string) {
	if (!string) {
		return;
	}
	dictionarynode< char *, uint64_t > 	*node=
				pvt->_strcache.getNode((char *)string);
	if (node) {
		node->setValue(node->getValue()-1);
		if (!node->getValue()) {
			char	*data=node->getKey();
			pvt->_strcache.remove(node);
			delete[] data;
		}
	}
}

void xmldom::insertChild(xmldomnode *child) {
	uint64_t	pos=pvt->_currentparent->getChildCount();
	if (pvt->_currentparent==pvt->_top) {
		pos=pvt->_topposition;
		pvt->_topposition++;
	}
	pvt->_currentparent->insertChild(child,pos);
}
