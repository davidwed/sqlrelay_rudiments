// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <stdio.h>

template <class type>
inline list<type>::list() {
	first=NULL;
	last=NULL;
	length=0;
}

template <class type>
inline list<type>::~list() {
	clear();
}

template <class type>
void list<type>::append(type value) {
	if (last) {
		last->setNext(new listnode<type>(value,last,NULL));
		last=last->getNext();
	} else {
		first=new listnode<type>(value);
		last=first;
	}
	length++;
}

template <class type>
int list<type>::insert(unsigned long index, type value) {

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
	listnode<type>	*newnode;
	if (!index) {
		newnode=new listnode<type>(value,NULL,first);
		first->setPrevious(newnode);
		first=newnode;
		length++;
		return 1;
	}

	// handle general insert
	listnode<type>	*current=getNode(index-1);
	if (!current) {
		return 0;
	}
	newnode=new listnode<type>(value,current,current->getNext());
	current->getNext()->setPrevious(newnode);
	current->setNext(newnode);
	length++;
	return 1;
}

template <class type>
inline int list<type>::setValue(unsigned long index, type value) {
	listnode<type>	*current=getNode(index);
	if (current) {
		current->setValue(value);
		return 1;
	}
	return 0;
}

template <class type>
inline int list<type>::removeIndex(unsigned long index) {
	return removeNode(getNode(index));
}

template <class type>
int list<type>::removeValue(type value) {

	listnode<type>	*current=first;
	for (unsigned long i=0; i<length; i++) {
		if (current->getValue()==value) {
			return removeNode(current);
		}
		current=current->getNext();
	}
	return 0;
}

template <class type>
int list<type>::removeAllValues(type value) {

	listnode<type>	*current=first;
	listnode<type>	*next;
	for (unsigned long i=0; i<length; i++) {
		if (current->getValue()==value) {
			next=current->getNext();
			if (!removeNode(current)) {
				return 0;
			}
			current=next;
		} else {
			current=current->getNext();
		}
	}
	return 1;
}

template <class type>
int list<type>::removeNode(listnode<type> *node) {
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
		first=node->getNext();
	}
	if (node==last) {
		last=node->getPrevious();
	}
	delete node;
	length--;
	return 1;
}

template <class type>
inline int list<type>::getValue(unsigned long index, type *value) const {
	listnode<type>	*current=getNode(index);
	if (current) {
		*value=current->getValue();
		return 1;
	}
	return 0;
}

template <class type>
inline unsigned long list<type>::getLength() const {
	return length;
}

template <class type>
listnode<type> *list<type>::getNode(unsigned long index) const {
	if (index>length) {
		return NULL;
	}
	listnode<type>	*current=first;
	for (unsigned long i=0; i<index; i++) {
		current=current->getNext();
	}
	return current;
}

template <class type>
void list<type>::clear() {
	listnode<type>	*current=first;
	listnode<type>	*next;
	for (unsigned long i=0; i<length; i++) {
		next=current->getNext();
		delete current;
		current=next;
	}
	length=0;
}
