// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>

#define LISTNODE_TEMPLATE template <class datatype>

#define LISTNODE_CLASS listnode<datatype>

LISTNODE_TEMPLATE
inline LISTNODE_CLASS::listnode() {
	data=0;
	previous=NULL;
	next=NULL;
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
inline void LISTNODE_CLASS::setPrevious(listnode<datatype> *previous) {
	this->previous=previous;
}

LISTNODE_TEMPLATE
inline void LISTNODE_CLASS::setNext(listnode<datatype> *next) {
	this->next=next;
}

LISTNODE_TEMPLATE
inline listnode<datatype> *LISTNODE_CLASS::getPrevious() const {
	return previous;
}

LISTNODE_TEMPLATE
inline listnode<datatype> *LISTNODE_CLASS::getNext() const {
	return next;
}

LISTNODE_TEMPLATE
inline int LISTNODE_CLASS::compare(datatype data) const {
	return datautil.compare(this->data,data);
}

LISTNODE_TEMPLATE
inline void LISTNODE_CLASS::print() const {
	return datautil.print(data);
}
