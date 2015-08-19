// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/xmldomnode.h>
#include <rudiments/charstring.h>
#include <rudiments/xmldom.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

class xmldomnodeprivate {
	friend class xmldomnode;
	private:
		xmldom		*_dom;
		bool		_cascade;
		xmldomnodetype	_type;
		const char	*_namespace;
		const char	*_name;
		const char	*_value;
		xmldomnode	*_parent;
		xmldomnode	*_next;
		xmldomnode	*_nexttag;
		xmldomnode	*_previous;
		uint64_t	_childcount;
		xmldomnode	*_firstchild;
		xmldomnode	*_firsttagchild;
		xmldomnode	*_lastchild;
		uint64_t	_attributecount;
		xmldomnode	*_firstattribute;
		xmldomnode	*_lastattribute;
		xmldomnode	*_nullnode;
		bool		_isnullnode;
		void		*_data;
};

xmldomnode::xmldomnode(xmldom *dom, xmldomnode *nullnode) {
	init(nullnode);
	pvt->_dom=dom;
	pvt->_type=NULL_XMLDOMNODETYPE;
}

xmldomnode::xmldomnode(xmldom *dom,
			xmldomnode *nullnode,
			xmldomnodetype type,
			const char *ns,
			const char *name,
			const char *value) {
	init(nullnode);
	pvt->_dom=dom;
	pvt->_type=type;
	setNamespace(ns);
	setName(name);
	setValue(value);
}

void xmldomnode::init(xmldomnode *nullnode) {
	pvt=new xmldomnodeprivate;
	pvt->_nullnode=nullnode;
	pvt->_parent=nullnode;
	pvt->_next=nullnode;
	pvt->_nexttag=nullnode;
	pvt->_previous=nullnode;
	pvt->_firstchild=nullnode;
	pvt->_firsttagchild=nullnode;
	pvt->_lastchild=nullnode;
	pvt->_childcount=0;
	pvt->_firstattribute=NULL;
	pvt->_lastattribute=NULL;
	pvt->_attributecount=0;
	pvt->_cascade=true;
	pvt->_isnullnode=false;
	pvt->_namespace=NULL;
	pvt->_name=NULL;
	pvt->_value=NULL;
	pvt->_data=NULL;
}

xmldomnode::~xmldomnode() {
	xmldomnode	*current;
	if (pvt->_cascade) {
		// delete child nodes
		current=pvt->_firstchild;
		while (current && !current->isNullNode()) {
			pvt->_lastchild=current->pvt->_next;
			delete current;
			current=pvt->_lastchild;
		}
	}
	// delete attributes
	current=pvt->_firstattribute;
	while (current && !current->isNullNode()) {
		pvt->_lastattribute=current->pvt->_next;
		delete current;
		current=pvt->_lastattribute;
	}
	if (pvt->_dom->stringCacheEnabled()) {
		pvt->_dom->unCacheString(pvt->_name);
		pvt->_dom->unCacheString(pvt->_value);
	} else {
		delete[] (char *)pvt->_namespace;
		delete[] (char *)pvt->_name;
		delete[] (char *)pvt->_value;
	}
	delete pvt;
}

xmldomnode *xmldomnode::createNullNode(xmldom *dom) {
	xmldomnode	*nn=new xmldomnode(dom,NULL);
	nn->pvt->_nullnode=nn;
	nn->pvt->_parent=nn;
	nn->pvt->_next=nn;
	nn->pvt->_nexttag=nn;
	nn->pvt->_previous=nn;
	nn->pvt->_firstchild=nn;
	nn->pvt->_firsttagchild=nn;
	nn->pvt->_lastchild=nn;
	nn->pvt->_isnullnode=true;
	return nn;
}

xmldomnode *xmldomnode::getPreviousTagSibling() const {
	xmldomnode	*node=getPreviousSibling();
	while (!node->isNullNode() && node->getType()!=TAG_XMLDOMNODETYPE) {
		node=node->getPreviousSibling();
	}
	return node;
}

xmldomnode *xmldomnode::getPreviousTagSibling(const char *name) const {
	return getPreviousTagSibling(NULL,name,false);
}

xmldomnode *xmldomnode::getPreviousTagSibling(const char *ns,
						const char *name) const {
	return getPreviousTagSibling(ns,name,false);
}

xmldomnode *xmldomnode::getPreviousTagSiblingIgnoringCase(
						const char *name) const {
	return getPreviousTagSibling(NULL,name,true);
}

xmldomnode *xmldomnode::getPreviousTagSiblingIgnoringCase(
						const char *ns,
						const char *name) const {
	return getPreviousTagSibling(ns,name,true);
}

xmldomnode *xmldomnode::getPreviousTagSibling(
					const char *ns,
					const char *name,
					bool ignorecase) const {
	for (xmldomnode *current=getPreviousTagSibling();
			current && !current->isNullNode();
			current=current->getPreviousTagSibling()) {
		if (match(current,ns,name,ignorecase)) {
			return current;
		}
	}
	return pvt->_nullnode;
}

xmldomnode *xmldomnode::getPreviousTagSibling(
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getPreviousTagSibling(NULL,name,
					attributename,attributevalue,false);
}

xmldomnode *xmldomnode::getPreviousTagSibling(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getPreviousTagSibling(ns,name,
					attributename,attributevalue,false);
}

xmldomnode *xmldomnode::getPreviousTagSiblingIgnoringCase(
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getPreviousTagSibling(NULL,name,
					attributename,attributevalue,true);
}

xmldomnode *xmldomnode::getPreviousTagSiblingIgnoringCase(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getPreviousTagSibling(ns,name,
					attributename,attributevalue,true);
}

