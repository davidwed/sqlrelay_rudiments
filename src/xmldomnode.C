// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/xmldomnode.h>
#ifndef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/xmldomnodeinlines.h>
#endif

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
	cascade=1;
	isnullnode=0;
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
	cascade=1;
	isnullnode=0;
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
	nn->isnullnode=1;
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

int xmldomnode::insertText(const char *value, int position) {
	xmldomnode	*text=new xmldomnode(nullnode);
	text->setName("text");
	text->setValue(value);
	return insertNode(text,position,TEXT_XMLDOMNODETYPE,
				&firstchild,&lastchild,&childcount);
}

int xmldomnode::insertAttribute(const char *name, const char *value,
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

int xmldomnode::insertNode(xmldomnode *node, int position,
				xmldomnodetype type,
				xmldomnode **first, xmldomnode **last,
				int *count) {
	if (position>(*count)) {
		return 0;
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
	return 1;
}

int xmldomnode::deleteNode(xmldomnode *node, int position, const char *name,
				xmldomnode **first, xmldomnode **last,
				int *count) {

	if (position>(*count)) {
		return 0;
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
		return 1;
	}
	return 0;
}
