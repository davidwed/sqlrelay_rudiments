// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>

template <class datatype, class keytype>
inline listnode<datatype,keytype>::listnode() {
	data=0;
	previous=NULL;
	next=NULL;
}

template <class datatype, class keytype>
inline listnode<datatype,keytype>::listnode(datatype data) {
	this->data=data;
	previous=NULL;
	next=NULL;
}

template <class datatype, class keytype>
inline listnode<datatype,keytype>::listnode(datatype data,
					listnode<datatype,keytype> *previous,
					listnode<datatype,keytype> *next) {
	this->data=data;
	this->previous=previous;
	this->next=next;
}

template <class datatype, class keytype>
inline listnode<datatype,keytype>::~listnode() {
}

template <class datatype, class keytype>
inline void listnode<datatype,keytype>::setData(datatype data) {
	this->data=data;
}

template <class datatype, class keytype>
inline datatype listnode<datatype,keytype>::getData() const {
	return data;
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