xmldomnode *xmldomnode::getPreviousTagSibling(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const {
	for (xmldomnode *current=getPreviousTagSibling();
			current && !current->isNullNode();
			current=current->getPreviousTagSibling()) {
		if (match(current,ns,name,ignorecase)) {
			const char	*value=current->getAttribute(
							attributename,
							ignorecase)->getValue();
			if (value &&
				((ignorecase)?
					!charstring::compareIgnoringCase(
							value,attributevalue):
					!charstring::compare(
							value,attributevalue))
				) {
				return current;
			}
		}
	}
	return pvt->_nullnode;
}

xmldomnode *xmldomnode::getNextTagSibling() const {
	if (pvt->_type==TAG_XMLDOMNODETYPE) {
		return pvt->_nexttag;
	}
	xmldomnode	*node=getNextSibling();
	while (!node->isNullNode() && node->getType()!=TAG_XMLDOMNODETYPE) {
		node=node->getNextSibling();
	}
	return node;
}

xmldomnode *xmldomnode::getNextTagSibling(const char *name) const {
	return getNextTagSibling(NULL,name,false);
}

xmldomnode *xmldomnode::getNextTagSibling(const char *ns,
					const char *name) const {
	return getNextTagSibling(ns,name,false);
}

xmldomnode *xmldomnode::getNextTagSiblingIgnoringCase(
					const char *name) const {
	return getNextTagSibling(NULL,name,true);
}

xmldomnode *xmldomnode::getNextTagSiblingIgnoringCase(
					const char *ns,
					const char *name) const {
	return getNextTagSibling(ns,name,true);
}

xmldomnode *xmldomnode::getNextTagSibling(const char *ns,
						const char *name,
						bool ignorecase) const {
	for (xmldomnode *current=getNextTagSibling();
			current && !current->isNullNode();
			current=current->getNextTagSibling()) {
		if (match(current,ns,name,ignorecase)) {
			return current;
		}
	}
	return pvt->_nullnode;
}

xmldomnode *xmldomnode::getNextTagSibling(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getNextTagSibling(NULL,name,
				attributename,attributevalue,false);
}

xmldomnode *xmldomnode::getNextTagSibling(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getNextTagSibling(ns,name,
				attributename,attributevalue,false);
}

xmldomnode *xmldomnode::getNextTagSiblingIgnoringCase(
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getNextTagSibling(NULL,name,
				attributename,attributevalue,true);
}

xmldomnode *xmldomnode::getNextTagSiblingIgnoringCase(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getNextTagSibling(ns,name,
				attributename,attributevalue,true);
}

xmldomnode *xmldomnode::getNextTagSibling(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const {
	for (xmldomnode *current=getNextTagSibling();
			current && !current->isNullNode();
			current=current->getNextTagSibling()) {
		if (match(current,ns,name,ignorecase)) {
			const char	*value=current->getAttribute(
							attributename,
							ignorecase)->getValue();
			if (value &&
				((ignorecase)?
					!charstring::compareIgnoringCase(
							value,attributevalue):
					!charstring::compare(
							value,attributevalue))
				) {
				return current;
			}
		}
	}
	return pvt->_nullnode;
}

xmldomnode *xmldomnode::getNextTagSiblingInSet(const char * const *set) const {
	return getNextTagSiblingInSet(NULL,set);
}

xmldomnode *xmldomnode::getNextTagSiblingInSet(const char *ns,
						const char * const *set) const {
	for (xmldomnode *child=getNextTagSibling();
			!child->isNullNode(); 
			child=child->getNextTagSibling()) {
		if (match(child,ns,set)) {
			return child;
		}
	}
	return pvt->_nullnode;
}

xmldomnode *xmldomnode::getFirstTagChild() const {
	return pvt->_firsttagchild;
}

xmldomnode *xmldomnode::getFirstTagChild(const char *name) const {
	return getFirstTagChild(NULL,name,false);
}

xmldomnode *xmldomnode::getFirstTagChild(const char *ns,
						const char *name) const {
	return getFirstTagChild(ns,name,false);
}

xmldomnode *xmldomnode::getFirstTagChildIgnoringCase(const char *name) const {
	return getFirstTagChild(NULL,name,true);
}

xmldomnode *xmldomnode::getFirstTagChildIgnoringCase(const char *ns,
						const char *name) const {
	return getFirstTagChild(ns,name,true);
}

xmldomnode *xmldomnode::getFirstTagChild(const char *ns,
						const char *name,
						bool ignorecase) const {
	xmldomnode	*node=pvt->_firsttagchild;
	if (node->isNullNode() || match(node,ns,name,ignorecase)) {
		return node;
	}
	return node->getNextTagSibling(ns,name,ignorecase);
}

xmldomnode *xmldomnode::getFirstTagChild(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstTagChild(NULL,name,attributename,attributevalue,false);
}

xmldomnode *xmldomnode::getFirstTagChild(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstTagChild(ns,name,attributename,attributevalue,false);
}

xmldomnode *xmldomnode::getFirstTagChildIgnoringCase(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstTagChild(NULL,name,attributename,attributevalue,true);
}

xmldomnode *xmldomnode::getFirstTagChildIgnoringCase(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstTagChild(ns,name,attributename,attributevalue,true);
}

xmldomnode *xmldomnode::getFirstTagChild(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const {
	xmldomnode	*node=getFirstChild(ns,name,attributename,
						attributevalue,ignorecase);
	return (node->isNullNode() || node->getType()==TAG_XMLDOMNODETYPE)?
		node:
		node->getNextTagSibling(ns,name,attributename,
					attributevalue,ignorecase);
}

xmldomnode *xmldomnode::getFirstTagChildInSet(const char * const *set) const {
	return getFirstTagChildInSet(NULL,set);
}

xmldomnode *xmldomnode::getFirstTagChildInSet(const char *ns,
						const char * const *set) const {
	xmldomnode	*child=getFirstTagChild();
	if (match(child,ns,set)) {
		return child;
	}
	return child->getNextTagSiblingInSet(ns,set);
}

xmldomnode *xmldomnode::getFirstChild(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstChild(NULL,name,attributename,attributevalue,false);
}

xmldomnode *xmldomnode::getFirstChild(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstChild(ns,name,attributename,attributevalue,false);
}

xmldomnode *xmldomnode::getFirstChildIgnoringCase(
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstChild(NULL,name,attributename,attributevalue,true);
}

xmldomnode *xmldomnode::getFirstChildIgnoringCase(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstChild(ns,name,attributename,attributevalue,true);
}

xmldomnode *xmldomnode::getFirstChild(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const {
	for (xmldomnode *current=pvt->_firstchild;
			!current->isNullNode();
			current=current->pvt->_next) {
		if (match(current,ns,name,ignorecase)) {
			const char	*value=current->getAttribute(
							attributename,
							ignorecase)->getValue();
			if (value &&
				((ignorecase)?
					!charstring::compareIgnoringCase(
							value,attributevalue):
					!charstring::compare(
							value,attributevalue))
				) {
				return current;
			}
		}
	}
	return pvt->_nullnode;
}

xmldomnode *xmldomnode::getFirstTagDescendent(const char *name) const {
	return getFirstTagDescendent(NULL,name,false);
}

xmldomnode *xmldomnode::getFirstTagDescendent(const char *ns,
						const char *name) const {
	return getFirstTagDescendent(ns,name,false);
}

xmldomnode *xmldomnode::getFirstTagDescendentIgnoringCase(
						const char *name) const {
	return getFirstTagDescendent(NULL,name,true);
}

xmldomnode *xmldomnode::getFirstTagDescendentIgnoringCase(
						const char *ns,
						const char *name) const {
	return getFirstTagDescendent(ns,name,true);
}

xmldomnode *xmldomnode::getFirstTagDescendent(const char *ns,
						const char *name,
						bool ignorecase) const {
	for (xmldomnode	*child=getFirstTagChild();
			!child->isNullNode();
			child=child->getNextTagSibling()) {

		if (match(child,ns,name,ignorecase)) {
			return child;
		}

		xmldomnode	*desc=
			child->getFirstTagDescendent(ns,name,ignorecase);
		if (!desc->isNullNode()) {
			return desc;
		}
	}
	return pvt->_nullnode;
}

xmldomnode *xmldomnode::getFirstTagDescendent(
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstTagDescendent(NULL,name,
					attributename,attributevalue,false);
}

xmldomnode *xmldomnode::getFirstTagDescendent(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstTagDescendent(ns,name,
					attributename,attributevalue,false);
}

xmldomnode *xmldomnode::getFirstTagDescendentIgnoringCase(
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstTagDescendent(NULL,name,
					attributename,attributevalue,true);
}

xmldomnode *xmldomnode::getFirstTagDescendentIgnoringCase(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue) const {
	return getFirstTagDescendent(ns,name,
					attributename,attributevalue,true);
}

xmldomnode *xmldomnode::getFirstTagDescendent(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const {

	for (xmldomnode	*child=getFirstTagChild();
			!child->isNullNode();
			child=child->getNextTagSibling()) {

		if (match(child,ns,name,ignorecase) &&
			((ignorecase)?
			!charstring::compareIgnoringCase(
					child->getAttributeValue(attributename),
					attributevalue):
			!charstring::compare(
					child->getAttributeValue(attributename),
					attributevalue))) {
			return child;
		}

		xmldomnode	*desc=child->getFirstTagDescendent(ns,name,
						attributename,attributevalue,
						ignorecase);
		if (!desc->isNullNode()) {
			return desc;
		}
	}
	return pvt->_nullnode;
}

xmldomnode *xmldomnode::getFirstTagDescendentInSet(
					const char * const *set) const {
	return getFirstTagDescendentInSet(NULL,set);
}

xmldomnode *xmldomnode::getFirstTagDescendentInSet(
					const char *ns,
					const char * const *set) const {
	for (xmldomnode	*child=getFirstTagChild();
			!child->isNullNode();
			child=child->getNextTagSibling()) {

		if (match(child,ns,set)) {
			return child;
		}

		xmldomnode	*desc=child->getFirstTagDescendentInSet(ns,set);
		if (!desc->isNullNode()) {
			return desc;
		}
	}
	return pvt->_nullnode;
}

xmldomnode *xmldomnode::getNextTag() const {
	return getNextTag(NULL);
}

xmldomnode *xmldomnode::getNextTag(xmldomnode *top) const {

	xmldomnode	*ftc=getFirstTagChild();
	if (!ftc->isNullNode()) {
		return ftc;
	}
	xmldomnode	*nts=getNextTagSibling();
	if (!nts->isNullNode()) {
		return nts;
	}
	const xmldomnode	*ancestor=this;
	for (;;) {
		ancestor=ancestor->getParent();
		if (ancestor->isNullNode() || ancestor==top) {
			return pvt->_nullnode;
		}
		xmldomnode	*ancestorsib=ancestor->getNextTagSibling();
		if (!ancestorsib->isNullNode()) {
			return ancestorsib;
		}
	}
}

xmldomnode *xmldomnode::getPreviousTag() const {
	return getPreviousTag(NULL);
}

xmldomnode *xmldomnode::getPreviousTag(xmldomnode *top) const {

	xmldomnode	*nts=getPreviousTagSibling();
	if (!nts->isNullNode()) {
		return nts;
	}
	const xmldomnode	*ancestor=this;
	for (;;) {
		ancestor=ancestor->getParent();
		if (ancestor->isNullNode() || ancestor==top) {
			return pvt->_nullnode;
		}
		xmldomnode	*ancestorsib=ancestor->getPreviousTagSibling();
		if (!ancestorsib->isNullNode()) {
			return ancestorsib;
		}
	}
}

bool xmldomnode::insertText(const char *value, uint64_t position) {
	xmldomnode	*text=new xmldomnode(pvt->_dom,pvt->_nullnode);
	text->setName("text");
	text->setValue(value);
	return insertNode(text,position,
				TEXT_XMLDOMNODETYPE,
				&pvt->_firstchild,
				&pvt->_lastchild,
				&pvt->_childcount);
}

bool xmldomnode::insertAttribute(const char *name, const char *value,
							uint64_t position) {
	xmldomnode	*attribute=new xmldomnode(pvt->_dom,pvt->_nullnode);
	attribute->setName(name);
	attribute->setValue(value);
	return insertNode(attribute,position,
				ATTRIBUTE_XMLDOMNODETYPE,
				&pvt->_firstattribute,
				&pvt->_lastattribute,
				&pvt->_attributecount);
}

void xmldomnode::xml(stringbuffer *strb,
			filedescriptor *fd,
			bool indent,
			uint16_t *indentlevel) const {

	xmldomnode	*current;
	if (pvt->_type==ROOT_XMLDOMNODETYPE) {
		current=pvt->_firstchild;
		for (uint64_t i=0; i<pvt->_childcount; i++) {
			current->xml(strb,fd,indent,indentlevel);
			current=current->pvt->_next;
		}
	} else if (pvt->_type==TAG_XMLDOMNODETYPE) {
		if (indent && indentlevel) {
			for (uint16_t i=0; i<*indentlevel; i++) {
				append(strb,fd," ");
			}
		}
		append(strb,fd,"<");
		safeAppend(strb,fd,pvt->_name);
		current=pvt->_firstattribute;
		for (uint64_t i=0; i<pvt->_attributecount; i++) {
			append(strb,fd," ");
			current->xml(strb,fd,indent,indentlevel);
			current=current->pvt->_next;
		}
		if (pvt->_childcount) {
			append(strb,fd,">");
			if (indent && indentlevel) {
				if (pvt->_firstchild->getType()!=
						TEXT_XMLDOMNODETYPE &&
					pvt->_firstchild->getType()!=
						CDATA_XMLDOMNODETYPE) {
					append(strb,fd,"\n");
				}
				*indentlevel=*indentlevel+2;
			}
			current=pvt->_firstchild;
			for (uint64_t i=0; i<pvt->_childcount; i++) {
				current->xml(strb,fd,indent,indentlevel);
				current=current->pvt->_next;
			}
			if (indent && indentlevel) {
				*indentlevel=*indentlevel-2;
				if (pvt->_lastchild->getType()!=
						TEXT_XMLDOMNODETYPE &&
					pvt->_lastchild->getType()!=
						CDATA_XMLDOMNODETYPE) {
					for (uint16_t i=0;
						i<*indentlevel; i++) {
						append(strb,fd," ");
					}
				}
			}
			append(strb,fd,"</");
			safeAppend(strb,fd,pvt->_name);
			append(strb,fd,">");
			if (indent && indentlevel) {
				append(strb,fd,"\n");
			}
		} else {
			if (pvt->_name[0]=='?') {
				append(strb,fd,"?>");
			} else if (pvt->_name[0]=='!') {
				append(strb,fd,">");
			} else {
				append(strb,fd,"/>");
			}
			if (indent && indentlevel) {
				append(strb,fd,"\n");
			}
		}
	} else if (pvt->_type==TEXT_XMLDOMNODETYPE) {
		safeAppend(strb,fd,pvt->_value);
	} else if (pvt->_type==ATTRIBUTE_XMLDOMNODETYPE) {
		if (pvt->_parent->pvt->_name[0]=='!') {
			append(strb,fd,"\"");
			safeAppend(strb,fd,pvt->_value);
			append(strb,fd,"\"");
		} else {
			safeAppend(strb,fd,pvt->_name);
			append(strb,fd,"=\"");
			safeAppend(strb,fd,pvt->_value);
			append(strb,fd,"\"");
		}
	} else if (pvt->_type==COMMENT_XMLDOMNODETYPE) {
		append(strb,fd,"<!--");
		safeAppend(strb,fd,pvt->_value);
		append(strb,fd,"-->");
	} else if (pvt->_type==CDATA_XMLDOMNODETYPE) {
		append(strb,fd,"<![CDATA[");
		safeAppend(strb,fd,pvt->_value);
		append(strb,fd,"]]>");
	}
}

void xmldomnode::append(stringbuffer *strb,
				filedescriptor *fd,
				const char *str) const {
	if (strb) {
		strb->append(str);
	} else if (fd) {
		fd->write(str);
	}
}

void xmldomnode::append(stringbuffer *strb,
				filedescriptor *fd,
				const char *str, size_t len) const {
	if (strb) {
		strb->append(str,len);
	} else if (fd) {
		fd->write(str,len);
	}
}

void xmldomnode::append(stringbuffer *strb,
				filedescriptor *fd,
				char ch) const {
	if (strb) {
		strb->append(ch);
	} else if (fd) {
		fd->write(ch);
	}
}

void xmldomnode::safeAppend(stringbuffer *strb,
				filedescriptor *fd,
				const char *str) const {

	if (!str) {
		return;
	}

	const char	*start=str;
	const char	*ch=start;
	const char	*entity=NULL;
	uint16_t	num=0;

	for (; *ch; ch++) {
		if (*ch=='&') {
			entity="&amp;";
		} else if (*ch=='<') {
			entity="&lt;";
		} else if (*ch=='>') {
			entity="&gt;";
		} else if (*ch=='\'') {
			entity="&apos;";
		} else if (*ch=='"') {
			entity="&quot;";
		} else if ((signed char)*ch<0) {
			num=static_cast<uint16_t>(
					static_cast<unsigned char>(*ch));
		}
		if (entity || num) {
			append(strb,fd,start,ch-start);
			if (entity) {
				append(strb,fd,entity);
				entity=NULL;
			} else {
				append(strb,fd,"&#");
				append(strb,fd,num);
				append(strb,fd,";");
				num=0;
			}
			start=ch+1;
		}
	}
	append(strb,fd,start,ch-start);
}

xmldomnode *xmldomnode::getNode(xmldomnode *first,
					uint64_t position,
					const char *ns,
					const char *name,
					bool ignorecase,
					uint64_t count) const {
	if (!first || position>=count) {
		return pvt->_nullnode;
	}
	xmldomnode	*current=first;
	if (name) {
		for (uint64_t i=0; i<count; i++) {
			if (match(current,ns,name,ignorecase)) {
				break;
			}
			current=current->pvt->_next;
		}
	} else {
		for (uint64_t i=0; i<position; i++) {
			current=current->pvt->_next;
		}
	}
	return current;
}

bool xmldomnode::insertNode(xmldomnode *node, uint64_t position,
				xmldomnodetype type,
				xmldomnode **first, xmldomnode **last,
				uint64_t *count) {
	if (position>(*count)) {
		return false;
	}
	node->pvt->_parent=this;
	node->pvt->_type=type;
	xmldomnode	*atpos=getNode(*first,position,
						NULL,NULL,false,*count);
	xmldomnode	*beforepos=getNode(*first,position-1,
						NULL,NULL,false,*count);
	if (atpos) {
		node->pvt->_next=atpos;
		atpos->pvt->_previous=node;
	}
	if (beforepos) {
		node->pvt->_previous=beforepos;
		beforepos->pvt->_next=node;
	}
	if (position==0) {
		(*first)=node;
	}
	if (position==(*count)) {
		(*last)=node;
	}
	if (type==TAG_XMLDOMNODETYPE) {
		xmldomnode	*prevtag=node->getPreviousTagSibling();
		if (prevtag->isNullNode()) {
			pvt->_firsttagchild=node;
		} else {
			prevtag->pvt->_nexttag=node;
		}
		node->pvt->_nexttag=node->pvt->_next;
		while (!node->pvt->_nexttag->isNullNode() &&
			node->pvt->_nexttag->getType()!=TAG_XMLDOMNODETYPE) {
			node->pvt->_nexttag=node->pvt->_nexttag->pvt->_next;
		}
	}
	(*count)++;
	return true;
}

bool xmldomnode::deleteNode(xmldomnode *node,
				uint64_t position, const char *name,
				xmldomnode **first, xmldomnode **last,
				uint64_t *count) {
	xmldomnode	*current=unlinkNode(node,position,name,
						first,last,count);
	if (!current) {
		return false;
	}
	delete current;
	return true;
}

xmldomnode *xmldomnode::unlinkNode(xmldomnode *node,
				uint64_t position, const char *name,
				xmldomnode **first, xmldomnode **last,
				uint64_t *count) {

	if (position>(*count)) {
		return NULL;
	}
	xmldomnode	*current=*first;
	if (node || name) {
		while (current && !current->isNullNode() &&
			((name && charstring::compare(
					current->pvt->_name,name)) ||
			(node && current!=node))) {
			current=current->pvt->_next;
		}
	} else {
		for (uint64_t i=0; i<position; i++) {
			current=current->pvt->_next;
		}
	}
	if (current && !current->isNullNode()) {
		if (current->pvt->_type==TAG_XMLDOMNODETYPE) {
			xmldomnode	*prevtag=
					current->getPreviousTagSibling();
			if (!prevtag->isNullNode()) {
				prevtag->pvt->_nexttag=
					current->getNextTagSibling();
			}
			if (current->pvt->_parent->
					pvt->_firsttagchild==current) {
				current->pvt->_parent->
					pvt->_firsttagchild=
						current->getNextTagSibling();
			}
		}
		if (current->pvt->_previous) {
			current->pvt->_previous->pvt->_next=current->pvt->_next;
		}
		if (current->pvt->_next) {
			current->pvt->_next->pvt->_previous=
					current->pvt->_previous;
		}
		if (current==*first) {
			*first=current->pvt->_next;
		}
		if (current==*last) {
			*last=current->pvt->_previous;
		}
		(*count)--;
		return current;
	}
	return NULL;
}

bool xmldomnode::moveChild(xmldomnode *child,
				xmldomnode *parent, uint64_t position) {
	if (!parent || parent->isNullNode()) {
		return false;
	}
	xmldomnode *current=unlinkNode(child,0,NULL,
					&pvt->_firstchild,
					&pvt->_lastchild,
					&pvt->_childcount);
	if (!current) {
		return false;
	}
	return parent->insertChild(current,position);
}

bool xmldomnode::appendChild(xmldomnode *child) {
	return insertChild(child,getChildCount());
}

xmldomnode *xmldomnode::appendTag(const char *tag) {
	return insertTag(NULL,tag,getChildCount());
}

xmldomnode *xmldomnode::appendTag(const char *ns, const char *tag) {
	return insertTag(ns,tag,getChildCount());
}

bool xmldomnode::appendXml(const char *xml) {
	return insertXml(xml,getChildCount());
}

bool xmldomnode::appendXmlFile(const char *xmlfile) {
	return insertXmlFile(xmlfile,getChildCount());
}

bool xmldomnode::appendText(const char *value) {
	return insertText(value,getChildCount());
}

bool xmldomnode::appendAttribute(xmldomnode *attribute) {
	return insertAttribute(attribute,getAttributeCount());
}

bool xmldomnode::appendAttribute(const char *name, const char *value) {
	return insertAttribute(name,value,getAttributeCount());
}

constnamevaluepairs *xmldomnode::getAttributes() const {

	if (pvt->_isnullnode) {
		return NULL;
	}

	constnamevaluepairs	*nvp=new constnamevaluepairs();
	for (uint64_t i=0; i<pvt->_attributecount; i++) {
		nvp->setValue(getAttribute(i)->getName(),
				getAttribute(i)->getValue());
	}
	return nvp;
}

void xmldomnode::setAttributeValue(const char *name, const char *value) {
	if (isNullNode()) {
		return;
	}
	xmldomnode	*attr=getAttribute(name);
	if (!attr->isNullNode()) {
		attr->setValue(value);
	} else {
		appendAttribute(name,value);
	}
}

void xmldomnode::setAttributeValue(const char *name, int64_t value) {
	char	*valuestr=charstring::parseNumber(value);
	setAttributeValue(name,valuestr);
	delete[] valuestr;
}

void xmldomnode::setAttributeValue(const char *name, uint64_t value) {
	char	*valuestr=charstring::parseNumber(value);
	setAttributeValue(name,valuestr);
	delete[] valuestr;
}

void xmldomnode::cascadeOnDelete() {
	pvt->_cascade=true;
}

void xmldomnode::dontCascadeOnDelete() {
	pvt->_cascade=false;
}

xmldomnodetype xmldomnode::getType() const {
	return pvt->_type;
}

const char *xmldomnode::getNamespace() const {
	return pvt->_namespace;
}

const char *xmldomnode::getName() const {
	return pvt->_name;
}

const char *xmldomnode::getValue() const {
	return pvt->_value;
}

xmldom *xmldomnode::getTree() const {
	return pvt->_dom;
}

xmldomnode *xmldomnode::getParent() const {
	return pvt->_parent;
}

uint64_t xmldomnode::getPosition() const {
	xmldomnode	*current=pvt->_parent->getChild((uint64_t)0);
	if (!current) {
		// shouldn't ever happen
		return 0;
	}
	uint64_t	count=pvt->_parent->getChildCount();
	for (uint64_t i=0; i<count; i++) {
		if (this==current) {
			return i;
		}
		current=current->pvt->_next;
	}
	// shouldn't ever happen
	return 0;
}

xmldomnode *xmldomnode::getPreviousSibling() const {
	return pvt->_previous;
}

xmldomnode *xmldomnode::getNextSibling() const {
	return pvt->_next;
}

xmldomnode *xmldomnode::getNullNode() const {
	return pvt->_nullnode;
}

bool xmldomnode::isNullNode() const {
	return pvt->_isnullnode;
}

uint64_t xmldomnode::getChildCount() const {
	return pvt->_childcount;
}

xmldomnode *xmldomnode::getChild(uint64_t position) const {
	return getNode(pvt->_firstchild,position,
				NULL,NULL,false,pvt->_childcount);
}

xmldomnode *xmldomnode::getFirstChild(const char *name) const {
	return getFirstChild(NULL,name,false);
}

xmldomnode *xmldomnode::getFirstChild(const char *ns,
					const char *name) const {
	return getFirstChild(ns,name,false);
}

xmldomnode *xmldomnode::getFirstChildIgnoringCase(const char *name) const {
	return getFirstChild(NULL,name,true);
}

xmldomnode *xmldomnode::getFirstChildIgnoringCase(const char *ns,
						const char *name) const {
	return getFirstChild(ns,name,true);
}

xmldomnode *xmldomnode::getFirstChild(const char *ns,
					const char *name,
					bool ignorecase) const {
	return getNode(pvt->_firstchild,0,
				ns,name,ignorecase,pvt->_childcount);
}

uint64_t xmldomnode::getAttributeCount() const {
	return pvt->_attributecount;
}

xmldomnode *xmldomnode::getAttribute(uint64_t position) const {
	return getNode(pvt->_firstattribute,position,
				NULL,NULL,false,pvt->_attributecount);
}

xmldomnode *xmldomnode::getAttribute(const char *name) const {
	return getAttribute(name,false);
}

xmldomnode *xmldomnode::getAttributeIgnoringCase(const char *name) const {
	return getAttribute(name,true);
}

xmldomnode *xmldomnode::getAttribute(const char *name, bool ignorecase) const {
	return getNode(pvt->_firstattribute,0,
				NULL,name,ignorecase,pvt->_attributecount);
}

const char *xmldomnode::getAttributeValue(uint64_t position) const {
	return getAttribute(position)->getValue();
}

const char *xmldomnode::getAttributeValue(const char *name) const {
	return getAttribute(name)->getValue();
}

void xmldomnode::setType(xmldomnodetype type) {
	pvt->_type=type;
}

void xmldomnode::setNamespace(const char *ns) {
	if (pvt->_dom->stringCacheEnabled()) {
		pvt->_dom->unCacheString(pvt->_namespace);
		pvt->_namespace=pvt->_dom->cacheString(ns);
	} else {
		delete[] (char *)pvt->_namespace;
		pvt->_namespace=charstring::duplicate(ns);
	}
}

void xmldomnode::setName(const char *name) {
	if (pvt->_dom->stringCacheEnabled()) {
		pvt->_dom->unCacheString(pvt->_name);
		pvt->_name=pvt->_dom->cacheString(name);
	} else {
		delete[] (char *)pvt->_name;
		pvt->_name=charstring::duplicate(name);
	}
}

void xmldomnode::setValue(const char *value) {
	if (pvt->_dom->stringCacheEnabled()) {
		pvt->_dom->unCacheString(pvt->_value);
		pvt->_value=pvt->_dom->cacheString(value);
	} else {
		delete[] (char *)pvt->_value;
		pvt->_value=charstring::duplicate(value);
	}
}

void xmldomnode::setParent(xmldomnode *parent) {
	pvt->_parent=parent;
}

void xmldomnode::setPreviousSibling(xmldomnode *previous) {
	pvt->_previous=previous;
}

void xmldomnode::setNextSibling(xmldomnode *next) {
	pvt->_next=next;
}

bool xmldomnode::insertChild(xmldomnode *child, uint64_t position) {
	return insertNode(child,position,
				child->pvt->_type,
				&pvt->_firstchild,
				&pvt->_lastchild,
				&pvt->_childcount);
}

xmldomnode *xmldomnode::insertTag(const char *tag,
					uint64_t position) {
	return insertTag(NULL,tag,position);
}

xmldomnode *xmldomnode::insertTag(const char *ns,
					const char *tag,
					uint64_t position) {
	xmldomnode	*child=new xmldomnode(pvt->_dom,
					pvt->_nullnode,
					TAG_XMLDOMNODETYPE,
					ns,tag,NULL);
	if (insertNode(child,position,
				TAG_XMLDOMNODETYPE,
				&pvt->_firstchild,
				&pvt->_lastchild,
				&pvt->_childcount)) {
		return child;
	}
	delete child;
	return pvt->_nullnode;
}

bool xmldomnode::insertXml(const char *xml, uint64_t position) {
	return pvt->_dom->parseString(xml,this,position);
}

bool xmldomnode::insertXmlFile(const char *xmlfile, uint64_t position) {
	return pvt->_dom->parseFile(xmlfile,this,position);
}

bool xmldomnode::insertAttribute(xmldomnode *attribute, uint64_t position) {
	return insertNode(attribute,position,
				ATTRIBUTE_XMLDOMNODETYPE,
				&pvt->_firstattribute,
				&pvt->_lastattribute,
				&pvt->_attributecount);
}

bool xmldomnode::deleteChild(uint64_t position) {
	return deleteNode(NULL,position,NULL,
				&pvt->_firstchild,
				&pvt->_lastchild,
				&pvt->_childcount);
}

bool xmldomnode::deleteChild(xmldomnode *child) {
	return deleteNode(child,0,NULL,
				&pvt->_firstchild,
				&pvt->_lastchild,
				&pvt->_childcount);
}

bool xmldomnode::deleteFirstChild(const char *name) {
	return deleteFirstChild(NULL,name,false);
}

bool xmldomnode::deleteFirstChild(const char *ns, const char *name) {
	return deleteFirstChild(ns,name,false);
}

bool xmldomnode::deleteFirstChildIgnoringCase(const char *name) {
	return deleteFirstChild(NULL,name,true);
}

bool xmldomnode::deleteFirstChildIgnoringCase(const char *ns,
						const char *name) {
	return deleteFirstChild(ns,name,true);
}

bool xmldomnode::deleteFirstChild(const char *ns,
				const char *name, bool ignorecase) {
	return deleteChild(getFirstChild(ns,name,ignorecase));
}

bool xmldomnode::deleteFirstChild(const char *name,
				const char *attributename,
				const char *attributevalue) {
	return deleteChild(getFirstChild(name,attributename,attributevalue));
}

bool xmldomnode::deleteFirstChild(const char *ns,
				const char *name,
				const char *attributename,
				const char *attributevalue) {
	return deleteChild(getFirstChild(ns,name,attributename,attributevalue));
}

bool xmldomnode::deleteFirstChildIgnoringCase(const char *name,
				const char *attributename,
				const char *attributevalue) {
	return deleteChild(getFirstChildIgnoringCase(
				name,attributename,attributevalue));
}

bool xmldomnode::deleteFirstChildIgnoringCase(
				const char *ns,
				const char *name,
				const char *attributename,
				const char *attributevalue) {
	return deleteChild(getFirstChildIgnoringCase(
				ns,name,attributename,attributevalue));
}

bool xmldomnode::deleteChildren() {
	while (!pvt->_lastchild->isNullNode()) {
		if (!deleteChild(pvt->_lastchild)) {
			return false;
		}
	}
	return true;
}

bool xmldomnode::deleteChildren(const char *name) {
	return deleteChildren(NULL,name,false);
}

bool xmldomnode::deleteChildren(const char *ns, const char *name) {
	return deleteChildren(ns,name,false);
}

bool xmldomnode::deleteChildrenIgnoringCase(const char *name) {
	return deleteChildren(NULL,name,true);
}

bool xmldomnode::deleteChildrenIgnoringCase(const char *ns,
						const char *name) {
	return deleteChildren(ns,name,true);
}

bool xmldomnode::deleteChildren(const char *ns,
				const char *name,
				bool ignorecase) {
	bool	foundone=false;
	while (deleteFirstChild(ns,name,ignorecase)) {
		foundone=true;
	}
	return foundone;
}

bool xmldomnode::deleteChildren(const char *name,
				const char *attributename,
				const char *attributevalue) {
	return deleteChildren(NULL,name,attributename,attributevalue,false);
}

bool xmldomnode::deleteChildren(const char *ns,
				const char *name,
				const char *attributename,
				const char *attributevalue) {
	return deleteChildren(ns,name,attributename,attributevalue,false);
}


bool xmldomnode::deleteChildrenIgnoringCase(const char *name,
						const char *attributename,
						const char *attributevalue) {
	return deleteChildren(NULL,name,attributename,attributevalue,true);
}

bool xmldomnode::deleteChildrenIgnoringCase(const char *ns,
						const char *name,
						const char *attributename,
						const char *attributevalue) {
	return deleteChildren(ns,name,attributename,attributevalue,true);
}

bool xmldomnode::deleteChildren(const char *ns,
				const char *name,
				const char *attributename,
				const char *attributevalue,
				bool ignorecase) {
	bool	foundone=false;
	for (;;) {
		xmldomnode	*child=getFirstTagChild(ns,name,
						attributename,attributevalue,
						ignorecase);
		if (!child->isNullNode()) {
			if (deleteChild(child)) {
				foundone=true;
			} else {
				return false;
			}
		} else {
			return foundone;
		}
	}
}

bool xmldomnode::deleteDescendents(const char *name) {
	return deleteDescendents(NULL,name,false);
}

bool xmldomnode::deleteDescendents(const char *ns,
					const char *name) {
	return deleteDescendents(ns,name,false);
}

bool xmldomnode::deleteDescendentsIgnoringCase(const char *name) {
	return deleteDescendents(NULL,name,true);
}

bool xmldomnode::deleteDescendentsIgnoringCase(const char *ns,
						const char *name) {
	return deleteDescendents(ns,name,true);
}

bool xmldomnode::deleteDescendents(const char *ns,
					const char *name,
					bool ignorecase) {

	// delete immediate children
	bool	foundone=deleteChildren(ns,name,ignorecase);

	// descend
	for (xmldomnode *child=getFirstTagChild();
			!child->isNullNode();
			child=child->getNextTagSibling()) {
		if (child->deleteDescendents(ns,name,ignorecase)) {
			foundone=true;
		}
	}
	return foundone;
}

bool xmldomnode::deleteDescendents(const char *name,
				const char *attributename,
				const char *attributevalue) {
	return deleteDescendents(NULL,name,attributename,attributevalue,false);
}

bool xmldomnode::deleteDescendents(const char *ns,
				const char *name,
				const char *attributename,
				const char *attributevalue) {
	return deleteDescendents(ns,name,attributename,attributevalue,false);
}

bool xmldomnode::deleteDescendentsIgnoringCase(const char *name,
						const char *attributename,
						const char *attributevalue) {
	return deleteDescendents(NULL,name,attributename,attributevalue,true);
}

bool xmldomnode::deleteDescendentsIgnoringCase(const char *ns,
						const char *name,
						const char *attributename,
						const char *attributevalue) {
	return deleteDescendents(ns,name,attributename,attributevalue,true);
}

bool xmldomnode::deleteDescendents(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) {

	// delete immediate children
	bool	foundone=deleteChildren(ns,name,
					attributename,attributevalue,
					ignorecase);

	// descend
	for (xmldomnode *child=getFirstTagChild();
			!child->isNullNode();
			child=child->getNextTagSibling()) {
		if (child->deleteDescendents(ns,name,
						attributename,attributevalue,
						ignorecase)) {
			foundone=true;
		}
	}
	return foundone;
}

bool xmldomnode::renameFirstChild(const char *oldname, const char *newname) {
	return renameFirstChild(NULL,oldname,NULL,newname,false);
}

bool xmldomnode::renameFirstChild(const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname) {
	return renameFirstChild(oldns,oldname,newns,newname,false);
}

bool xmldomnode::renameFirstChildIgnoringCase(const char *oldname,
						const char *newname) {
	return renameFirstChild(NULL,oldname,NULL,newname,true);
}

bool xmldomnode::renameFirstChildIgnoringCase(const char *oldns,
						const char *oldname,
						const char *newns,
						const char *newname) {
	return renameFirstChild(oldns,oldname,newns,newname,true);
}

bool xmldomnode::renameFirstChild(const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname,
					bool ignorecase) {
	xmldomnode	*child=getFirstChild(oldns,oldname,ignorecase);
	if (!child->isNullNode()) {
		child->setNamespace(newns);
		child->setName(newname);
		return true;
	}
	return false;
}

bool xmldomnode::renameChildren(const char *oldname,
						const char *newname) {
	return renameChildren(NULL,oldname,NULL,newname,false);
}

bool xmldomnode::renameChildren(const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname) {
	return renameChildren(oldns,oldname,newns,newname,false);
}

bool xmldomnode::renameChildrenIgnoringCase(const char *oldname,
						const char *newname) {
	return renameChildren(NULL,oldname,NULL,newname,true);
}

bool xmldomnode::renameChildrenIgnoringCase(const char *oldns,
						const char *oldname,
						const char *newns,
						const char *newname) {
	return renameChildren(oldns,oldname,newns,newname,true);
}

bool xmldomnode::renameChildren(const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname,
					bool ignorecase) {

	// don't do anything if the ns/names are the same
	if ((ignorecase)?
		(!charstring::compareIgnoringCase(oldns,newns) &&
		!charstring::compareIgnoringCase(oldname,newname)):
		(!charstring::compare(oldns,newns) &&
		!charstring::compare(oldname,newname))) {
		return true;
	}

	// rename children
	bool	foundone=false;
	for (xmldomnode *child=getChild((uint64_t)0);
			!child->isNullNode();
			child=child->getNextSibling()) {
		if (match(child,oldns,oldname,ignorecase)) {
			child->setNamespace(newns);
			child->setName(newname);
			foundone=true;
		}
	}
	return foundone;
}

bool xmldomnode::renameDescendents(const char *oldname,
						const char *newname) {
	return renameDescendents(NULL,oldname,NULL,newname,false);
}

bool xmldomnode::renameDescendents(const char *oldns,
						const char *oldname,
						const char *newns,
						const char *newname) {
	return renameDescendents(oldns,oldname,newns,newname,false);
}

bool xmldomnode::renameDescendentsIgnoringCase(const char *oldname,
						const char *newname) {
	return renameDescendents(NULL,oldname,NULL,newname,true);
}

bool xmldomnode::renameDescendentsIgnoringCase(const char *oldns,
						const char *oldname,
						const char *newns,
						const char *newname) {
	return renameDescendents(oldns,oldname,newns,newname,true);
}

bool xmldomnode::renameDescendents(const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname,
					bool ignorecase) {
	// rename immediate children
	bool	foundone=renameChildren(oldns,oldname,newns,newname,ignorecase);

	// descend
	for (xmldomnode *child=getFirstTagChild();
			!child->isNullNode();
			child=child->getNextTagSibling()) {
		if (child->renameDescendents(oldns,oldname,
						newns,newname,ignorecase)) {
			foundone=true;
		}
	}
	return foundone;
}

bool xmldomnode::deleteAttribute(uint64_t position) {
	return deleteNode(NULL,position,NULL,
				&pvt->_firstattribute,
				&pvt->_lastattribute,
				&pvt->_attributecount);
}

bool xmldomnode::deleteAttribute(const char *name) {
	return deleteNode(NULL,0,name,
				&pvt->_firstattribute,
				&pvt->_lastattribute,
				&pvt->_attributecount);
}

bool xmldomnode::deleteAttribute(xmldomnode *attribute) {
	return deleteNode(attribute,0,NULL,
				&pvt->_firstattribute,
				&pvt->_lastattribute,
				&pvt->_attributecount);
}

stringbuffer *xmldomnode::xml() const {
	stringbuffer *strb=new stringbuffer();
	xml(strb,NULL,false,NULL);
	return strb;
}

void xmldomnode::print(stringbuffer *strb) const {
	uint16_t	indentlevel=0;
	xml(strb,NULL,true,&indentlevel);
}

void xmldomnode::print(filedescriptor *fd) const {
	uint16_t	indentlevel=0;
	xml(NULL,fd,true,&indentlevel);
}

stringbuffer *xmldomnode::getPath() const {

	// Path: /element[index]/...

	// run up the tree, counting parent nodes
	int64_t			ancestors=0;
	const xmldomnode	*node=this;
	while (!node->isNullNode() && node->getType()!=ROOT_XMLDOMNODETYPE) {
		ancestors++;
		node=node->getParent();
	}

	// create pointers to the names of each parent node
	const char	**names=new const char *[ancestors];
	uint64_t	*indices=new uint64_t[ancestors];
	int64_t		index;
	node=this;
	for (index=ancestors-1; index>=0; index--) {

		// get the name
		names[index]=node->getName();

		// figure out which sibling this node is
		indices[index]=0;
		for (xmldomnode *siblingnode=
				node->getPreviousTagSibling(names[index]);
			(!siblingnode->isNullNode() &&
				siblingnode->getType()!=ROOT_XMLDOMNODETYPE);
			siblingnode=siblingnode->
				getPreviousTagSibling(names[index])) {
			indices[index]++;
		}
		
		node=node->getParent();
	}

	// run through the list of parent node names and indices,
	// append them all to the path
	stringbuffer	*path=new stringbuffer();
	for (index=0; index<ancestors; index++) {
		path->append('/')->append(names[index]);
		path->append('[')->append(indices[index])->append(']');
	}

	delete[] names;
	delete[] indices;

	return path;
}

xmldomnode *xmldomnode::getChildByPath(const char *path) const {

	// Path: /element[index]/...
	const xmldomnode	*node=this;
	stringbuffer		name;
	stringbuffer		indexstring;
	stringbuffer		*buffer=&name;
	const char		*ptr=path;
	for (;;) {

		if (!*ptr || *ptr=='@') {
			break;
		} else if (*ptr=='/') {
			buffer=&name;
		} else if (*ptr=='[') {
			buffer=&indexstring;
		} else if (*ptr==']') {

			// get the first child
			node=node->getFirstChild(name.getString());

			// now skip until we get the specified index
			uint64_t	index=
				charstring::toUnsignedInteger(
						indexstring.getString());
			for (uint64_t i=0; i<index; i++) {
				node=node->getNextTagSibling(name.getString());
			}

			// reset the buffers
			name.clear();
			indexstring.clear();

		} else {
			buffer->append(*ptr);
		}

		ptr++;
	}

	return const_cast<xmldomnode *>(node);
}

xmldomnode *xmldomnode::getAttributeByPath(const char *path,
						uint64_t position) const {
	return getChildByPath(path)->getAttribute(position);
}

xmldomnode *xmldomnode::getAttributeByPath(const char *path,
						const char *name) const {
	return getChildByPath(path)->getAttribute(name);
}

const char *xmldomnode::getAttributeValueByPath(const char *path,
						uint64_t position) const {
	return getChildByPath(path)->getAttributeValue(position);
}

const char *xmldomnode::getAttributeValueByPath(const char *path,
						const char *name) const {
	return getChildByPath(path)->getAttributeValue(name);
}

void xmldomnode::setData(void *data) {
	pvt->_data=data;
}

void *xmldomnode::getData() {
	return pvt->_data;
}

bool xmldomnode::unwrapChild(xmldomnode *child) {

	// get the position of the specified node
	uint64_t	position=child->getPosition();

	// deparent the children
	for (xmldomnode *grandchild=child->getChild((uint64_t)0);
			!grandchild->isNullNode();
			grandchild=child->getChild((uint64_t)0)) {

		position++;
		if (!child->moveChild(grandchild,this,position)) {
			return false;
		}
	}

	// delete the specified child
	return deleteChild(child);
}

bool xmldomnode::unwrapFirstChild(const char *name) {
	return unwrapFirstChild(NULL,name,false);
}

bool xmldomnode::unwrapFirstChild(const char *ns, const char *name) {
	return unwrapFirstChild(ns,name,false);
}

bool xmldomnode::unwrapFirstChildIgnoringCase(const char *name) {
	return unwrapFirstChild(NULL,name,true);
}

bool xmldomnode::unwrapFirstChildIgnoringCase(const char *ns,
						const char *name) {
	return unwrapFirstChild(ns,name,true);
}

bool xmldomnode::unwrapFirstChild(const char *ns,
					const char *name,
					bool ignorecase) {
	xmldomnode	*child=getFirstTagChild(ns,name,ignorecase);
	if (child->isNullNode()) {
		return false;
	}
	return unwrapChild(child);
}

bool xmldomnode::unwrapChildrenIgnoringCase(const char *name) {
	return unwrapChildren(NULL,name,false);
}

bool xmldomnode::unwrapChildrenIgnoringCase(const char *ns,
						const char *name) {
	return unwrapChildren(ns,name,false);
}

bool xmldomnode::unwrapChildren(const char *name) {
	return unwrapChildren(NULL,name,true);
}

bool xmldomnode::unwrapChildren(const char *ns, const char *name) {
	return unwrapChildren(ns,name,true);
}

bool xmldomnode::unwrapChildren(const char *ns,
				const char *name,
				bool ignorecase) {
	bool	foundone=false;
	while (unwrapFirstChild(ns,name,ignorecase)) {
		foundone=true;
	}
	return foundone;
}

bool xmldomnode::unwrapDescendents(const char *name) {
	return unwrapDescendents(NULL,name,false);
}

bool xmldomnode::unwrapDescendents(const char *ns, const char *name) {
	return unwrapDescendents(ns,name,false);
}

bool xmldomnode::unwrapDescendentsIgnoringCase(const char *name) {
	return unwrapDescendents(NULL,name,true);
}

bool xmldomnode::unwrapDescendentsIgnoringCase(const char *ns,
						const char *name) {
	return unwrapDescendents(ns,name,true);
}

bool xmldomnode::unwrapDescendents(const char *ns,
					const char *name,
					bool ignorecase) {

	// unwrap immediate children
	bool	foundone=unwrapChildren(ns,name,ignorecase);

	// descend
	for (xmldomnode *child=getFirstTagChild();
			!child->isNullNode();
			child=child->getNextTagSibling()) {
		if (child->unwrapDescendents(ns,name,ignorecase)) {
			foundone=true;
		}
	}
	return foundone;
}

xmldomnode *xmldomnode::wrapChild(xmldomnode *child, const char *name) {
	return wrapChildren(child,child,name);
}

xmldomnode *xmldomnode::wrapChild(xmldomnode *child,
					const char *ns, const char *name) {
	return wrapChildren(child,child,ns,name);
}

xmldomnode *xmldomnode::wrapChildren(xmldomnode *startchild,
						xmldomnode *endchild,
						const char *name) {
	return wrapChildren(startchild,endchild,NULL,name);
}

xmldomnode *xmldomnode::wrapChildren(xmldomnode *startchild,
						xmldomnode *endchild,
						const char *ns,
						const char *name) {

	xmldomnode	*newnode=insertTag(ns,name,startchild->getPosition());
	if (newnode->isNullNode()) {
		return pvt->_nullnode;
	}

	xmldomnode	*currentchild=startchild;
	for (;;) {

		xmldomnode	*nextchild=currentchild->getNextSibling();

		if (!moveChild(currentchild,newnode,newnode->getChildCount())) {
			return pvt->_nullnode;
		}

		if (currentchild==endchild) {
			break;
		}

		currentchild=nextchild;
	}
	return newnode;
}

xmldomnode *xmldomnode::wrapChildren(const char *name) {
	return wrapChildren(NULL,name);
}

xmldomnode *xmldomnode::wrapChildren(const char *ns, const char *name) {

	if (!getChildCount()) {
		return insertTag(ns,name,0);
	}

	return wrapChildren(getChild((uint64_t)0),
				getChild(getChildCount()-1),ns,name);
}

xmldomnode *xmldomnode::clone() {

	// clone this node
	xmldomnode	*clonednode=new xmldomnode(getTree(),
						getNullNode(),getType(),
						getNamespace(),
						getName(),getValue());

	// clone attributes
	for (uint64_t i=0; i<getAttributeCount(); i++) {
		clonednode->setAttributeValue(getAttribute(i)->getName(),
						getAttribute(i)->getValue());
	}

	// clone children
	for (xmldomnode *child=pvt->_firstchild;
			!child->isNullNode(); child=child->pvt->_next) {
		clonednode->appendChild(child->clone());
	}

	return clonednode;
}

bool xmldomnode::match(xmldomnode *node,
				const char *ns,
				const char *name,
				bool ignorecase) const {

	const char	*nodens=node->pvt->_namespace;
	const char	*nodename=node->pvt->_name;

	if (ns) {
		if (ignorecase) {
			if (charstring::compareIgnoringCase(nodens,ns)) {
				return false;
			}
		} else {
			if (charstring::compare(nodens,ns)) {
				return false;
			}
		}
	}

	if (name) {
		if (ignorecase) {
			if (charstring::compareIgnoringCase(nodename,name)) {
				return false;
			}
		} else {
			if (charstring::compare(nodename,name)) {
				return false;
			}
		}
	}
	return true;
}

bool xmldomnode::match(xmldomnode *node,
				const char *ns,
				const char * const *set) const {
	if (ns && charstring::compare(node->pvt->_namespace,ns)) {
		return false;
	}
	if (set && charstring::inSet(node->pvt->_name,set)) {
		return false;
	}
	return true;
}
