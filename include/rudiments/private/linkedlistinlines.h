// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/rudimentsinlines.h>
#include <rudiments/private/linkedlistutilinlines.h>

#define LINKEDLIST_TEMPLATE template <class valuetype>

#define LINKEDLIST_CLASS linkedlist<valuetype>

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLIST_CLASS::linkedlist() {
	first=NULL;
	last=NULL;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLIST_CLASS::~linkedlist() {
	clear();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::append(valuetype value) {
	append(new linkedlistnode<valuetype>(value));
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::append(linkedlistnode<valuetype> *node) {
	if (last) {
		last->next=node;
		node->previous=last;
		last=node;
	} else {
		first=node;
		last=first;
	}
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::insert(uint64_t index, valuetype value) {
	return insert(index,new linkedlistnode<valuetype>(value));
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::insert(uint64_t index, linkedlistnode<valuetype> *node) {

	// handle insert into index 0
	if (!index) {
		node->next=first;
		first->previous=node;
		first=node;
		return true;
	}

	// handle general insert
	linkedlistnode<valuetype>	*current=getNodeByIndex(index-1);
	if (!current) {
		return false;
	}
	node->previous=current;
	node->next=current->next;
	current->next->previous=node;
	current->next=node;
	return true;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::setValueByIndex(uint64_t index, valuetype value) {
	linkedlistnode<valuetype>	*current=getNodeByIndex(index);
	if (current) {
		current->value=value;
		return true;
	}
	return false;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeByIndex(uint64_t index) {
	return removeNode(getNodeByIndex(index));
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeByValue(valuetype value) {
	for (linkedlistnode<valuetype> *current=first;
				current; current=current->next) {
		if (!current->compare(value)) {
			return removeNode(current);
		}
	}
	return false;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeAllByValue(valuetype value) {

	linkedlistnode<valuetype>	*current=first;
	linkedlistnode<valuetype>	*next;
	while (current) {
		if (!current->compare(value)) {
			next=current->next;
			if (!removeNode(current)) {
				return false;
			}
			current=next;
		} else {
			current=current->next;
		}
	}
	return true;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeNode(linkedlistnode<valuetype> *node) {
	if (!node) {
		return false;
	}
	if (node->getNext()) {
		node->next->previous=node->previous;
	}
	if (node->getPrevious()) {
		node->previous->next=node->next;
	}
	if (node==first) {
		first=node->next;
	}
	if (node==last) {
		last=node->previous;
	}
	delete node;
	return true;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::getValueByIndex(uint64_t index, valuetype *value) {
	linkedlistnode<valuetype>	*current=getNodeByIndex(index);
	if (current) {
		*value=current->value;
		return true;
	}
	return false;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
uint64_t LINKEDLIST_CLASS::getLength() const {
	uint64_t	length=0;
	for (linkedlistnode<valuetype> *current=first;
				current; current=current->next) {
		length++;
	}
	return length;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getFirstNode() {
	return first;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getLastNode() {
	return last;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getNodeByIndex(uint64_t index) {
	linkedlistnode<valuetype>	*current=first;
	for (uint64_t i=0; current && i<index; i++) {
		current=current->next;
	}
	return current;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getNodeByValue(valuetype value) {
	return getNodeByValue(first,value);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getNodeByValue(
					linkedlistnode<valuetype> *startnode,
					valuetype value) {
	for (linkedlistnode<valuetype> *current=startnode;
				current; current=current->next) {
		if (!current->compare(value)) {
			return current;
		}
	}
	return NULL;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::clear() {
	linkedlistnode<valuetype>	*next;
	linkedlistnode<valuetype>	*current=first;
	while (current) {
		next=current->next;
		delete current;
		current=next;
	}
	first=NULL;
	last=NULL;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::print() const {
	uint64_t	i=0;
	for (linkedlistnode<valuetype> *current=first;
				current; current=current->next) {
		stdoutput.printf("index %lld: ",(long long)i);
		current->print();
		stdoutput.printf("\n");
		i++;
	}
}

#define LINKEDLISTNODE_TEMPLATE template <class valuetype>

#define LINKEDLISTNODE_CLASS linkedlistnode<valuetype>

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLISTNODE_CLASS::linkedlistnode(valuetype value) {
	this->value=value;
	previous=NULL;
	next=NULL;
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLISTNODE_CLASS::~linkedlistnode() {
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLISTNODE_CLASS::setValue(valuetype value) {
	this->value=value;
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
valuetype LINKEDLISTNODE_CLASS::getValue() const {
	return value;
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLISTNODE_CLASS::setPrevious(LINKEDLISTNODE_CLASS *previous) {
	this->previous=previous;
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLISTNODE_CLASS::setNext(LINKEDLISTNODE_CLASS *next) {
	this->next=next;
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLISTNODE_CLASS *LINKEDLISTNODE_CLASS::getPrevious() {
	return previous;
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLISTNODE_CLASS *LINKEDLISTNODE_CLASS::getNext() {
	return next;
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
int32_t LINKEDLISTNODE_CLASS::compare(valuetype value) const {
	return _linkedlistutil_compare(this->value,value);
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLISTNODE_CLASS::print() const {
	_linkedlistutil_print(value);
}
