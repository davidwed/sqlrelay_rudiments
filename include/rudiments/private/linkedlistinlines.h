// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <stdio.h>

#include <rudiments/private/rudimentsinlines.h>

#define LINKEDLIST_TEMPLATE template <class datatype, class linkedlistnodetype>

#define LINKEDLIST_CLASS linkedlist<datatype,linkedlistnodetype>

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLIST_CLASS::linkedlist() {
	first=NULL;
	last=NULL;
	length=0;
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
	length++;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::insert(unsigned long index, datatype data) {
	linkedlistnodetype	*node=new linkedlistnodetype();
	node->setData(data);
	return insert(index,node);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::insert(unsigned long index, linkedlistnodetype *node) {

	// handle invalid index
	if (index>length) {
		return false;
	}

	// handle append
	if (index==length) {
		append(node);
		return true;
	}

	// handle insert into index 0
	if (!index) {
		node->setNext(first);
		first->setPrevious(node);
		first=(linkedlistnodetype *)node;
		length++;
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
	length++;
	return true;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::setDataByIndex(unsigned long index,
							datatype data) {
	linkedlistnodetype	*current=getNodeByIndex(index);
	if (current) {
		current->setData(data);
		return true;
	}
	return false;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeByIndex(unsigned long index) {
	return removeNode(getNodeByIndex(index));
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeByData(datatype data) {
	linkedlistnodetype	*current=first;
	for (unsigned long i=0; i<length; i++) {
		if (!current->compare(data)) {
			return removeNode(current);
		}
		current=(linkedlistnodetype *)current->getNext();
	}
	return false;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeAllByData(datatype data) {

	linkedlistnodetype	*current=first;
	linkedlistnodetype	*next;
	for (unsigned long i=0; i<length; i++) {
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
	length--;
	return true;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::getDataByIndex(unsigned long index,
						datatype *data) const {
	linkedlistnodetype	*current=getNodeByIndex(index);
	if (current) {
		*data=current->getData();
		return true;
	}
	return false;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
unsigned long LINKEDLIST_CLASS::getLength() const {
	return length;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnodetype *LINKEDLIST_CLASS::getNodeByIndex(unsigned long index)
									const {
	if (index>=length) {
		return NULL;
	}
	linkedlistnodetype	*current=(linkedlistnodetype *)first;
	for (unsigned long i=0; i<index; i++) {
		current=(linkedlistnodetype *)current->getNext();
	}
	return current;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnodetype *LINKEDLIST_CLASS::getNodeByData(datatype data) const {
	return getNodeByData((linkedlistnodetype *)first,data);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnodetype *LINKEDLIST_CLASS::getNodeByData(
						linkedlistnodetype *startnode,
							datatype data) const {
	linkedlistnodetype	*current=startnode;
	while (current) {
		if (!current->compare(data)) {
			return current;
		}
		current=(linkedlistnodetype *)current->getNext();
	}
	return NULL;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::clear() {
	linkedlistnodetype	*current=first;
	linkedlistnodetype	*next;
	for (unsigned long i=0; i<length; i++) {
		next=(linkedlistnodetype *)current->getNext();
		delete current;
		current=next;
	}
	first=NULL;
	last=NULL;
	length=0;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::print() const {
	linkedlistnodetype	*current=first;
	for (unsigned long i=0; i<length; i++) {
		printf("index %ld: ",i);
		current->print();
		printf("\n");
		current=(linkedlistnodetype *)current->getNext();
	}
}
