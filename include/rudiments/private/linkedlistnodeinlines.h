// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>

template <class type>
inline linkedlistnode<type>::linkedlistnode() {
	value=0;
	previous=NULL;
	next=NULL;
}

template <class type>
inline linkedlistnode<type>::linkedlistnode(type value) {
	this->value=value;
	previous=NULL;
	next=NULL;
}

template <class type>
inline linkedlistnode<type>::linkedlistnode(type value,
						linkedlistnode *previous,
						linkedlistnode *next) {
	this->value=value;
	this->previous=previous;
	this->next=next;
}

template <class type>
inline void linkedlistnode<type>::setValue(type value) {
	this->value=value;
}

template <class type>
inline type linkedlistnode<type>::getValue() const {
	return value;
}

template <class type>
inline void linkedlistnode<type>::setPrevious(linkedlistnode<type> *previous) {
	this->previous=previous;
}

template <class type>
inline void linkedlistnode<type>::setNext(linkedlistnode<type> *next) {
	this->next=next;
}

template <class type>
inline linkedlistnode<type> *linkedlistnode<type>::getPrevious() const {
	return previous;
}

template <class type>
inline linkedlistnode<type> *linkedlistnode<type>::getNext() const {
	return next;
}
