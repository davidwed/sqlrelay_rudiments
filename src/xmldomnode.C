// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/xmldomnode.h>

#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

#include <stdio.h>

xmldomnode::xmldomnode(xmldomnode *nullnode) {
	this->nullnode=nullnode;
	type=(xmldomnodetype)NULL;
	nodename=NULL;
	nodevalue=NULL;
	parent=nullnode;
	next=nullnode;
	previous=nullnode;
	firstchild=NULL;
	lastchild=NULL;
	childcount=0;
	firstattribute=NULL;
	lastattribute=NULL;
	attributecount=0;
	cascade=true;
	isnullnode=false;
}

xmldomnode::xmldomnode(xmldomnode *nullnode, xmldomnodetype type,
			const char *name, const char *value) {
	this->nullnode=nullnode;
	this->type=type;
	nodename=(name)?strdup((char *)name):NULL;
	nodevalue=(value)?strdup((char *)value):NULL;
	parent=NULL;
	next=nullnode;
	previous=nullnode;
	firstchild=NULL;
	lastchild=NULL;
	childcount=0;
	firstattribute=NULL;
	lastattribute=NULL;
	attributecount=0;
	cascade=true;
	isnullnode=false;
}

xmldomnode::~xmldomnode() {
	delete[] nodename;
	delete[] nodevalue;
	xmldomnode	*current;
	if (cascade) {
		// delete child nodes
		current=firstchild;
		while (current && !current->isNullNode()) {
			lastchild=current->next;
			delete current;
			current=lastchild;
		}
	}
	// delete attributes
	current=firstattribute;
	while (current && !current->isNullNode()) {
		lastattribute=current->next;
		delete current;
		current=lastattribute;
	}
}

xmldomnode *xmldomnode::createNullNode() {
	xmldomnode	*nn=new xmldomnode(NULL);
	nn->parent=nn;
	nn->next=nn;
	nn->previous=nn;
	nn->isnullnode=true;
	nn->nullnode=nn;
	return nn;
}

xmldomnode *xmldomnode::getPreviousTagSibling() const {
	xmldomnode	*node=(xmldomnode *)this;
	while (!(node=node->getPreviousSibling())->isNullNode() &&
				!node->getType()==TAG_XMLDOMNODETYPE);
	return node;
}

xmldomnode *xmldomnode::getPreviousTagSibling(const char *name) const {
	for (xmldomnode *current=getPreviousTagSibling();
			current && !current->isNullNode();
				current=current->getPreviousTagSibling()) {
		char	*nm=current->getName();
		if ((name && nm && !strcmp(name,nm)) || !name) {
			return current;
		}
	}
	return nullnode;
}

xmldomnode *xmldomnode::getPreviousTagSibling(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	for (xmldomnode *current=getPreviousTagSibling();
			current && !current->isNullNode();
				current=current->getPreviousTagSibling()) {
		char	*nm=current->getName();
		if ((name && nm && !strcmp(name,nm)) || !name) {
			char	*value=current->
					getAttribute(attributename)->
						getValue();
			if (value && !strcmp(value,attributevalue)) {
				return current;
			}
		}
	}
	return nullnode;
}

xmldomnode *xmldomnode::getNextTagSibling() const {
	xmldomnode	*node=(xmldomnode *)getNextSibling();
	while (!node->isNullNode() && !node->getType()==TAG_XMLDOMNODETYPE) {
		node=node->getNextSibling();
	}
	return node;
}

xmldomnode *xmldomnode::getNextTagSibling(const char *name) const {
	for (xmldomnode *current=getNextTagSibling();
			current && !current->isNullNode();
				current=current->getNextTagSibling()) {
		char	*nm=current->getName();
		if ((name && nm && !strcmp(name,nm)) || !name) {
			return current;
		}
	}
	return nullnode;
}

xmldomnode *xmldomnode::getNextTagSibling(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	for (xmldomnode *current=getNextTagSibling();
			current && !current->isNullNode();
				current=current->getNextTagSibling()) {
		char	*nm=current->getName();
		if ((name && nm && !strcmp(name,nm)) || !name) {
			char	*value=current->
					getAttribute(attributename)->
						getValue();
			if (value && !strcmp(value,attributevalue)) {
				return current;
			}
		}
	}
	return nullnode;
}

