// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE void xmldomnode::cascadeOnDelete() {
	cascade=1;
}

RUDIMENTS_INLINE void xmldomnode::dontCascadeOnDelete() {
	cascade=0;
}

RUDIMENTS_INLINE xmldomnodetype xmldomnode::getType() const {
	return type;
}

RUDIMENTS_INLINE char *xmldomnode::getName() const {
	return nodename;
}

RUDIMENTS_INLINE char *xmldomnode::getValue() const {
	return nodevalue;
}

RUDIMENTS_INLINE xmldomnode *xmldomnode::getParent() const {
	return parent;
}

RUDIMENTS_INLINE xmldomnode *xmldomnode::getPreviousSibling() const {
	return previous;
}

RUDIMENTS_INLINE xmldomnode *xmldomnode::getNextSibling() const {
	return next;
}

RUDIMENTS_INLINE xmldomnode *xmldomnode::getNullNode() const {
	return nullnode;
}

RUDIMENTS_INLINE int xmldomnode::isNullNode() const {
	return isnullnode;
}

RUDIMENTS_INLINE int xmldomnode::getChildCount() const {
	return childcount;
}

RUDIMENTS_INLINE xmldomnode *xmldomnode::getChild(int position) const {
	return getNode(firstchild,position,NULL,childcount);
}

RUDIMENTS_INLINE xmldomnode *xmldomnode::getChild(const char *name) const {
	return getNode(firstchild,0,name,childcount);
}

RUDIMENTS_INLINE int xmldomnode::getAttributeCount() const {
	return attributecount;
}

RUDIMENTS_INLINE xmldomnode *xmldomnode::getAttribute(int position) const {
	return getNode(firstattribute,position,NULL,attributecount);
}

RUDIMENTS_INLINE xmldomnode *xmldomnode::getAttribute(const char *name) const {
	return getNode(firstattribute,0,name,attributecount);
}

RUDIMENTS_INLINE void xmldomnode::setType(xmldomnodetype type) {
	this->type=type;
}

RUDIMENTS_INLINE void xmldomnode::setName(const char *name) {
	nodename=(name)?strdup(name):NULL;
}

RUDIMENTS_INLINE void xmldomnode::setValue(const char *value) {
	nodevalue=(value)?strdup(value):NULL;
}

RUDIMENTS_INLINE void xmldomnode::setParent(xmldomnode *parent) {
	this->parent=parent;
}

RUDIMENTS_INLINE void xmldomnode::setPreviousSibling(xmldomnode *previous) {
	this->previous=previous;
}

RUDIMENTS_INLINE void xmldomnode::setNextSibling(xmldomnode *next) {
	this->next=next;
}

RUDIMENTS_INLINE int xmldomnode::insertChild(xmldomnode *child, int position) {
	return insertNode(child,position,child->type,
				&firstchild,&lastchild,&childcount);
}

RUDIMENTS_INLINE int xmldomnode::appendChild(xmldomnode *child) {
	return insertChild(child,getChildCount());
}

RUDIMENTS_INLINE int xmldomnode::appendText(const char *value) {
	return insertText(value,getChildCount());
}

RUDIMENTS_INLINE int xmldomnode::insertAttribute(xmldomnode *attribute,
								int position) {
	return insertNode(attribute,position,ATTRIBUTE_XMLDOMNODETYPE,
			&firstattribute,&lastattribute,&attributecount);
}

RUDIMENTS_INLINE int xmldomnode::appendAttribute(xmldomnode *attribute) {
	return insertAttribute(attribute,getAttributeCount());
}

RUDIMENTS_INLINE int xmldomnode::appendAttribute(const char *name,
							const char *value) {
	return insertAttribute(name,value,getAttributeCount());
}

RUDIMENTS_INLINE int xmldomnode::deleteChild(int position) {
	return deleteNode(NULL,position,NULL,
				&firstchild,&lastchild,&childcount);
}

RUDIMENTS_INLINE int xmldomnode::deleteChild(xmldomnode *child) {
	return deleteNode(child,0,NULL,&firstchild,&lastchild,&childcount);
}

RUDIMENTS_INLINE int xmldomnode::deleteAttribute(int position) {
	return deleteNode(NULL,position,NULL,
			&firstattribute,&lastattribute,&attributecount);
}

RUDIMENTS_INLINE int xmldomnode::deleteAttribute(const char *name) {
	return deleteNode(NULL,0,name,
			&firstattribute,&lastattribute,&attributecount);
}

RUDIMENTS_INLINE int xmldomnode::deleteAttribute(xmldomnode *attribute) {
	return deleteNode(attribute,0,NULL,
			&firstattribute,&lastattribute,&attributecount);
}

RUDIMENTS_INLINE stringbuffer *xmldomnode::xml() const {
	return xml(NULL);
}
