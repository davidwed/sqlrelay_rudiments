// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <stdio.h>

template <class type>
inline linkedlist<type>::linkedlist() {
	first=NULL;
	last=NULL;
	length=0;
}

template <class type>
inline linkedlist<type>::~linkedlist() {
	clear();
}

template <class type>
void linkedlist<type>::append(type value) {
	if (last) {
		last->setNext(new linkedlistnode<type>(value,last,NULL));
		last=last->getNext();
	} else {
		first=new linkedlistnode<type>(value);
		last=first;
	}
	length++;
}

template <class type>
int linkedlist<type>::insert(unsigned long index, type value) {

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
	linkedlistnode<type>	*newnode;
	if (!index) {
		newnode=new linkedlistnode<type>(value,NULL,first);
		first->setPrevious(newnode);
		first=newnode;
		length++;
		return 1;
	}

	// handle general insert
	linkedlistnode<type>	*current=getNode(index-1);
	if (!current) {
		return 0;
	}
	newnode=new linkedlistnode<type>(value,current,current->getNext());
	current->getNext()->setPrevious(newnode);
	current->setNext(newnode);
	length++;
	return 1;
}

template <class type>
inline int linkedlist<type>::setValue(unsigned long index, type value) {
	linkedlistnode<type>	*current=getNode(index);
	if (current) {
		current->setValue(value);
		return 1;
	}
	return 0;
}

template <class type>
inline int linkedlist<type>::removeIndex(unsigned long index) {
	return removeNode(getNode(index));
}

template <class type>
int linkedlist<type>::removeValue(type value) {

	linkedlistnode<type>	*current=first;
	for (unsigned long i=0; i<length; i++) {
		if (current->getValue()==value) {
			return removeNode(current);
		}
		current=current->getNext();
	}
	return 0;
}

template <class type>
int linkedlist<type>::removeAllValues(type value) {

	linkedlistnode<type>	*current=first;
	linkedlistnode<type>	*next;
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
int linkedlist<type>::removeNode(linkedlistnode<type> *node) {
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
inline int linkedlist<type>::getValue(unsigned long index, type *value) const {
	linkedlistnode<type>	*current=getNode(index);
	if (current) {
		*value=current->getValue();
		return 1;
	}
	return 0;
}

template <class type>
inline unsigned long linkedlist<type>::getLength() const {
	return length;
}

template <class type>
linkedlistnode<type> *linkedlist<type>::getNode(unsigned long index) const {
	if (index>length) {
		return NULL;
	}
	linkedlistnode<type>	*current=first;
	for (unsigned long i=0; i<index; i++) {
		current=current->getNext();
	}
	return current;
}

template <class type>
void linkedlist<type>::clear() {
	linkedlistnode<type>	*current=first;
	linkedlistnode<type>	*next;
	for (unsigned long i=0; i<length; i++) {
		next=current->getNext();
		delete current;
		current=next;
	}
	length=0;
}
