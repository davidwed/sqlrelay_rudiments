// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#define EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
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
		const char	*_nodename;
		const char	*_nodevalue;
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
			xmldomnode *nullnode, xmldomnodetype type,
			const char *name, const char *value) {
	init(nullnode);
	pvt->_dom=dom;
	pvt->_type=type;
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
	pvt->_nodename=NULL;
	pvt->_nodevalue=NULL;
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
		pvt->_dom->unCacheString(pvt->_nodename);
		pvt->_dom->unCacheString(pvt->_nodevalue);
	} else {
		delete[] pvt->_nodename;
		delete[] pvt->_nodevalue;
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
	for (xmldomnode *current=getPreviousTagSibling();
			current && !current->isNullNode();
				current=current->getPreviousTagSibling()) {
		const char	*nm=current->getName();
		if ((name && nm && !charstring::compare(name,nm)) || !name) {
			return current;
		}
	}
	return pvt->_nullnode;
}

xmldomnode *xmldomnode::getPreviousTagSibling(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	for (xmldomnode *current=getPreviousTagSibling();
			current && !current->isNullNode();
				current=current->getPreviousTagSibling()) {
		const char	*nm=current->getName();
		if ((name && nm && !charstring::compare(name,nm)) || !name) {
			const char	*value=current->
						getAttribute(attributename)->
						getValue();
			if (value &&
				!charstring::compare(value,attributevalue)) {
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
	for (xmldomnode *current=getNextTagSibling();
			current && !current->isNullNode();
				current=current->getNextTagSibling()) {
		const char	*nm=current->getName();
		if ((name && nm && !charstring::compare(name,nm)) || !name) {
			return current;
		}
	}
	return pvt->_nullnode;
}

xmldomnode *xmldomnode::getNextTagSibling(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	for (xmldomnode *current=getNextTagSibling();
			current && !current->isNullNode();
				current=current->getNextTagSibling()) {
		const char	*nm=current->getName();
		if ((name && nm && !charstring::compare(name,nm)) || !name) {
			const char	*value=current->
						getAttribute(attributename)->
						getValue();
			if (value &&
				!charstring::compare(value,attributevalue)) {
				return current;
			}
		}
	}
	return pvt->_nullnode;
}

xmldomnode *xmldomnode::getNextTagSiblingInSet(const char * const *set) const {
	for (xmldomnode *child=getNextTagSibling();
			!child->isNullNode(); 
			child=child->getNextTagSibling()) {
		if (charstring::inSet(child->getName(),set)) {
			return child;
		}
	}
	return pvt->_nullnode;
}

xmldomnode *xmldomnode::getFirstTagChild() const {
	return pvt->_firsttagchild;
}

xmldomnode *xmldomnode::getFirstTagChild(const char *name) const {
	xmldomnode	*node=pvt->_firsttagchild;
	return (node->isNullNode() ||
		!charstring::compare(node->getName(),name))?
			node:node->getNextTagSibling(name);
}

xmldomnode *xmldomnode::getFirstTagChild(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	xmldomnode	*node=getChild(name,attributename,attributevalue);
	return (node->isNullNode() || node->getType()==TAG_XMLDOMNODETYPE)?
		node:node->getNextTagSibling(name,attributename,attributevalue);
}

xmldomnode *xmldomnode::getFirstTagChildInSet(const char * const *set) const {
	xmldomnode *child=getFirstTagChild();
	if (charstring::inSet(child->getName(),set)) {
		return child;
	}
	return child->getNextTagSiblingInSet(set);
}

xmldomnode *xmldomnode::getChild(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	for (xmldomnode *current=pvt->_firstchild;
			!current->isNullNode(); current=current->pvt->_next) {
		const char	*nm=current->getName();
		if ((name && nm && !charstring::compare(name,nm)) || !name) {
			const char	*value=current->
						getAttribute(attributename)->
						getValue();
			if (value &&
				!charstring::compare(value,attributevalue)) {
				return current;
			}
		}
	}
	return pvt->_nullnode;
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

stringbuffer *xmldomnode::xml(stringbuffer *string) const {
	stringbuffer	*output=string;
	if (!string) {
		output=new stringbuffer();
	}
	xmldomnode	*current;
	if (pvt->_type==ROOT_XMLDOMNODETYPE) {
		current=pvt->_firstchild;
		for (uint64_t i=0; i<pvt->_childcount; i++) {
			current->xml(output);
			current=current->pvt->_next;
		}
	} else if (pvt->_type==TAG_XMLDOMNODETYPE) {
		output->append("<");
		safeAppend(output,pvt->_nodename);
		current=pvt->_firstattribute;
		for (uint64_t i=0; i<pvt->_attributecount; i++) {
			output->append(" ");
			current->xml(output);
			current=current->pvt->_next;
		}
		if (pvt->_childcount) {
			output->append(">");
			current=pvt->_firstchild;
			for (uint64_t i=0; i<pvt->_childcount; i++) {
				current->xml(output);
				current=current->pvt->_next;
			}
			output->append("</");
			safeAppend(output,pvt->_nodename);
			output->append(">");
		} else {
			if (pvt->_nodename[0]=='?') {
				output->append("?>");
			} else if (pvt->_nodename[0]=='!') {
				output->append(">");
			} else {
				output->append("/>");
			}
		}
	} else if (pvt->_type==TEXT_XMLDOMNODETYPE) {
		safeAppend(output,pvt->_nodevalue);
	} else if (pvt->_type==ATTRIBUTE_XMLDOMNODETYPE) {
		if (pvt->_parent->pvt->_nodename[0]=='!') {
			output->append("\"");
			safeAppend(output,pvt->_nodevalue);
			output->append("\"");
		} else {
			safeAppend(output,pvt->_nodename);
			output->append("=\"");
			safeAppend(output,pvt->_nodevalue);
			output->append("\"");
		}
	} else if (pvt->_type==COMMENT_XMLDOMNODETYPE) {
		output->append("<!--");
		safeAppend(output,pvt->_nodevalue);
		output->append("-->");
	} else if (pvt->_type==CDATA_XMLDOMNODETYPE) {
		output->append("<![CDATA[");
		safeAppend(output,pvt->_nodevalue);
		output->append("]]>");
	}
	return output;
}

void xmldomnode::safeAppend(stringbuffer *output, const char *str) const {

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
			output->append(start,ch-start);
			if (entity) {
				output->append(entity);
				entity=NULL;
			} else {
				output->append("&#");
				output->append(num);
				output->append(";");
				num=0;
			}
			start=ch+1;
		}
	}
	output->append(start,ch-start);
}

xmldomnode *xmldomnode::getNode(xmldomnode *first, uint64_t position,
				const char *name, uint64_t count) const {
	if (!first || position>=count) {
		return pvt->_nullnode;
	}
	xmldomnode	*current=first;
	if (name) {
		for (uint64_t i=0; i<count; i++) {
			if (!charstring::compare(
					current->pvt->_nodename,name)) {
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
	xmldomnode	*atpos=getNode(*first,position,NULL,*count);
	xmldomnode	*beforepos=getNode(*first,position-1,NULL,*count);
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
					current->pvt->_nodename,name)) ||
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
		nvp->setData(getAttribute(i)->getName(),
				getAttribute(i)->getValue());
	}
	return nvp;
}

void xmldomnode::setAttributeValue(const char *name, const char *value) {
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

const char *xmldomnode::getName() const {
	return pvt->_nodename;
}

const char *xmldomnode::getValue() const {
	return pvt->_nodevalue;
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
	return getNode(pvt->_firstchild,position,NULL,pvt->_childcount);
}

xmldomnode *xmldomnode::getChild(const char *name) const {
	return getNode(pvt->_firstchild,0,name,pvt->_childcount);
}

uint64_t xmldomnode::getAttributeCount() const {
	return pvt->_attributecount;
}

xmldomnode *xmldomnode::getAttribute(uint64_t position) const {
	return getNode(pvt->_firstattribute,position,NULL,pvt->_attributecount);
}

xmldomnode *xmldomnode::getAttribute(const char *name) const {
	return getNode(pvt->_firstattribute,0,name,pvt->_attributecount);
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

void xmldomnode::setName(const char *name) {
	if (pvt->_dom->stringCacheEnabled()) {
		pvt->_dom->unCacheString(pvt->_nodename);
		pvt->_nodename=pvt->_dom->cacheString(name);
	} else {
		delete[] pvt->_nodename;
		pvt->_nodename=charstring::duplicate(name);
	}
}

void xmldomnode::setValue(const char *value) {
	if (pvt->_dom->stringCacheEnabled()) {
		pvt->_dom->unCacheString(pvt->_nodevalue);
		pvt->_nodevalue=pvt->_dom->cacheString(value);
	} else {
		delete[] pvt->_nodevalue;
		pvt->_nodevalue=charstring::duplicate(value);
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

bool xmldomnode::deleteChildren() {
	while (!pvt->_lastchild->isNullNode()) {
		if (!deleteChild(pvt->_lastchild)) {
			return false;
		}
	}
	return true;
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
	return xml(NULL);
}

stringbuffer *xmldomnode::getPath() const {

	// Path: /element[index]/...

	// run up the tree, counting parent nodes
	uint64_t	ancestors=0;
	const xmldomnode	*node=this;
	while (!node->isNullNode() && node->getType()!=ROOT_XMLDOMNODETYPE) {
		ancestors++;
		node=node->getParent();
	}

	// create pointers to the names of each parent node
	const char	**names=new const char *[ancestors];
	uint64_t	*indices=new uint64_t[ancestors];
	node=this;
	for (uint64_t index=ancestors-1; index>=0; index--) {

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
	for (uint64_t index=0; index<ancestors; index++) {
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
			node=node->getChild(name.getString());

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

void xmldomnode::print(xmldomnode *node) {
	print(node,NULL);
}

void xmldomnode::print(xmldomnode *node, stringbuffer *strb) {
	if (!node || node->isNullNode()) {
		return;
	}
	stringbuffer	*xmlstr=node->xml();
	const char	*xml=xmlstr->getString();
	int16_t		indent=0;
	bool		endtag=false;
	for (const char *ptr=xml; *ptr; ptr++) {
		if (*ptr=='<') {
			if (*(ptr+1)=='/') {
				indent=indent-2;
				endtag=true;
			}
			for (uint16_t i=0; i<indent; i++) {
				if (strb) {
					strb->append(' ');
				} else {
					stdoutput.printf(" ");
				}
			}
		}
		if (strb) {
			strb->append(*ptr);
		} else {
			stdoutput.printf("%c",*ptr);
		}
		if (*ptr=='>') {
			if (strb) {
				strb->append('\n');
			} else {
				stdoutput.printf("\n");
			}
			if (*(ptr-1)!='/' && !endtag) {
				indent=indent+2;
			}
			endtag=false;
		}
	}
	delete xmlstr;
}
