// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <stdio.h>

template <class datatype, class keytype, class listnodetype>
inline list<datatype,keytype,listnodetype>::list() {
	first=NULL;
	last=NULL;
	length=0;
}

template <class datatype, class keytype, class listnodetype>
inline list<datatype,keytype,listnodetype>::~list() {
	clear();
}

template <class datatype, class keytype, class listnodetype>
void list<datatype,keytype,listnodetype>::append(datatype value) {
	if (last) {
		last->setNext(new listnodetype(value,last,NULL));
		last=(listnodetype *)last->getNext();
	} else {
		first=(listnodetype *)new listnodetype(value);
		last=first;
	}
	length++;
}

template <class datatype, class keytype, class listnodetype>
int list<datatype,keytype,listnodetype>::insert(unsigned long index,
							datatype value) {

	// handle invalid index
	if (index>length) {
		return 0;
	}

	// handle append
	if (index==length) {
		append(value);
		return 1;
	}

	// handle insert into index 0
	listnodetype	*newnode;
	if (!index) {
		newnode=new listnodetype(value,NULL,first);
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
	newnode=(listnodetype *)new listnodetype(value,
					(listnodetype *)current,
					(listnodetype *)current->getNext());
	current->getNext()->setPrevious(newnode);
	current->setNext(newnode);
	length++;
	return 1;
}

template <class datatype, class keytype, class listnodetype>
inline int list<datatype,keytype,listnodetype>::setValueByIndex(
							unsigned long index,
							datatype value) {
	listnodetype	*current=getNodeByIndex(index);
	if (current) {
		current->setValue(value);
		return 1;
	}
	return 0;
}

template <class datatype, class keytype, class listnodetype>
inline int list<datatype,keytype,listnodetype>::removeByIndex(
							unsigned long index) {
	return removeNode(getNodeByIndex(index));
}

template <class datatype, class keytype, class listnodetype>
int list<datatype,keytype,listnodetype>::removeByKey(keytype key) {

	listnodetype	*current=first;
	for (unsigned long i=0; i<length; i++) {
		if (!current->compare(key)) {
			return removeNode(current);
		}
		current=(listnodetype *)current->getNext();
	}
	return 0;
}

template <class datatype, class keytype, class listnodetype>
int list<datatype,keytype,listnodetype>::removeAllByKey(keytype key) {

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

template <class datatype, class keytype, class listnodetype>
int list<datatype,keytype,listnodetype>::removeNode(listnodetype *node) {
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

template <class datatype, class keytype, class listnodetype>
inline int list<datatype,keytype,listnodetype>::getValueByIndex(
							unsigned long index,
							datatype *value) const {
	listnodetype	*current=getNodeByIndex(index);
	if (current) {
		*value=current->getValue();
		return 1;
	}
	return 0;
}

template <class datatype, class keytype, class listnodetype>
inline int list<datatype,keytype,listnodetype>::getValueByKey(
							keytype key,
							datatype *value) const {
	for (listnodetype *current=first; current;
			current=(listnodetype *)current->getNext()) {
		if (!current->compare(key)) {
			*value=current->getValue();
			return 1;
		}
	}
	return 0;
}

template <class datatype, class keytype, class listnodetype>
inline unsigned long list<datatype,keytype,listnodetype>::getLength() const {
	return length;
}

template <class datatype, class keytype, class listnodetype>
listnodetype *list<datatype,keytype,listnodetype>::getNodeByIndex(
						unsigned long index) const {
	if (index>=length) {
		return NULL;
	}
	listnodetype	*current=first;
	for (unsigned long i=0; i<index; i++) {
		current=(listnodetype *)current->getNext();
	}
	return current;
}

template <class datatype, class keytype, class listnodetype>
void list<datatype,keytype,listnodetype>::clear() {
	listnodetype	*current=first;
	listnodetype	*next;
	for (unsigned long i=0; i<length; i++) {
		next=(listnodetype *)current->getNext();
		delete current;
		current=next;
	}
	length=0;
}
