// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/xmldom.h>
#include <rudiments/charstring.h>
#include <rudiments/linkedlist.h>
#include <rudiments/filesystem.h>
#include <rudiments/sys.h>

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

// for GetSystemInfo
#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif

class xmldomprivate {
	friend class xmldom;
	private:
		xmldomnode		*_nullnode;
		xmldomnode		*_rootnode;
		xmldomnode		*_currentparent;
		xmldomnode		*_currentattribute;
		bool			_stringcacheenabled;
		linkedlist< char * >	_strlist;
		linkedlist< uint64_t >	_refcountlist;
};

xmldom::xmldom() : xmlsax() {
	init(true);
}


xmldom::xmldom(bool stringcacheenabled) : xmlsax() {
	init(stringcacheenabled);
}

void xmldom::init(bool stringcacheenabled) {
	pvt=new xmldomprivate;
	pvt->_nullnode=xmldomnode::createNullNode(this);
	pvt->_rootnode=pvt->_nullnode;
	pvt->_currentparent=NULL;
	pvt->_currentattribute=NULL;
	pvt->_stringcacheenabled=stringcacheenabled;
}

xmldom::~xmldom() {
	if (!pvt->_rootnode->isNullNode()) {
		delete pvt->_rootnode;
	}
	delete pvt->_nullnode;
	for (linkedlistnode< char * > *node=pvt->_strlist.getFirst();
						node; node=node->getNext()) {
		delete[] node->getValue();
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

bool xmldom::stringCacheEnabled() {
	return pvt->_stringcacheenabled;
}

bool xmldom::writeFile(const char *filename, mode_t perms) const {
	filesystem	fs;
	off64_t	optblocksize;
	if (fs.initialize(filename)) {
		optblocksize=fs.getOptimumTransferBlockSize();
	} else {
		#if defined(RUDIMENTS_HAVE_GETSYSTEMINFO)
			// On windows, sys::getPageSize returns
			// dwAllocationGranularity, not dwPageSize,
			// so call GetSystemInfo directly here in
			// that case.
			SYSTEM_INFO	systeminfo;
			GetSystemInfo(&systeminfo);
			optblocksize=systeminfo.dwPageSize;
		#else
			optblocksize=sys::getPageSize();
		#endif
	}
	file	fl;
	if (!fl.open(filename,O_WRONLY|O_CREAT|O_TRUNC,perms)) {
		return false;
	}
	fl.setWriteBufferSize(optblocksize);
	stringbuffer	*xml=pvt->_rootnode->xml();
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
	pvt->_rootnode=new xmldomnode(this,pvt->_nullnode);
	pvt->_rootnode->setName("document");
	pvt->_rootnode->setType(ROOT_XMLDOMNODETYPE);
	pvt->_currentparent=pvt->_rootnode;
}

xmldomnode *xmldom::getRootNode() const {
	return (pvt->_rootnode)?pvt->_rootnode:pvt->_nullnode;
}

xmldomnode *xmldom::getNullNode() const {
	return pvt->_nullnode;
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
	linkedlistnode< char * > 	*strnode=pvt->_strlist.getFirst();
	linkedlistnode< uint64_t >	*refnode=pvt->_refcountlist.getFirst();
	while (strnode) {
		const char	*data=strnode->getValue();
		if (!charstring::compare(string,data)) {
			refnode->setValue(refnode->getValue()+1);
			return data;
		}
		strnode=strnode->getNext();
		refnode=refnode->getNext();
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
	linkedlistnode< char * > 	*strnode=pvt->_strlist.getFirst();
	linkedlistnode< uint64_t >	*refnode=pvt->_refcountlist.getFirst();
	while (strnode) {
		char	*data=strnode->getValue();
		if (!charstring::compare(string,data)) {
			refnode->setValue(refnode->getValue()-1);
			if (!refnode->getValue()) {
				pvt->_refcountlist.remove(refnode);
				pvt->_strlist.remove(strnode);
				delete[] data;
				return;
			}
		}
		strnode=strnode->getNext();
		refnode=refnode->getNext();
	}
}
