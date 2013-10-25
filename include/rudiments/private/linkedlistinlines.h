// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/rudimentsinlines.h>

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
	linkedlistnode<valuetype>	*node=new linkedlistnode<valuetype>();
	node->setValue(value);
	append(node);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::append(linkedlistnode<valuetype> *node) {
	if (last) {
		last->setNext(node);
		node->setPrevious(last);
		last=node;
	} else {
		first=node;
		last=first;
	}
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::insert(uint64_t index, valuetype value) {
	linkedlistnode<valuetype>	*node=new linkedlistnode<valuetype>();
	node->setValue(value);
	return insert(index,node);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::insert(uint64_t index, linkedlistnode<valuetype> *node) {

	// handle insert into index 0
	if (!index) {
		node->setNext(first);
		first->setPrevious(node);
		first=(linkedlistnode<valuetype> *)node;
		return true;
	}

	// handle general insert
	linkedlistnode<valuetype>	*current=getNodeByIndex(index-1);
	if (!current) {
		return false;
	}
	node->setPrevious(current);
	node->setNext(current->getNext());
	current->getNext()->setPrevious(node);
	current->setNext(node);
	return true;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::setValueByIndex(uint64_t index, valuetype value) {
	linkedlistnode<valuetype>	*current=getNodeByIndex(index);
	if (current) {
		current->setValue(value);
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
	for (linkedlistnode<valuetype> *current=first; current;
		current=(linkedlistnode<valuetype> *)current->getNext()) {
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
			next=(linkedlistnode<valuetype> *)current->getNext();
			if (!removeNode(current)) {
				return false;
			}
			current=next;
		} else {
			current=(linkedlistnode<valuetype> *)current->getNext();
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
		node->getNext()->setPrevious(node->getPrevious());
	}
	if (node->getPrevious()) {
		node->getPrevious()->setNext(node->getNext());
	}
	if (node==first) {
		first=(linkedlistnode<valuetype> *)node->getNext();
	}
	if (node==last) {
		last=(linkedlistnode<valuetype> *)node->getPrevious();
	}
	delete node;
	return true;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::getValueByIndex(uint64_t index, valuetype *value) {
	linkedlistnode<valuetype>	*current=getNodeByIndex(index);
	if (current) {
		*value=current->getValue();
		return true;
	}
	return false;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
uint64_t LINKEDLIST_CLASS::getLength() const {
	uint64_t	length=0;
	for (linkedlistnode<valuetype> *current=first; current;
		current=(linkedlistnode<valuetype> *)current->getNext()) {
		length++;
	}
	return length;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getFirstNode() {
	return (linkedlistnode<valuetype> *)first;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getLastNode() {
	return (linkedlistnode<valuetype> *)last;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getNodeByIndex(uint64_t index) {
	linkedlistnode<valuetype>	*current=
					(linkedlistnode<valuetype> *)first;
	for (uint64_t i=0; current && i<index; i++) {
		current=(linkedlistnode<valuetype> *)current->getNext();
	}
	return current;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getNodeByValue(valuetype value) {
	return getNodeByValue((linkedlistnode<valuetype> *)first,value);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getNodeByValue(
					linkedlistnode<valuetype> *startnode,
					valuetype value) {
	for (linkedlistnode<valuetype> *current=startnode; current;
		current=(linkedlistnode<valuetype> *)current->getNext()) {
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
		next=(linkedlistnode<valuetype> *)current->getNext();
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
	for (linkedlistnode<valuetype> *current=first; current;
		current=(linkedlistnode<valuetype> *)current->getNext()) {
		stdoutput.printf("index %lld: ",(long long)i);
		current->print();
		stdoutput.printf("\n");
		i++;
	}
}
