// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>

#define LINKEDLISTNODE_TEMPLATE template <class datatype>

#define LINKEDLISTNODE_CLASS linkedlistnode<datatype>

LINKEDLISTNODE_TEMPLATE
inline LINKEDLISTNODE_CLASS::linkedlistnode() {
	data=0;
	previous=NULL;
	next=NULL;
}

LINKEDLISTNODE_TEMPLATE
inline LINKEDLISTNODE_CLASS::~linkedlistnode() {
}

LINKEDLISTNODE_TEMPLATE
inline void LINKEDLISTNODE_CLASS::setData(datatype data) {
	this->data=data;
}

LINKEDLISTNODE_TEMPLATE
inline datatype LINKEDLISTNODE_CLASS::getData() const {
	return data;
}

LINKEDLISTNODE_TEMPLATE
inline void LINKEDLISTNODE_CLASS::setPrevious(LINKEDLISTNODE_CLASS *previous) {
	this->previous=previous;
}

LINKEDLISTNODE_TEMPLATE
inline void LINKEDLISTNODE_CLASS::setNext(LINKEDLISTNODE_CLASS *next) {
	this->next=next;
}

LINKEDLISTNODE_TEMPLATE
inline LINKEDLISTNODE_CLASS *LINKEDLISTNODE_CLASS::getPrevious() const {
	return previous;
}

LINKEDLISTNODE_TEMPLATE
inline LINKEDLISTNODE_CLASS *LINKEDLISTNODE_CLASS::getNext() const {
	return next;
}

LINKEDLISTNODE_TEMPLATE
inline int LINKEDLISTNODE_CLASS::compare(datatype data) const {
	return datautil.compare(this->data,data);
}

LINKEDLISTNODE_TEMPLATE
inline void LINKEDLISTNODE_CLASS::print() const {
	return datautil.print(data);
}
