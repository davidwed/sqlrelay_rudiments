// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>

#define LISTNODE_TEMPLATE \
	template <class datatype, class keytype>

#define LISTNODE_CLASS \
	listnode<datatype,keytype>

LISTNODE_TEMPLATE
inline LISTNODE_CLASS::listnode() {
	data=0;
	previous=NULL;
	next=NULL;
}

LISTNODE_TEMPLATE
inline LISTNODE_CLASS::listnode(datatype data) {
	this->data=data;
	previous=NULL;
	next=NULL;
}

LISTNODE_TEMPLATE
inline LISTNODE_CLASS::listnode(datatype data,
					listnode<datatype,keytype> *previous,
					listnode<datatype,keytype> *next) {
	this->data=data;
	this->previous=previous;
	this->next=next;
}

LISTNODE_TEMPLATE
inline LISTNODE_CLASS::~listnode() {
}

LISTNODE_TEMPLATE
inline void LISTNODE_CLASS::setData(datatype data) {
	this->data=data;
}

LISTNODE_TEMPLATE
inline datatype LISTNODE_CLASS::getData() const {
	return data;
}

LISTNODE_TEMPLATE
inline void LISTNODE_CLASS::setPrevious(listnode<datatype,keytype> *previous) {
	this->previous=previous;
}

LISTNODE_TEMPLATE
inline void LISTNODE_CLASS::setNext(listnode<datatype,keytype> *next) {
	this->next=next;
}

LISTNODE_TEMPLATE
inline listnode<datatype,keytype> *LISTNODE_CLASS::getPrevious() const {
	return previous;
}

LISTNODE_TEMPLATE
inline listnode<datatype,keytype> *LISTNODE_CLASS::getNext() const {
	return next;
}
