// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>

template <class type>
inline listnode<type>::listnode() {
	value=0;
	previous=NULL;
	next=NULL;
}

template <class type>
inline listnode<type>::listnode(type value) {
	this->value=value;
	previous=NULL;
	next=NULL;
}

template <class type>
inline listnode<type>::listnode(type value,
					listnode *previous, listnode *next) {
	this->value=value;
	this->previous=previous;
	this->next=next;
}

template <class type>
inline void listnode<type>::setValue(type value) {
	this->value=value;
}

template <class type>
inline type listnode<type>::getValue() const {
	return value;
}

template <class type>
inline void listnode<type>::setPrevious(listnode<type> *previous) {
	this->previous=previous;
}

template <class type>
inline void listnode<type>::setNext(listnode<type> *next) {
	this->next=next;
}

template <class type>
inline listnode<type> *listnode<type>::getPrevious() const {
	return previous;
}

template <class type>
inline listnode<type> *listnode<type>::getNext() const {
	return next;
}
