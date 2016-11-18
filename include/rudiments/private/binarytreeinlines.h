// Copyright (c) 2016 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/rudimentsinlines.h>
#include <rudiments/private/binarytreeutilinlines.h>

#define BINARYTREE_TEMPLATE template <class valuetype>

#define BINARYTREE_CLASS binarytree<valuetype>

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREE_CLASS::binarytree() {
	top=NULL;
	first=NULL;
	last=NULL;
	length=0;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREE_CLASS::~binarytree() {
	clear();
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREE_CLASS::insert(valuetype value) {
	insert(new binarytreenode<valuetype>(value));
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREE_CLASS::insert(binarytreenode<valuetype> *node) {
	if (!node) {
		return;
	} else if (first) {
		// FIXME: do something... 
	} else {
		// FIXME: do something... 
	}
	length++;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREE_CLASS::detach(binarytreenode<valuetype> *node) {

	if (node==first) {
		// FIXME: do something... 
	}
	if (node==last) {
		// FIXME: do something... 
	}
	node->setParent(NULL);
	node->setLeftChild(NULL);
	node->setRightChild(NULL);
	length--;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool BINARYTREE_CLASS::remove(valuetype value) {
	binarytreenode<valuetype>	*current=find(value);
	return (current)?remove(current):false;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool BINARYTREE_CLASS::removeAll(valuetype value) {

	binarytreenode<valuetype>	*current=first;
	binarytreenode<valuetype>	*next;
	while (current) {
		next=current->getNext();
		if (!current->compare(value) && !remove(current)) {
			return false;
		}
		current=next;
	}
	return true;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool BINARYTREE_CLASS::remove(binarytreenode<valuetype> *node) {
	if (!node) {
		return false;
	}
	if (node->getNext()) {
		// FIXME: do something...
	}
	if (node->getPrevious()) {
		// FIXME: do something...
	}
	if (node==first) {
		// FIXME: do something...
	}
	if (node==last) {
		// FIXME: do something...
	}
	delete node;
	length--;
	return true;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
uint64_t BINARYTREE_CLASS::getLength() const {
	return length;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREE_CLASS::getFirst() {
	return first;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREE_CLASS::getLast() {
	return last;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREE_CLASS::getPrevious(
					binarytreenode<valuetype> *node) {
	return (node)?node->getPrevious():NULL;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREE_CLASS::getNext(
					binarytreenode<valuetype> *node) {
	return (node)?node->getNext():NULL;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREE_CLASS::find(valuetype value) {
	return find((binarytreenode<valuetype> *)first,value);
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREE_CLASS::find(
					binarytreenode<valuetype> *startnode,
					valuetype value) {
	for (binarytreenode<valuetype> *current=startnode;
			current; current=current->getNext()) {
		if (!current->compare(value)) {
			return current;
		}
	}
	return NULL;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREE_CLASS::sort() {
	// FIXME: do something...
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREE_CLASS::clear() {
	// FIXME: do something...
	first=NULL;
	last=NULL;
	length=0;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREE_CLASS::print() const {
	// FIXME: do something...
}

#define BINARYTREENODE_TEMPLATE template <class valuetype>

#define BINARYTREENODE_CLASS binarytreenode<valuetype>

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREENODE_CLASS::binarytreenode(valuetype value) {
	this->value=value;
	parent=NULL;
	left=NULL;
	right=NULL;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREENODE_CLASS::~binarytreenode() {
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREENODE_CLASS::setValue(valuetype value) {
	this->value=value;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
valuetype BINARYTREENODE_CLASS::getValue() const {
	return value;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREENODE_CLASS *BINARYTREENODE_CLASS::getParent() {
	return parent;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREENODE_CLASS *BINARYTREENODE_CLASS::getLeftChild() {
	return left;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREENODE_CLASS *BINARYTREENODE_CLASS::getRightChild() {
	return right;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREENODE_CLASS *BINARYTREENODE_CLASS::getPrevious() {
	// FIXME: do something...
	return NULL;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREENODE_CLASS *BINARYTREENODE_CLASS::getNext() {
	// FIXME: do something...
	return NULL;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
int32_t BINARYTREENODE_CLASS::compare(valuetype value) const {
	return _binarytreeutil_compare(this->value,value);
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
int32_t BINARYTREENODE_CLASS::compare(binarytreenode<valuetype> *peer) const {
	return _binarytreeutil_compare(this->value,peer->value);
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREENODE_CLASS::print() const {
	_binarytreeutil_print(value);
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREENODE_CLASS::setParent(BINARYTREENODE_CLASS *node) {
	parent=node;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREENODE_CLASS::setLeftChild(BINARYTREENODE_CLASS *node) {
	left=node;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREENODE_CLASS::setRightChild(BINARYTREENODE_CLASS *node) {
	right=node;
}
