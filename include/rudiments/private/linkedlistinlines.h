// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <stdio.h>

#define LINKEDLIST_TEMPLATE template <class datatype, class linkedlistnodetype>

#define LINKEDLIST_CLASS linkedlist<datatype,linkedlistnodetype>

LINKEDLIST_TEMPLATE
inline LINKEDLIST_CLASS::linkedlist() {
	first=NULL;
	last=NULL;
	length=0;
}

LINKEDLIST_TEMPLATE
inline LINKEDLIST_CLASS::~linkedlist() {
	clear();
}

LINKEDLIST_TEMPLATE
void LINKEDLIST_CLASS::append(datatype data) {
	linkedlistnodetype	*newnode=new linkedlistnodetype();
	newnode->setData(data);
	if (last) {
		last->setNext(newnode);
		newnode->setPrevious(last);
		last=newnode;
	} else {
		first=newnode;
		last=first;
	}
	length++;
}

LINKEDLIST_TEMPLATE
bool LINKEDLIST_CLASS::insert(unsigned long index, datatype data) {

	// handle invalid index
	if (index>length) {
		return false;
	}

	// handle append
	if (index==length) {
		append(data);
		return true;
	}

	// handle insert into index 0
	linkedlistnodetype	*newnode=new linkedlistnodetype();
	if (!index) {
		newnode->setData(data);
		newnode->setNext(first);
		first->setPrevious(newnode);
		first=(linkedlistnodetype *)newnode;
		length++;
		return true;
	}

	// handle general insert
	linkedlistnodetype	*current=getNodeByIndex(index-1);
	if (!current) {
		return false;
	}
	newnode->setData(data);
	newnode->setPrevious(current);
	newnode->setNext(current->getNext());
	current->getNext()->setPrevious(newnode);
	current->setNext(newnode);
	length++;
	return true;
}

LINKEDLIST_TEMPLATE
inline bool LINKEDLIST_CLASS::setDataByIndex(unsigned long index,
							datatype data) {
	linkedlistnodetype	*current=getNodeByIndex(index);
	if (current) {
		current->setData(data);
		return true;
	}
	return false;
}

LINKEDLIST_TEMPLATE
inline bool LINKEDLIST_CLASS::removeByIndex(unsigned long index) {
	return removeNode(getNodeByIndex(index));
}

LINKEDLIST_TEMPLATE
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
inline bool LINKEDLIST_CLASS::getDataByIndex(unsigned long index,
						datatype *data) const {
	linkedlistnodetype	*current=getNodeByIndex(index);
	if (current) {
		*data=current->getData();
		return true;
	}
	return false;
}

LINKEDLIST_TEMPLATE
inline unsigned long LINKEDLIST_CLASS::getLength() const {
	return length;
}

LINKEDLIST_TEMPLATE
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
linkedlistnodetype *LINKEDLIST_CLASS::getNodeByData(datatype data) const {
	return getNodeByData((linkedlistnodetype *)first,data);
}

LINKEDLIST_TEMPLATE
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
void LINKEDLIST_CLASS::print() const {
	linkedlistnodetype	*current=first;
	for (unsigned long i=0; i<length; i++) {
		printf("index %ld: ",i);
		current->print();
		printf("\n");
		current=(linkedlistnodetype *)current->getNext();
	}
}
