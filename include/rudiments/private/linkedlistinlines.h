// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#ifndef EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <rudiments/stdio.h>

#include <rudiments/private/rudimentsinlines.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#define LINKEDLIST_TEMPLATE template <class datatype, class linkedlistnodetype>

#define LINKEDLIST_CLASS linkedlist<datatype,linkedlistnodetype>

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
void LINKEDLIST_CLASS::append(datatype data) {
	linkedlistnodetype	*node=new linkedlistnodetype();
	node->setData(data);
	append(node);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::append(linkedlistnodetype *node) {
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
bool LINKEDLIST_CLASS::insert(uint64_t index, datatype data) {
	linkedlistnodetype	*node=new linkedlistnodetype();
	node->setData(data);
	return insert(index,node);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::insert(uint64_t index, linkedlistnodetype *node) {

	// handle insert into index 0
	if (!index) {
		node->setNext(first);
		first->setPrevious(node);
		first=(linkedlistnodetype *)node;
		return true;
	}

	// handle general insert
	linkedlistnodetype	*current=getNodeByIndex(index-1);
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
bool LINKEDLIST_CLASS::setDataByIndex(uint64_t index, datatype data) {
	linkedlistnodetype	*current=getNodeByIndex(index);
	if (current) {
		current->setData(data);
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
bool LINKEDLIST_CLASS::removeByData(datatype data) {
	for (linkedlistnodetype *current=first; current;
			current=(linkedlistnodetype *)current->getNext()) {
		if (!current->compare(data)) {
			return removeNode(current);
		}
	}
	return false;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeAllByData(datatype data) {

	linkedlistnodetype	*current=first;
	linkedlistnodetype	*next;
	while (current) {
		if (!current->compare(data)) {
			next=(linkedlistnodetype *)current->getNext();
			if (!removeNode(current)) {
				return false;
			}
			current=next;
		} else {
			current=(linkedlistnodetype *)current->getNext();
		}
	}
	return true;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeNode(linkedlistnodetype *node) {
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
		first=(linkedlistnodetype *)node->getNext();
	}
	if (node==last) {
		last=(linkedlistnodetype *)node->getPrevious();
	}
	delete node;
	return true;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::getDataByIndex(uint64_t index, datatype *data) {
	linkedlistnodetype	*current=getNodeByIndex(index);
	if (current) {
		*data=current->getData();
		return true;
	}
	return false;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
uint64_t LINKEDLIST_CLASS::getLength() const {
	uint64_t	length=0;
	for (linkedlistnodetype *current=first; current;
			current=(linkedlistnodetype *)current->getNext()) {
		length++;
	}
	return length;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnodetype *LINKEDLIST_CLASS::getFirstNode() {
	return (linkedlistnodetype *)first;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnodetype *LINKEDLIST_CLASS::getLastNode() {
	return (linkedlistnodetype *)last;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnodetype *LINKEDLIST_CLASS::getNodeByIndex(uint64_t index) {
	linkedlistnodetype	*current=(linkedlistnodetype *)first;
	for (uint64_t i=0; current && i<index; i++) {
		current=(linkedlistnodetype *)current->getNext();
	}
	return current;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnodetype *LINKEDLIST_CLASS::getNodeByData(datatype data) {
	return getNodeByData((linkedlistnodetype *)first,data);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnodetype *LINKEDLIST_CLASS::getNodeByData(
						linkedlistnodetype *startnode,
							datatype data) {
	for (linkedlistnodetype *current=startnode; current;
			current=(linkedlistnodetype *)current->getNext()) {
		if (!current->compare(data)) {
			return current;
		}
	}
	return NULL;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::clear() {
	linkedlistnodetype	*next;
	linkedlistnodetype	*current=first;
	while (current) {
		next=(linkedlistnodetype *)current->getNext();
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
	for (linkedlistnodetype *current=first; current;
		current=(linkedlistnodetype *)current->getNext()) {
		stdoutput.printf("index %lld: ",(long long)i);
		current->print();
		stdoutput.printf("\n");
		i++;
	}
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
