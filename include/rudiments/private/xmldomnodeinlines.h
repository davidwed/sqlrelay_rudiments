// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <stdlib.h>

inline void xmldomnode::cascadeOnDelete() {
	cascade=1;
}

inline void xmldomnode::dontCascadeOnDelete() {
	cascade=0;
}

inline xmldomnodetype xmldomnode::getType() const {
	return type;
}

inline char *xmldomnode::getName() const {
	return nodename;
}

inline char *xmldomnode::getValue() const {
	return nodevalue;
}

inline xmldomnode *xmldomnode::getParent() const {
	return parent;
}

inline xmldomnode *xmldomnode::getPreviousSibling() const {
	return previous;
}

inline xmldomnode *xmldomnode::getNextSibling() const {
	return next;
}

inline xmldomnode *xmldomnode::getNullNode() const {
	return nullnode;
}

inline int xmldomnode::isNullNode() const {
	return isnullnode;
}

inline int xmldomnode::getChildCount() const {
	return childcount;
}

inline xmldomnode *xmldomnode::getChild(int position) const {
	return getNode(firstchild,position,NULL,childcount);
}

inline xmldomnode *xmldomnode::getChild(const char *name) const {
	return getNode(firstchild,0,name,childcount);
}

inline int xmldomnode::getAttributeCount() const {
	return attributecount;
}

inline xmldomnode *xmldomnode::getAttribute(int position) const {
	return getNode(firstattribute,position,NULL,attributecount);
}

inline xmldomnode *xmldomnode::getAttribute(const char *name) const {
	return getNode(firstattribute,0,name,attributecount);
}

inline void xmldomnode::setType(xmldomnodetype type) {
	this->type=type;
}

inline void xmldomnode::setName(const char *name) {
	set(&(nodename),name);
}

inline void xmldomnode::setValue(const char *value) {
	set(&(nodevalue),value);
}

inline void xmldomnode::setParent(xmldomnode *parent) {
	this->parent=parent;
}

inline void xmldomnode::setPreviousSibling(xmldomnode *previous) {
	this->previous=previous;
}

inline void xmldomnode::setNextSibling(xmldomnode *next) {
	this->next=next;
}

inline int xmldomnode::insertChild(xmldomnode *child, int position) {
	return insertNode(child,position,child->type,
				&firstchild,&lastchild,&childcount);
}

inline int xmldomnode::appendChild(xmldomnode *child) {
	return insertChild(child,getChildCount());
}

inline int xmldomnode::appendText(const char *value) {
	return insertText(value,getChildCount());
}

inline int xmldomnode::insertAttribute(xmldomnode *attribute, int position) {
	return insertNode(attribute,position,ATTRIBUTE_XMLDOMNODETYPE,
			&firstattribute,&lastattribute,&attributecount);
}

inline int xmldomnode::appendAttribute(xmldomnode *attribute) {
	return insertAttribute(attribute,getAttributeCount());
}

inline int xmldomnode::appendAttribute(const char *name, const char *value) {
	return insertAttribute(name,value,getAttributeCount());
}

inline int xmldomnode::deleteChild(int position) {
	return deleteNode(NULL,position,NULL,
				&firstchild,&lastchild,&childcount);
}

inline int xmldomnode::deleteChild(xmldomnode *child) {
	return deleteNode(child,0,NULL,&firstchild,&lastchild,&childcount);
}

inline int xmldomnode::deleteAttribute(int position) {
	return deleteNode(NULL,position,NULL,
			&firstattribute,&lastattribute,&attributecount);
}

inline int xmldomnode::deleteAttribute(const char *name) {
	return deleteNode(NULL,0,name,
			&firstattribute,&lastattribute,&attributecount);
}

inline int xmldomnode::deleteAttribute(xmldomnode *attribute) {
	return deleteNode(attribute,0,NULL,
			&firstattribute,&lastattribute,&attributecount);
}

inline stringbuffer *xmldomnode::xml() const {
	return xml(NULL);
}
