// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <stdio.h>

#define LIST_TEMPLATE \
	template <class datatype, class keytype, class listnodetype>

#define LIST_CLASS \
	list<datatype,keytype,listnodetype>

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
	if (last) {
		last->setNext(new listnodetype(data,last,NULL));
		last=(listnodetype *)last->getNext();
	} else {
		first=(listnodetype *)new listnodetype(data);
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
	listnodetype	*newnode;
	if (!index) {
		newnode=new listnodetype(data,NULL,first);
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
	newnode=(listnodetype *)new listnodetype(data,
					(listnodetype *)current,
					(listnodetype *)current->getNext());
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
int LIST_CLASS::removeByKey(keytype key) {

	listnodetype	*current=first;
	for (unsigned long i=0; i<length; i++) {
		if (!current->compare(key)) {
			return removeNode(current);
		}
		current=(listnodetype *)current->getNext();
	}
	return 0;
}

LIST_TEMPLATE
int LIST_CLASS::removeAllByKey(keytype key) {

	listnodetype	*current=first;
	listnodetype	*next;
	for (unsigned long i=0; i<length; i++) {
		if (!current->compare(key)) {
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
inline int LIST_CLASS::getDataByKey(keytype key, datatype *data) const {
	for (listnodetype *current=first; current;
			current=(listnodetype *)current->getNext()) {
		if (!current->compare(key)) {
			*data=current->getData();
			return 1;
		}
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
	listnodetype	*current=first;
	for (unsigned long i=0; i<index; i++) {
		current=(listnodetype *)current->getNext();
	}
	return current;
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
