// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>

template <class datatype, class keytype>
inline listnode<datatype,keytype>::listnode() {
	value=0;
	previous=NULL;
	next=NULL;
}

template <class datatype, class keytype>
inline listnode<datatype,keytype>::listnode(datatype value) {
	this->value=value;
	previous=NULL;
	next=NULL;
}

template <class datatype, class keytype>
inline listnode<datatype,keytype>::listnode(datatype value,
					listnode<datatype,keytype> *previous,
					listnode<datatype,keytype> *next) {
	this->value=value;
	this->previous=previous;
	this->next=next;
}

template <class datatype, class keytype>
inline listnode<datatype,keytype>::~listnode() {
}

template <class datatype, class keytype>
inline void listnode<datatype,keytype>::setValue(datatype value) {
	this->value=value;
}

template <class datatype, class keytype>
inline datatype listnode<datatype,keytype>::getValue() const {
	return value;
}

template <class datatype, class keytype>
inline void listnode<datatype,keytype>::setPrevious(
					listnode<datatype,keytype> *previous) {
	this->previous=previous;
}

template <class datatype, class keytype>
inline void listnode<datatype,keytype>::setNext(
					listnode<datatype,keytype> *next) {
	this->next=next;
}

template <class datatype, class keytype>
inline listnode<datatype,keytype> *listnode<datatype,keytype>::getPrevious()
									const {
	return previous;
}

template <class datatype, class keytype>
inline listnode<datatype,keytype> *listnode<datatype,keytype>::getNext() const {
	return next;
}