xmldomnode *xmldomnode::getFirstTagChild() const {
	xmldomnode	*node=getChild(0);
	return (node->getType()==TAG_XMLDOMNODETYPE)?
					node:node->getNextTagSibling();
}

xmldomnode *xmldomnode::getChild(const char *name,
					const char *attributename,
					const char *attributevalue) const {
	for (xmldomnode *current=firstchild;
			current && !current->isNullNode();
				current=current->next) {
		char	*nm=current->getName();
		if ((name && nm && !strcmp(name,nm)) || !name) {
			char	*value=current->
					getAttribute(attributename)->
						getValue();
			if (value && !strcmp(value,attributevalue)) {
				return current;
			}
		}
	}
	return nullnode;
}

bool xmldomnode::insertText(const char *value, int position) {
	xmldomnode	*text=new xmldomnode(nullnode);
	text->setName("text");
	text->setValue(value);
	return insertNode(text,position,TEXT_XMLDOMNODETYPE,
				&firstchild,&lastchild,&childcount);
}

bool xmldomnode::insertAttribute(const char *name, const char *value,
								int position) {
	xmldomnode	*attribute=new xmldomnode(nullnode);
	attribute->setName(name);
	attribute->setValue(value);
	return insertNode(attribute,position,ATTRIBUTE_XMLDOMNODETYPE,
			&firstattribute,&lastattribute,&attributecount);
}

stringbuffer *xmldomnode::xml(stringbuffer *string) const {
	stringbuffer	*output=string;
	if (!string) {
		output=new stringbuffer();
	}
	xmldomnode	*current;
	if (type==ROOT_XMLDOMNODETYPE) {
		current=firstchild;
		for (int i=0; i<childcount; i++) {
			current->xml(output);
			current=current->next;
		}
	} else if (type==TAG_XMLDOMNODETYPE) {
		output->append("<");
		output->append(nodename);
		current=firstattribute;
		for (int i=0; i<attributecount; i++) {
			output->append(" ");
			current->xml(output);
			current=current->next;
		}
		if (childcount) {
			output->append(">");
			current=firstchild;
			for (int i=0; i<childcount; i++) {
				current->xml(output);
				current=current->next;
			}
			output->append("</");
			output->append(nodename);
			output->append(">");
		} else {
			if (nodename[0]=='?') {
				output->append("?>");
			} else if (nodename[0]=='!') {
				output->append(">");
			} else {
				output->append("/>");
			}
		}
	} else if (type==TEXT_XMLDOMNODETYPE) {
		output->append(nodevalue);
	} else if (type==ATTRIBUTE_XMLDOMNODETYPE) {
		if (parent->nodename[0]=='!') {
			output->append("\"");
			output->append(nodevalue);
			output->append("\"");
		} else {
			output->append(nodename);
			output->append("=\"");
			output->append(nodevalue);
			output->append("\"");
		}
	} else if (type==COMMENT_XMLDOMNODETYPE) {
		output->append("<!--");
		output->append(nodevalue);
		output->append("-->");
	} else if (type==CDATA_XMLDOMNODETYPE) {
		output->append("<![CDATA[");
		output->append(nodevalue);
		output->append("]]>");
	}
	return output;
}

