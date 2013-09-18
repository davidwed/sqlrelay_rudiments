// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/rudimentsinlines.h>

#define LINKEDLIST_TEMPLATE template <class datatype>

#define LINKEDLIST_CLASS linkedlist<datatype>

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLIST_CLASS::linkedlist() {
	first=NULL;
	last=NULL;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLIST_CLASS::~linkedlist() {
	clear();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::append(datatype data) {
	linkedlistnode<datatype>	*node=new linkedlistnode<datatype>();
	node->setData(data);
	append(node);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::append(linkedlistnode<datatype> *node) {
	if (last) {
		last->setNext(node);
		node->setPrevious(last);
		last=node;
	} else {
		first=node;
		last=first;
	}
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::insert(uint64_t index, datatype data) {
	linkedlistnode<datatype>	*node=new linkedlistnode<datatype>();
	node->setData(data);
	return insert(index,node);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::insert(uint64_t index, linkedlistnode<datatype> *node) {

	// handle insert into index 0
	if (!index) {
		node->setNext(first);
		first->setPrevious(node);
		first=(linkedlistnode<datatype> *)node;
		return true;
	}

	// handle general insert
	linkedlistnode<datatype>	*current=getNodeByIndex(index-1);
	if (!current) {
		return false;
	}
	node->setPrevious(current);
	node->setNext(current->getNext());
	current->getNext()->setPrevious(node);
	current->setNext(node);
	return true;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::setDataByIndex(uint64_t index, datatype data) {
	linkedlistnode<datatype>	*current=getNodeByIndex(index);
	if (current) {
		current->setData(data);
		return true;
	}
	return false;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeByIndex(uint64_t index) {
	return removeNode(getNodeByIndex(index));
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeByData(datatype data) {
	for (linkedlistnode<datatype> *current=first; current;
		current=(linkedlistnode<datatype> *)current->getNext()) {
		if (!current->compare(data)) {
			return removeNode(current);
		}
	}
	return false;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeAllByData(datatype data) {

	linkedlistnode<datatype>	*current=first;
	linkedlistnode<datatype>	*next;
	while (current) {
		if (!current->compare(data)) {
			next=(linkedlistnode<datatype> *)current->getNext();
			if (!removeNode(current)) {
				return false;
			}
			current=next;
		} else {
			current=(linkedlistnode<datatype> *)current->getNext();
		}
	}
	return true;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeNode(linkedlistnode<datatype> *node) {
	if (!node) {
		return false;
	}
	if (node->getNext()) {
		node->getNext()->setPrevious(node->getPrevious());
	}
	if (node->getPrevious()) {
		node->getPrevious()->setNext(node->getNext());
	}
	if (node==first) {
		first=(linkedlistnode<datatype> *)node->getNext();
	}
	if (node==last) {
		last=(linkedlistnode<datatype> *)node->getPrevious();
	}
	delete node;
	return true;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::getDataByIndex(uint64_t index, datatype *data) {
	linkedlistnode<datatype>	*current=getNodeByIndex(index);
	if (current) {
		*data=current->getData();
		return true;
	}
	return false;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
uint64_t LINKEDLIST_CLASS::getLength() const {
	uint64_t	length=0;
	for (linkedlistnode<datatype> *current=first; current;
		current=(linkedlistnode<datatype> *)current->getNext()) {
		length++;
	}
	return length;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<datatype> *LINKEDLIST_CLASS::getFirstNode() {
	return (linkedlistnode<datatype> *)first;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<datatype> *LINKEDLIST_CLASS::getLastNode() {
	return (linkedlistnode<datatype> *)last;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<datatype> *LINKEDLIST_CLASS::getNodeByIndex(uint64_t index) {
	linkedlistnode<datatype>	*current=
					(linkedlistnode<datatype> *)first;
	for (uint64_t i=0; current && i<index; i++) {
		current=(linkedlistnode<datatype> *)current->getNext();
	}
	return current;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<datatype> *LINKEDLIST_CLASS::getNodeByData(datatype data) {
	return getNodeByData((linkedlistnode<datatype> *)first,data);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<datatype> *LINKEDLIST_CLASS::getNodeByData(
					linkedlistnode<datatype> *startnode,
					datatype data) {
	for (linkedlistnode<datatype> *current=startnode; current;
		current=(linkedlistnode<datatype> *)current->getNext()) {
		if (!current->compare(data)) {
			return current;
		}
	}
	return NULL;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::clear() {
	linkedlistnode<datatype>	*next;
	linkedlistnode<datatype>	*current=first;
	while (current) {
		next=(linkedlistnode<datatype> *)current->getNext();
		delete current;
		current=next;
	}
	first=NULL;
	last=NULL;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::print() const {
	uint64_t	i=0;
	for (linkedlistnode<datatype> *current=first; current;
		current=(linkedlistnode<datatype> *)current->getNext()) {
		stdoutput.printf("index %lld: ",(long long)i);
		current->print();
		stdoutput.printf("\n");
		i++;
	}
}
