// Copyright (c) 2014 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/rudimentsinlines.h>
#include <rudiments/private/linkedlistutilinlines.h>

#define SINGLYLINKEDLIST_TEMPLATE template <class valuetype>

#define SINGLYLINKEDLIST_CLASS singlylinkedlist<valuetype>

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
SINGLYLINKEDLIST_CLASS::singlylinkedlist() {
	first=NULL;
	last=NULL;
	length=0;
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
SINGLYLINKEDLIST_CLASS::~singlylinkedlist() {
	clear();
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void SINGLYLINKEDLIST_CLASS::prepend(valuetype value) {
	prepend(new singlylinkedlistnode<valuetype>(value));
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void SINGLYLINKEDLIST_CLASS::prepend(singlylinkedlistnode<valuetype> *node) {
	if (!node) {
		return;
	} else if (first) {
		node->setNext(first);
		first=node;
	} else {
		first=node;
		last=first;
	}
	length++;
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void SINGLYLINKEDLIST_CLASS::append(valuetype value) {
	append(new singlylinkedlistnode<valuetype>(value));
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void SINGLYLINKEDLIST_CLASS::append(singlylinkedlistnode<valuetype> *node) {
	if (!node) {
		return;
	} else if (last) {
		last->setNext(node);
		last=node;
	} else {
		first=node;
		last=first;
	}
	length++;
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void SINGLYLINKEDLIST_CLASS::insertAfter(
				singlylinkedlistnode<valuetype> *node,
				valuetype value) {
	insertAfter(node,new singlylinkedlistnode<valuetype>(value));
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void SINGLYLINKEDLIST_CLASS::insertAfter(
				singlylinkedlistnode<valuetype> *node,
				singlylinkedlistnode<valuetype> *newnode) {
	if (!node) {
		return;
	} else if (node==last) {
		append(newnode);
	} else {
		newnode->setNext(node->getNext());
		node->setNext(newnode);
		length++;
	}
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool SINGLYLINKEDLIST_CLASS::remove(valuetype value) {
	singlylinkedlistnode<valuetype> *current=first;
	if (!current->compare(value)) {
		if (first==last) {
			first=NULL;
			last=NULL;
		} else {
			first=first->getNext();
		}
	} else {
		singlylinkedlistnode<valuetype> *prev=first;
		current=current->getNext();
		while (current) {
			if (!current->compare(value)) {
				prev->setNext(current->getNext());
				break;
			}
			prev=current;
			current=current->getNext();
		}
		if (last==current) {
			last=prev;
		}
	}
	if (current) {
		delete current;
		length--;
		return true;
	}
	return false;
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool SINGLYLINKEDLIST_CLASS::removeAll(valuetype value) {
	bool	retval=false;
	singlylinkedlistnode<valuetype> *current=first;
	if (!current->compare(value)) {
		retval=true;
		if (first==last) {
			first=NULL;
			last=NULL;
			delete current;
			length--;
			return true;
		} else {
			first=first->getNext();
			delete current;
			length--;
			current=first;
		}
	}
	singlylinkedlistnode<valuetype> *prev=first;
	current=current->getNext();
	while (current) {
		if (!current->compare(value)) {
			retval=true;
			singlylinkedlistnode<valuetype> *temp=
							current->getNext();
			prev->setNext(temp);
			if (last==current) {
				last=prev;
			}
			delete current;
			length--;
			current=temp;
		} else {
			prev=current;
			current=current->getNext();
		}
	}
	return retval;
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool SINGLYLINKEDLIST_CLASS::remove(singlylinkedlistnode<valuetype> *node) {
	if (!node) {
		return false;
	}
	singlylinkedlistnode<valuetype> *current=first;
	if (current==node) {
		if (first==last) {
			first=NULL;
			last=NULL;
		} else {
			first=first->getNext();
		}
	} else {
		singlylinkedlistnode<valuetype> *prev=first;
		current=current->getNext();
		while (current) {
			if (current==node) {
				prev->setNext(current->getNext());
				break;
			}
			prev=current;
			current=current->getNext();
		}
		if (last==current) {
			last=prev;
		}
	}
	if (current) {
		delete current;
		length--;
		return true;
	}
	return false;
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
uint64_t SINGLYLINKEDLIST_CLASS::getLength() const {
	return length;
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
singlylinkedlistnode<valuetype> *SINGLYLINKEDLIST_CLASS::getFirst() {
	return first;
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
singlylinkedlistnode<valuetype> *SINGLYLINKEDLIST_CLASS::getLast() {
	return last;
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
singlylinkedlistnode<valuetype> *SINGLYLINKEDLIST_CLASS::getNext(
					singlylinkedlistnode<valuetype> *node) {
	return (node)?node->getNext():NULL;
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
singlylinkedlistnode<valuetype> *SINGLYLINKEDLIST_CLASS::find(valuetype value) {
	return find((singlylinkedlistnode<valuetype> *)first,value);
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
singlylinkedlistnode<valuetype> *SINGLYLINKEDLIST_CLASS::find(
				singlylinkedlistnode<valuetype> *startnode,
				valuetype value) {
	for (singlylinkedlistnode<valuetype> *current=startnode;
			current; current=current->getNext()) {
		if (!current->compare(value)) {
			return current;
		}
	}
	return NULL;
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void SINGLYLINKEDLIST_CLASS::clear() {
	singlylinkedlistnode<valuetype>	*next;
	singlylinkedlistnode<valuetype>	*current=first;
	while (current) {
		next=current->getNext();
		delete current;
		current=next;
	}
	first=NULL;
	last=NULL;
	length=0;
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void SINGLYLINKEDLIST_CLASS::print() const {
	uint64_t	i=0;
	for (singlylinkedlistnode<valuetype> *current=first;
				current; current=current->getNext()) {
		stdoutput.printf("index %lld: ",(long long)i);
		current->print();
		stdoutput.printf("\n");
		i++;
	}
}

#define SINGLYLINKEDLISTNODE_TEMPLATE template <class valuetype>

#define SINGLYLINKEDLISTNODE_CLASS singlylinkedlistnode<valuetype>

SINGLYLINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
SINGLYLINKEDLISTNODE_CLASS::singlylinkedlistnode(valuetype value) {
	this->value=value;
	next=NULL;
}

SINGLYLINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
SINGLYLINKEDLISTNODE_CLASS::~singlylinkedlistnode() {
}

SINGLYLINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void SINGLYLINKEDLISTNODE_CLASS::setValue(valuetype value) {
	this->value=value;
}

SINGLYLINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
valuetype SINGLYLINKEDLISTNODE_CLASS::getValue() const {
	return value;
}

SINGLYLINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
SINGLYLINKEDLISTNODE_CLASS *SINGLYLINKEDLISTNODE_CLASS::getNext() {
	return next;
}

SINGLYLINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
int32_t SINGLYLINKEDLISTNODE_CLASS::compare(valuetype value) const {
	return _linkedlistutil_compare(this->value,value);
}

SINGLYLINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void SINGLYLINKEDLISTNODE_CLASS::print() const {
	_linkedlistutil_print(value);
}

SINGLYLINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void SINGLYLINKEDLISTNODE_CLASS::setNext(
				SINGLYLINKEDLISTNODE_CLASS *next) {
	this->next=next;
}
