// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <stdlib.h>

INLINE void xmldomnode::cascadeOnDelete() {
	cascade=1;
}

INLINE void xmldomnode::dontCascadeOnDelete() {
	cascade=0;
}

INLINE xmldomnodetype xmldomnode::getType() const {
	return type;
}

INLINE char *xmldomnode::getName() const {
	return nodename;
}

INLINE char *xmldomnode::getValue() const {
	return nodevalue;
}

INLINE xmldomnode *xmldomnode::getParent() const {
	return parent;
}

INLINE xmldomnode *xmldomnode::getPreviousSibling() const {
	return previous;
}

INLINE xmldomnode *xmldomnode::getNextSibling() const {
	return next;
}

INLINE xmldomnode *xmldomnode::getNullNode() const {
	return nullnode;
}

INLINE int xmldomnode::isNullNode() const {
	return isnullnode;
}

INLINE int xmldomnode::getChildCount() const {
	return childcount;
}

INLINE xmldomnode *xmldomnode::getChild(int position) const {
	return getNode(firstchild,position,NULL,childcount);
}

INLINE xmldomnode *xmldomnode::getChild(const char *name) const {
	return getNode(firstchild,0,name,childcount);
}

INLINE int xmldomnode::getAttributeCount() const {
	return attributecount;
}

INLINE xmldomnode *xmldomnode::getAttribute(int position) const {
	return getNode(firstattribute,position,NULL,attributecount);
}

INLINE xmldomnode *xmldomnode::getAttribute(const char *name) const {
	return getNode(firstattribute,0,name,attributecount);
}

INLINE void xmldomnode::setType(xmldomnodetype type) {
	this->type=type;
}

INLINE void xmldomnode::setName(const char *name) {
	set(&(nodename),name);
}

INLINE void xmldomnode::setValue(const char *value) {
	set(&(nodevalue),value);
}

INLINE void xmldomnode::setParent(xmldomnode *parent) {
	this->parent=parent;
}

INLINE void xmldomnode::setPreviousSibling(xmldomnode *previous) {
	this->previous=previous;
}

INLINE void xmldomnode::setNextSibling(xmldomnode *next) {
	this->next=next;
}

INLINE int xmldomnode::insertChild(xmldomnode *child, int position) {
	return insertNode(child,position,child->type,
				&firstchild,&lastchild,&childcount);
}

INLINE int xmldomnode::appendChild(xmldomnode *child) {
	return insertChild(child,getChildCount());
}

INLINE int xmldomnode::appendText(const char *value) {
	return insertText(value,getChildCount());
}

INLINE int xmldomnode::insertAttribute(xmldomnode *attribute, int position) {
	return insertNode(attribute,position,ATTRIBUTE_XMLDOMNODETYPE,
			&firstattribute,&lastattribute,&attributecount);
}

INLINE int xmldomnode::appendAttribute(xmldomnode *attribute) {
	return insertAttribute(attribute,getAttributeCount());
}

INLINE int xmldomnode::appendAttribute(const char *name, const char *value) {
	return insertAttribute(name,value,getAttributeCount());
}

INLINE int xmldomnode::deleteChild(int position) {
	return deleteNode(NULL,position,NULL,
				&firstchild,&lastchild,&childcount);
}

INLINE int xmldomnode::deleteChild(xmldomnode *child) {
	return deleteNode(child,0,NULL,&firstchild,&lastchild,&childcount);
}

INLINE int xmldomnode::deleteAttribute(int position) {
	return deleteNode(NULL,position,NULL,
			&firstattribute,&lastattribute,&attributecount);
}

INLINE int xmldomnode::deleteAttribute(const char *name) {
	return deleteNode(NULL,0,name,
			&firstattribute,&lastattribute,&attributecount);
}

INLINE int xmldomnode::deleteAttribute(xmldomnode *attribute) {
	return deleteNode(attribute,0,NULL,
			&firstattribute,&lastattribute,&attributecount);
}

INLINE stringbuffer *xmldomnode::xml() const {
	return xml(NULL);
}
