// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <stdio.h>

#define LIST_TEMPLATE template <class datatype, class listnodetype>

#define LIST_CLASS list<datatype,listnodetype>

LIST_TEMPLATE
inline LIST_CLASS::list() {
	first=NULL;
	last=NULL;
	length=0;
}

LIST_TEMPLATE
inline LIST_CLASS::~list() {
	clear();
}

LIST_TEMPLATE
void LIST_CLASS::append(datatype data) {
	listnodetype	*newnode=new listnodetype();
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

LIST_TEMPLATE
int LIST_CLASS::insert(unsigned long index, datatype data) {

	// handle invalid index
	if (index>length) {
		return 0;
	}

	// handle append
	if (index==length) {
		append(data);
		return 1;
	}

	// handle insert into index 0
	listnodetype	*newnode=new listnodetype();
	if (!index) {
		newnode->setData(data);
		newnode->setNext(first);
		first->setPrevious(newnode);
		first=(listnodetype *)newnode;
		length++;
		return 1;
	}

	// handle general insert
	listnodetype	*current=getNodeByIndex(index-1);
	if (!current) {
		return 0;
	}
	newnode->setData(data);
	newnode->setPrevious(current);
	newnode->setNext(current->getNext());
	current->getNext()->setPrevious(newnode);
	current->setNext(newnode);
	length++;
	return 1;
}

LIST_TEMPLATE
inline int LIST_CLASS::setDataByIndex(unsigned long index, datatype data) {
	listnodetype	*current=getNodeByIndex(index);
	if (current) {
		current->setData(data);
		return 1;
	}
	return 0;
}

LIST_TEMPLATE
inline int LIST_CLASS::removeByIndex(unsigned long index) {
	return removeNode(getNodeByIndex(index));
}

LIST_TEMPLATE
int LIST_CLASS::removeByData(datatype data) {
	listnodetype	*current=first;
	for (unsigned long i=0; i<length; i++) {
		if (!current->compare(data)) {
			return removeNode(current);
		}
		current=(listnodetype *)current->getNext();
	}
	return 0;
}

LIST_TEMPLATE
int LIST_CLASS::removeAllByData(datatype data) {

	listnodetype	*current=first;
	listnodetype	*next;
	for (unsigned long i=0; i<length; i++) {
		if (!current->compare(data)) {
			next=(listnodetype *)current->getNext();
			if (!removeNode(current)) {
				return 0;
			}
			current=next;
		} else {
			current=(listnodetype *)current->getNext();
		}
	}
	return 1;
}

LIST_TEMPLATE
int LIST_CLASS::removeNode(listnodetype *node) {
	if (!node) {
		return 0;
	}
	if (node->getNext()) {
		node->getNext()->setPrevious(node->getPrevious());
	}
	if (node->getPrevious()) {
		node->getPrevious()->setNext(node->getNext());
	}
	if (node==first) {
		first=(listnodetype *)node->getNext();
	}
	if (node==last) {
		last=(listnodetype *)node->getPrevious();
	}
	delete node;
	length--;
	return 1;
}

LIST_TEMPLATE
inline int LIST_CLASS::getDataByIndex(unsigned long index,
						datatype *data) const {
	listnodetype	*current=getNodeByIndex(index);
	if (current) {
		*data=current->getData();
		return 1;
	}
	return 0;
}

LIST_TEMPLATE
inline unsigned long LIST_CLASS::getLength() const {
	return length;
}

LIST_TEMPLATE
listnodetype *LIST_CLASS::getNodeByIndex(unsigned long index) const {
	if (index>=length) {
		return NULL;
	}
	listnodetype	*current=(listnodetype *)first;
	for (unsigned long i=0; i<index; i++) {
		current=(listnodetype *)current->getNext();
	}
	return current;
}

LIST_TEMPLATE
listnodetype *LIST_CLASS::getNodeByData(datatype data) const {
	return getNodeByData((listnodetype *)first,data);
}

LIST_TEMPLATE
listnodetype *LIST_CLASS::getNodeByData(listnodetype *startnode,
						datatype data) const {
	listnodetype	*current=startnode;
	while (current) {
		if (!current->compare(data)) {
			return current;
		}
		current=(listnodetype *)current->getNext();
	}
	return NULL;
}

LIST_TEMPLATE
void LIST_CLASS::clear() {
	listnodetype	*current=first;
	listnodetype	*next;
	for (unsigned long i=0; i<length; i++) {
		next=(listnodetype *)current->getNext();
		delete current;
		current=next;
	}
	first=NULL;
	last=NULL;
	length=0;
}

LIST_TEMPLATE
void LIST_CLASS::print() const {
	listnodetype	*current=first;
	for (unsigned long i=0; i<length; i++) {
		printf("index %d: ",i);
		current->print();
		current=(listnodetype *)current->getNext();
	}
}
