// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#define EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
#include <rudiments/xmldomnode.h>
#include <rudiments/charstring.h>
#include <rudiments/xmldom.h>

#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
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
		xmldomnode	*_previous;
		int		_childcount;
		xmldomnode	*_firstchild;
		xmldomnode	*_lastchild;
		int		_attributecount;
		xmldomnode	*_firstattribute;
		xmldomnode	*_lastattribute;
		xmldomnode	*_nullnode;
		bool		_isnullnode;
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
	pvt->_previous=nullnode;
	pvt->_firstchild=NULL;
	pvt->_lastchild=NULL;
	pvt->_childcount=0;
	pvt->_firstattribute=NULL;
	pvt->_lastattribute=NULL;
	pvt->_attributecount=0;
	pvt->_cascade=true;
	pvt->_isnullnode=false;
	pvt->_nodename=NULL;
	pvt->_nodevalue=NULL;
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
	pvt->_dom->unCacheString(pvt->_nodename);
	pvt->_dom->unCacheString(pvt->_nodevalue);
	delete pvt;
}

xmldomnode *xmldomnode::createNullNode(xmldom *dom) {
	xmldomnode	*nn=new xmldomnode(dom,NULL);
	nn->pvt->_parent=nn;
	nn->pvt->_next=nn;
	nn->pvt->_previous=nn;
	nn->pvt->_isnullnode=true;
	nn->pvt->_nullnode=nn;
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

xmldomnode *xmldomnode::getFirstTagChild() const {
	xmldomnode	*node=getChild(0);
	return (node->getType()==TAG_XMLDOMNODETYPE)?
					node:node->getNextTagSibling();
}

xmldomnode *xmldomnode::getFirstTagChild(const char *name) const {
	xmldomnode	*node=getChild(name);
	return (node->getType()==TAG_XMLDOMNODETYPE)?
					node:node->getNextTagSibling(name);
}

xmldomnode *xmldomnode::getFirstTagChild(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	xmldomnode	*node=getChild(name,attributename,attributevalue);
	return (node->getType()==TAG_XMLDOMNODETYPE)?
		node:node->getNextTagSibling(name,attributename,attributevalue);
}

xmldomnode *xmldomnode::getChild(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	for (xmldomnode *current=pvt->_firstchild;
			current && !current->isNullNode();
				current=current->pvt->_next) {
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

bool xmldomnode::insertText(const char *value, int position) {
	xmldomnode	*text=new xmldomnode(pvt->_dom,pvt->_nullnode);
	text->setName("text");
	text->setValue(value);
	return insertNode(text,position,TEXT_XMLDOMNODETYPE,
				&pvt->_firstchild,&pvt->_lastchild,
				&pvt->_childcount);
}

bool xmldomnode::insertAttribute(const char *name, const char *value,
								int position) {
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
		for (int i=0; i<pvt->_childcount; i++) {
			current->xml(output);
			current=current->pvt->_next;
		}
	} else if (pvt->_type==TAG_XMLDOMNODETYPE) {
		output->append("<");
		output->append(pvt->_nodename);
		current=pvt->_firstattribute;
		for (int i=0; i<pvt->_attributecount; i++) {
			output->append(" ");
			current->xml(output);
			current=current->pvt->_next;
		}
		if (pvt->_childcount) {
			output->append(">");
			current=pvt->_firstchild;
			for (int i=0; i<pvt->_childcount; i++) {
				current->xml(output);
				current=current->pvt->_next;
			}
			output->append("</");
			output->append(pvt->_nodename);
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
		output->append(pvt->_nodevalue);
	} else if (pvt->_type==ATTRIBUTE_XMLDOMNODETYPE) {
		if (pvt->_parent->pvt->_nodename[0]=='!') {
			output->append("\"");
			output->append(pvt->_nodevalue);
			output->append("\"");
		} else {
			output->append(pvt->_nodename);
			output->append("=\"");
			output->append(pvt->_nodevalue);
			output->append("\"");
		}
	} else if (pvt->_type==COMMENT_XMLDOMNODETYPE) {
		output->append("<!--");
		output->append(pvt->_nodevalue);
		output->append("-->");
	} else if (pvt->_type==CDATA_XMLDOMNODETYPE) {
		output->append("<![CDATA[");
		output->append(pvt->_nodevalue);
		output->append("]]>");
	}
	return output;
}

xmldomnode *xmldomnode::getNode(xmldomnode *first, int position,
					const char *name, int count) const {
	if (!first || position>=count) {
		return pvt->_nullnode;
	}
	xmldomnode	*current=first;
	if (name) {
		for (int i=0; i<count; i++) {
			if (!charstring::compare(
					current->pvt->_nodename,name)) {
				break;
			}
			current=current->pvt->_next;
		}
	} else {
		for (int i=0; i<position; i++) {
			current=current->pvt->_next;
		}
	}
	return current;
}

bool xmldomnode::insertNode(xmldomnode *node, int position,
				xmldomnodetype type,
				xmldomnode **first, xmldomnode **last,
				int *count) {
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
	(*count)++;
	return true;
}

bool xmldomnode::deleteNode(xmldomnode *node, int position, const char *name,
				xmldomnode **first, xmldomnode **last,
				int *count) {

	if (position>(*count)) {
		return false;
	}
	xmldomnode	*current=*first;
	if (node || name) {
		while (current &&
			((name && charstring::compare(
					current->pvt->_nodename,name)) ||
			(node && current!=node))) {
			current=current->pvt->_next;
		}
	} else {
		for (int i=0; i<position; i++) {
			current=current->pvt->_next;
		}
	}
	if (current) {
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
		delete current;
		(*count)--;
		return true;
	}
	return false;
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
	for (int i=0; i<pvt->_attributecount; i++) {
		nvp->setData(getAttribute(i)->getName(),
				getAttribute(i)->getValue());
	}
	return nvp;
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

xmldomnode *xmldomnode::getParent() const {
	return pvt->_parent;
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

int xmldomnode::getChildCount() const {
	return pvt->_childcount;
}

xmldomnode *xmldomnode::getChild(int position) const {
	return getNode(pvt->_firstchild,position,NULL,pvt->_childcount);
}

xmldomnode *xmldomnode::getChild(const char *name) const {
	return getNode(pvt->_firstchild,0,name,pvt->_childcount);
}

int xmldomnode::getAttributeCount() const {
	return pvt->_attributecount;
}

xmldomnode *xmldomnode::getAttribute(int position) const {
	return getNode(pvt->_firstattribute,position,NULL,pvt->_attributecount);
}

xmldomnode *xmldomnode::getAttribute(const char *name) const {
	return getNode(pvt->_firstattribute,0,name,pvt->_attributecount);
}

const char *xmldomnode::getAttributeValue(int position) const {
	return getAttribute(position)->getValue();
}

const char *xmldomnode::getAttributeValue(const char *name) const {
	return getAttribute(name)->getValue();
}

void xmldomnode::setType(xmldomnodetype type) {
	pvt->_type=type;
}

void xmldomnode::setName(const char *name) {
	pvt->_dom->unCacheString(pvt->_nodename);
	pvt->_nodename=pvt->_dom->cacheString(name);
}

void xmldomnode::setValue(const char *value) {
	pvt->_dom->unCacheString(pvt->_nodevalue);
	pvt->_nodevalue=pvt->_dom->cacheString(value);
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

bool xmldomnode::insertChild(xmldomnode *child, int position) {
	return insertNode(child,position,
				child->pvt->_type,
				&pvt->_firstchild,
				&pvt->_lastchild,
				&pvt->_childcount);
}

bool xmldomnode::insertAttribute(xmldomnode *attribute, int position) {
	return insertNode(attribute,position,
				ATTRIBUTE_XMLDOMNODETYPE,
				&pvt->_firstattribute,
				&pvt->_lastattribute,
				&pvt->_attributecount);
}

bool xmldomnode::deleteChild(int position) {
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

bool xmldomnode::deleteAttribute(int position) {
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
	int	ancestors=0;
	const xmldomnode	*node=this;
	while (!node->isNullNode() && node->getType()!=ROOT_XMLDOMNODETYPE) {
		ancestors++;
		node=node->getParent();
	}

	// create pointers to the names of each parent node
	const char	**names=new const char *[ancestors];
	uint32_t	*indices=new uint32_t[ancestors];
	node=this;
	for (int index=ancestors-1; index>=0; index--) {

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
	for (int index=0; index<ancestors; index++) {
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
						int position) const {
	return getChildByPath(path)->getAttribute(position);
}

xmldomnode *xmldomnode::getAttributeByPath(const char *path,
						const char *name) const {
	return getChildByPath(path)->getAttribute(name);
}

const char *xmldomnode::getAttributeValueByPath(const char *path,
						int position) const {
	return getChildByPath(path)->getAttributeValue(position);
}

const char *xmldomnode::getAttributeValueByPath(const char *path,
						const char *name) const {
	return getChildByPath(path)->getAttributeValue(name);
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
