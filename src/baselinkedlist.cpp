// Copyright (c) 2013 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/private/baselinkedlist.h>

class baselinkedlistprivate {
	friend class baselinkedlist;
	private:
		baselinkedlistnode	*_first;
		baselinkedlistnode	*_last;
};

baselinkedlist::baselinkedlist() {
	pvt=new baselinkedlistprivate;
	pvt->_first=NULL;
	pvt->_last=NULL;
}

baselinkedlist::~baselinkedlist() {
	clear();
	delete pvt;
}

baselinkedlistnode *baselinkedlist::newNode() {
	return new baselinkedlistnode();
}

void baselinkedlist::append(uint64_t value, uint64_t size) {
	baselinkedlistnode	*node=newNode();
	node->setValue(value,size);
	append(node);
}

void baselinkedlist::append(baselinkedlistnode *node) {
	if (pvt->_last) {
		pvt->_last->setNext(node);
		node->setPrevious(pvt->_last);
		pvt->_last=node;
	} else {
		pvt->_first=node;
		pvt->_last=pvt->_first;
	}
}

bool baselinkedlist::insert(uint64_t index, uint64_t value, uint64_t size) {
	baselinkedlistnode *node=newNode();
	node->setValue(value,size);
	return insert(index,node);
}

bool baselinkedlist::insert(uint64_t index, baselinkedlistnode *node) {

	// handle insert into index 0
	if (!index) {
		node->setNext(pvt->_first);
		pvt->_first->setPrevious(node);
		pvt->_first=node;
		return true;
	}

	// handle general insert
	baselinkedlistnode *current=getNodeByIndex(index-1);
	if (!current) {
		return false;
	}
	node->setPrevious(current);
	node->setNext(current->getNext());
	current->getNext()->setPrevious(node);
	current->setNext(node);
	return true;
}

bool baselinkedlist::setValueByIndex(uint64_t index,
					uint64_t value, uint64_t size) {
	baselinkedlistnode *current=getNodeByIndex(index);
	if (current) {
		current->setValue(value,size);
		return true;
	}
	return false;
}

bool baselinkedlist::removeByIndex(uint64_t index) {
	return removeNode(getNodeByIndex(index));
}

bool baselinkedlist::removeByValue(uint64_t value) {
	for (baselinkedlistnode *current=pvt->_first;
			current; current=current->getNext()) {
		if (!current->compare(value)) {
			return removeNode(current);
		}
	}
	return false;
}

bool baselinkedlist::removeAllByValue(uint64_t value) {

	baselinkedlistnode	*current=pvt->_first;
	baselinkedlistnode	*next;
	while (current) {
		if (!current->compare(value)) {
			next=current->getNext();
			if (!removeNode(current)) {
				return false;
			}
			current=next;
		} else {
			current=current->getNext();
		}
	}
	return true;
}

bool baselinkedlist::removeNode(baselinkedlistnode *node) {
	if (!node) {
		return false;
	}
	if (node->getNext()) {
		node->getNext()->setPrevious(node->getPrevious());
	}
	if (node->getPrevious()) {
		node->getPrevious()->setNext(node->getNext());
	}
	if (node==pvt->_first) {
		pvt->_first=node->getNext();
	}
	if (node==pvt->_last) {
		pvt->_last=node->getPrevious();
	}
	delete node;
	return true;
}

bool baselinkedlist::getValueByIndex(uint64_t index, uint64_t *value) {
	baselinkedlistnode	*current=getNodeByIndex(index);
	if (current) {
		*value=current->getValue();
		return true;
	}
	return false;
}

uint64_t baselinkedlist::getLength() const {
	uint64_t	length=0;
	for (baselinkedlistnode *current=pvt->_first;
			current; current=current->getNext()) {
		length++;
	}
	return length;
}

baselinkedlistnode *baselinkedlist::getFirstNode() {
	return pvt->_first;
}

baselinkedlistnode *baselinkedlist::getLastNode() {
	return pvt->_last;
}

baselinkedlistnode *baselinkedlist::getNodeByIndex(uint64_t index) {
	baselinkedlistnode	*current=pvt->_first;
	for (uint64_t i=0; current && i<index; i++) {
		current=current->getNext();
	}
	return current;
}

baselinkedlistnode *baselinkedlist::getNodeByValue(uint64_t value) {
	return getNodeByValue(pvt->_first,value);
}

baselinkedlistnode *baselinkedlist::getNodeByValue(
					baselinkedlistnode *startnode,
					uint64_t value) {
	for (baselinkedlistnode *current=startnode;
			current; current=current->getNext()) {
		if (!current->compare(value)) {
			return current;
		}
	}
	return NULL;
}

void baselinkedlist::clear() {
	baselinkedlistnode	*next;
	baselinkedlistnode	*current=pvt->_first;
	while (current) {
		next=current->getNext();
		delete current;
		current=next;
	}
	pvt->_first=NULL;
	pvt->_last=NULL;
}

void baselinkedlist::print() const {
	uint64_t	i=0;
	for (baselinkedlistnode *current=pvt->_first;
			current; current=current->getNext()) {
		stdoutput.printf("index %lld: ",(long long)i);
		current->print();
		stdoutput.printf("\n");
		i++;
	}
}

class baselinkedlistnodeprivate {
	friend class baselinkedlistnode;
	private:
		uint64_t		_value;
		uint64_t		_size;
		baselinkedlistnode	*_previous;
		baselinkedlistnode	*_next;
};

baselinkedlistnode::baselinkedlistnode() {
	pvt=new baselinkedlistnodeprivate;
	pvt->_value=0;
	pvt->_size=0;
	pvt->_previous=NULL;
	pvt->_next=NULL;
}

baselinkedlistnode::~baselinkedlistnode() {
	delete pvt;
}

void baselinkedlistnode::setValue(uint64_t value, uint64_t size) {
	pvt->_value=value;
	pvt->_size=size;
}

uint64_t baselinkedlistnode::getValue() const {
	return pvt->_value;
}

void baselinkedlistnode::setPrevious(baselinkedlistnode *previous) {
	pvt->_previous=previous;
}

void baselinkedlistnode::setNext(baselinkedlistnode *next) {
	pvt->_next=next;
}

baselinkedlistnode *baselinkedlistnode::getPrevious() {
	return pvt->_previous;
}

baselinkedlistnode *baselinkedlistnode::getNext() {
	return pvt->_next;
}

int32_t baselinkedlistnode::compare(uint64_t value) const {
	return rawbuffer::compare(&pvt->_value,&value,pvt->_size);
}

void baselinkedlistnode::print() const {
	stdoutput.printf("%08x",pvt->_value);
}