xmldomnode *xmldomnode::getNode(xmldomnode *first, int position,
					const char *name, int count) const {
	if (!first || position>=count) {
		return nullnode;
	}
	xmldomnode	*current=first;
	if (name) {
		for (int i=0; i<count; i++) {
			if (!strcmp(current->nodename,name)) {
				break;
			}
			current=current->next;
		}
	} else {
		for (int i=0; i<position; i++) {
			current=current->next;
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
	node->parent=this;
	node->type=type;
	xmldomnode	*atpos=getNode(*first,position,NULL,*count);
	xmldomnode	*beforepos=getNode(*first,position-1,NULL,*count);
	if (atpos) {
		node->next=atpos;
		atpos->previous=node;
	}
	if (beforepos) {
		node->previous=beforepos;
		beforepos->next=node;
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
			(name && strcmp(current->nodename,name)) ||
			(node && current!=node)) {
			current=current->next;
		}
	} else {
		for (int i=0; i<position; i++) {
			current=current->next;
		}
	}
	if (current) {
		if (current->previous) {
			current->previous->next=current->next;
		}
		if (current->next) {
			current->next->previous=current->previous;
		}
		if (current==*first) {
			*first=current->next;
		}
		if (current==*last) {
			*last=current->previous;
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

namevaluepairs *xmldomnode::getAttributes() const {

	if (isnullnode) {
		return NULL;
	}

	namevaluepairs	*nvp=new namevaluepairs();
	for (int i=0; i<attributecount; i++) {
		nvp->setData(getAttribute(i)->getName(),
				getAttribute(i)->getValue());
	}
	return nvp;
}

void xmldomnode::cascadeOnDelete() {
	cascade=true;
}

void xmldomnode::dontCascadeOnDelete() {
	cascade=false;
}

xmldomnodetype xmldomnode::getType() const {
	return type;
}

char *xmldomnode::getName() const {
	return nodename;
}

char *xmldomnode::getValue() const {
	return nodevalue;
}

xmldomnode *xmldomnode::getParent() const {
	return parent;
}

xmldomnode *xmldomnode::getPreviousSibling() const {
	return previous;
}

xmldomnode *xmldomnode::getNextSibling() const {
	return next;
}

xmldomnode *xmldomnode::getNullNode() const {
	return nullnode;
}

bool xmldomnode::isNullNode() const {
	return isnullnode;
}

int xmldomnode::getChildCount() const {
	return childcount;
}

xmldomnode *xmldomnode::getChild(int position) const {
	return getNode(firstchild,position,NULL,childcount);
}

xmldomnode *xmldomnode::getChild(const char *name) const {
	return getNode(firstchild,0,name,childcount);
}

int xmldomnode::getAttributeCount() const {
	return attributecount;
}

xmldomnode *xmldomnode::getAttribute(int position) const {
	return getNode(firstattribute,position,NULL,attributecount);
}

xmldomnode *xmldomnode::getAttribute(const char *name) const {
	return getNode(firstattribute,0,name,attributecount);
}

char *xmldomnode::getAttributeValue(int position) const {
	return getAttribute(position)->getValue();
}

char *xmldomnode::getAttributeValue(const char *name) const {
	return getAttribute(name)->getValue();
}

void xmldomnode::setType(xmldomnodetype type) {
	this->type=type;
}

void xmldomnode::setName(const char *name) {
	nodename=(name)?strdup(name):NULL;
}

void xmldomnode::setValue(const char *value) {
	nodevalue=(value)?strdup(value):NULL;
}

void xmldomnode::setParent(xmldomnode *parent) {
	this->parent=parent;
}

void xmldomnode::setPreviousSibling(xmldomnode *previous) {
	this->previous=previous;
}

void xmldomnode::setNextSibling(xmldomnode *next) {
	this->next=next;
}

bool xmldomnode::insertChild(xmldomnode *child, int position) {
	return insertNode(child,position,child->type,
				&firstchild,&lastchild,&childcount);
}

bool xmldomnode::insertAttribute(xmldomnode *attribute, int position) {
	return insertNode(attribute,position,ATTRIBUTE_XMLDOMNODETYPE,
			&firstattribute,&lastattribute,&attributecount);
}

bool xmldomnode::deleteChild(int position) {
	return deleteNode(NULL,position,NULL,
				&firstchild,&lastchild,&childcount);
}

bool xmldomnode::deleteChild(xmldomnode *child) {
	return deleteNode(child,0,NULL,&firstchild,&lastchild,&childcount);
}

bool xmldomnode::deleteAttribute(int position) {
	return deleteNode(NULL,position,NULL,
			&firstattribute,&lastattribute,&attributecount);
}

bool xmldomnode::deleteAttribute(const char *name) {
	return deleteNode(NULL,0,name,
			&firstattribute,&lastattribute,&attributecount);
}

bool xmldomnode::deleteAttribute(xmldomnode *attribute) {
	return deleteNode(attribute,0,NULL,
			&firstattribute,&lastattribute,&attributecount);
}

stringbuffer *xmldomnode::xml() const {
	return xml(NULL);
}
