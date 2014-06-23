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
void SINGLYLINKEDLIST_CLASS::moveAfter(
				singlylinkedlistnode<valuetype> *node,
				singlylinkedlistnode<valuetype> *nodetomove) {

	if (!node || !nodetomove || node==nodetomove) {
		return;
	}

	if (nodetomove==first) {
		first=nodetomove->getNext();
	} else if (nodetomove==last) {
		singlylinkedlistnode<valuetype> *secondtolast=first;
		while (secondtolast->getNext()!=last) {
			secondtolast=secondtolast->getNext();
		}
		last=secondtolast;
		secondtolast->setNext(NULL);
	} else {
		singlylinkedlistnode<valuetype> *previous=first;
		while (previous->getNext()!=nodetomove) {
			previous=previous->getNext();
		}
		previous->setNext(nodetomove->getNext());
	}

	nodetomove->setNext(node->getNext());
	node->setNext(nodetomove);
	if (node==last) {
		last=nodetomove;
	}
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void SINGLYLINKEDLIST_CLASS::detach(singlylinkedlistnode<valuetype> *node) {

	if (node==first && node==last) {
		first=NULL;
		last=NULL;
	} else if (node==first) {
		first=node->getNext();
	} else if (node==last) {
		singlylinkedlistnode<valuetype> *secondtolast=first;
		while (secondtolast->getNext()!=last) {
			secondtolast=secondtolast->getNext();
		}
		last=secondtolast;
		secondtolast->setNext(NULL);
	} else {
		singlylinkedlistnode<valuetype> *previous=first;
		while (previous->getNext()!=node) {
			previous=previous->getNext();
		}
		previous->setNext(node->getNext());
	}
	node->setNext(NULL);
	length--;
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
void SINGLYLINKEDLIST_CLASS::sort() {

	// insertion sort with a few optimization...

	// if there are 0 or 1 items in the list then it's already sorted
	if (length<2) {
		return;
	}

	// first and last pointers for the new list
	singlylinkedlistnode<valuetype>	*newfirst=NULL;
	singlylinkedlistnode<valuetype>	*newlast=NULL;

	// pointers for iterating through the new list
	singlylinkedlistnode<valuetype> *current=NULL;
	singlylinkedlistnode<valuetype> *previous=NULL;

	// iterate through the current list, building a new one as we go
	singlylinkedlistnode<valuetype>	*node=first;
	singlylinkedlistnode<valuetype>	*next=NULL;
	while (node) {

		// get the next node so we can move on later
		next=node->getNext();

		// if the new list is empty
		if (!newfirst) {
			node->setNext(NULL);
			newfirst=node;
			newlast=node;
		} else

		// if the node belongs at the beginning of the new list
		// (optimization for lists that are already largely forwards)
		if (newfirst->compare(node->getValue())>0) {
			node->setNext(newfirst);
			newfirst=node;
		} else

		// if the node belongs at the end of the new list
		// (optimization for lists that are already largely backwards)
		if (newlast->compare(node->getValue())<=0) {
			node->setNext(NULL);
			newlast->setNext(node);
			newlast=node;
		} else

		// if the node belongs somewhere in the middle of the new list
		{
			// search from the left...
			current=newfirst->getNext();
			previous=newfirst;
			while (current) {

				// if the current node is greater than...
				if (current->compare(node->getValue())>0) {

					// insert before
					node->setNext(current);
					previous->setNext(node);
					break;
				}

				// move on
				previous=current;
				current=current->getNext();
			}
		}

		// move on
		node=next;
	}

	// make the new list the current list
	first=newfirst;
	last=newlast;
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
	print(length);
}

SINGLYLINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void SINGLYLINKEDLIST_CLASS::print(uint64_t count) const {
	uint64_t	i=0;
	for (singlylinkedlistnode<valuetype> *current=first;
			current && i<count; current=current->getNext()) {
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
