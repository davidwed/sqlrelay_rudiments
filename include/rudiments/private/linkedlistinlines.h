// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/rudimentsinlines.h>
#include <rudiments/private/linkedlistutilinlines.h>

#define LINKEDLIST_TEMPLATE template <class valuetype>

#define LINKEDLIST_CLASS linkedlist<valuetype>

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLIST_CLASS::linkedlist() {
	first=NULL;
	last=NULL;
	length=0;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLIST_CLASS::~linkedlist() {
	clear();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::prepend(valuetype value) {
	prepend(new linkedlistnode<valuetype>(value));
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::prepend(linkedlistnode<valuetype> *node) {
	if (!node) {
		return;
	} else if (first) {
		first->setPrevious(node);
		node->setNext(first);
		first=node;
	} else {
		first=node;
		last=first;
	}
	length++;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::append(valuetype value) {
	append(new linkedlistnode<valuetype>(value));
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::append(linkedlistnode<valuetype> *node) {
	if (!node) {
		return;
	} else if (last) {
		last->setNext(node);
		node->setPrevious(last);
		last=node;
	} else {
		first=node;
		last=first;
	}
	length++;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::insertBefore(linkedlistnode<valuetype> *node,
							valuetype value) {
	insertBefore(node,new linkedlistnode<valuetype>(value));
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::insertBefore(linkedlistnode<valuetype> *node,
					linkedlistnode<valuetype> *newnode) {
	if (!node) {
		return;
	} else if (node==first) {
		prepend(newnode);
	} else {
		newnode->setNext(node);
		newnode->setPrevious(node->getPrevious());
		node->getPrevious()->setNext(newnode);
		node->setPrevious(newnode);
		length++;
	}
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::insertAfter(linkedlistnode<valuetype> *node,
							valuetype value) {
	insertAfter(node,new linkedlistnode<valuetype>(value));
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::insertAfter(linkedlistnode<valuetype> *node,
					linkedlistnode<valuetype> *newnode) {
	if (!node) {
		return;
	} else if (node==last) {
		append(newnode);
	} else {
		newnode->setNext(node->getNext());
		newnode->setPrevious(node);
		node->getNext()->setPrevious(newnode);
		node->setNext(newnode);
		length++;
	}
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::moveBefore(linkedlistnode<valuetype> *node,
					linkedlistnode<valuetype> *nodetomove) {
	return move(node,nodetomove,true);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::moveAfter(linkedlistnode<valuetype> *node,
					linkedlistnode<valuetype> *nodetomove) {
	return move(node,nodetomove,false);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::move(linkedlistnode<valuetype> *node,
					linkedlistnode<valuetype> *nodetomove,
					bool before) {

	if (!node || !nodetomove || node==nodetomove) {
		return;
	}

	if (nodetomove==first) {
		first=nodetomove->getNext();
	} else if (nodetomove==last) {
		last=nodetomove->getPrevious();
	}
	if (nodetomove->getPrevious()) {
		nodetomove->getPrevious()->setNext(nodetomove->getNext());
	}
	if (nodetomove->getNext()) {
		nodetomove->getNext()->setPrevious(nodetomove->getPrevious());
	}

	if (before) {
		nodetomove->setNext(node);
		nodetomove->setPrevious(node->getPrevious());
		node->setPrevious(nodetomove);
		if (node==first) {
			first=nodetomove;
		}
	} else {
		nodetomove->setNext(node->getNext());
		nodetomove->setPrevious(node);
		node->setNext(nodetomove);
		if (node==last) {
			last=nodetomove;
		}
	}
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::detach(linkedlistnode<valuetype> *node) {

	if (node==first) {
		first=node->getNext();
	}
	if (node==last) {
		last=node->getPrevious();
	}
	if (node->getPrevious()) {
		node->getPrevious()->setNext(node->getNext());
	}
	if (node->getNext()) {
		node->getNext()->setPrevious(node->getPrevious());
	}
	node->setNext(NULL);
	node->setPrevious(NULL);
	length--;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::remove(valuetype value) {
	linkedlistnode<valuetype>	*current=find(value);
	return (current)?remove(current):false;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeAll(valuetype value) {

	linkedlistnode<valuetype>	*current=first;
	linkedlistnode<valuetype>	*next;
	while (current) {
		next=current->getNext();
		if (!current->compare(value) && !remove(current)) {
			return false;
		}
		current=next;
	}
	return true;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::remove(linkedlistnode<valuetype> *node) {
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
		first=node->getNext();
	}
	if (node==last) {
		last=node->getPrevious();
	}
	delete node;
	length--;
	return true;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
uint64_t LINKEDLIST_CLASS::getLength() const {
	return length;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getFirst() {
	return first;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getLast() {
	return last;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getPrevious(
					linkedlistnode<valuetype> *node) {
	return (node)?node->getPrevious():NULL;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getNext(
					linkedlistnode<valuetype> *node) {
	return (node)?node->getNext():NULL;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::find(valuetype value) {
	return find((linkedlistnode<valuetype> *)first,value);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::find(
					linkedlistnode<valuetype> *startnode,
					valuetype value) {
	for (linkedlistnode<valuetype> *current=startnode;
			current; current=current->getNext()) {
		if (!current->compare(value)) {
			return current;
		}
	}
	return NULL;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::sort() {

	// insertion sort with a few optimizations...

	// if there are 0 or 1 items in the list then it's already sorted
	if (length<2) {
		return;
	}

	// first and last pointers for the new list
	linkedlistnode<valuetype>	*newfirst=NULL;
	linkedlistnode<valuetype>	*newlast=NULL;

	// pointer for iterating through the new list
	linkedlistnode<valuetype>	*currentfromfirst=NULL;
	linkedlistnode<valuetype>	*currentfromlast=NULL;

	// iterate through the current list, building a new one as we go
	linkedlistnode<valuetype>	*node=first;
	linkedlistnode<valuetype>	*next=NULL;
	while (node) {

		// get the next node so we can move on later
		next=node->getNext();

		// if the new list is empty...
		if (!newfirst) {
			node->setPrevious(NULL);
			node->setNext(NULL);
			newfirst=node;
			newlast=node;
		} else

		// if the node belongs at the beginning of the new list
		// (optimization for lists that are already largely forwards)
		if (newfirst->compare(node->getValue())>0) {
			node->setNext(newfirst);
			node->setPrevious(NULL);
			newfirst->setPrevious(node);
			newfirst=node;
		} else

		// if the node belongs at the end of the new list
		// (optimization for lists that are already largely backwards)
		if (newlast->compare(node->getValue())<=0) {
			node->setPrevious(newlast);
			node->setNext(NULL);
			newlast->setNext(node);
			newlast=node;
		} else

		// if the node belongs somewhere in the middle of the new list
		{

			// search from both ends toward the middle...
			// (optimization for data that is more random)
			currentfromfirst=newfirst->getNext();
			currentfromlast=newlast->getPrevious();
			while (currentfromfirst) {

				// if the current node (from the left)
				// is greater than...
				if (currentfromfirst->
						compare(node->getValue())>0) {

					// insert before
					node->setNext(currentfromfirst);
					node->setPrevious(currentfromfirst->
								getPrevious());
					currentfromfirst->
						getPrevious()->setNext(node);
					currentfromfirst->
						setPrevious(node);
					break;

				} else

				// if the current node (from the right)
				// is less than or equal to...
				if (currentfromlast->
						compare(node->getValue())<=0) {

					// insert after
					node->setPrevious(currentfromlast);
					node->setNext(currentfromlast->
								getNext());
					currentfromlast->
						getNext()->setPrevious(node);
					currentfromlast->
						setNext(node);
					break;
				}

				// move on
				currentfromfirst=currentfromfirst->getNext();
				currentfromlast=currentfromlast->getPrevious();
			}
		}

		// move on
		node=next;
	}

	// make the new list the current list
	first=newfirst;
	last=newlast;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::clear() {
	linkedlistnode<valuetype>	*next;
	linkedlistnode<valuetype>	*current=first;
	while (current) {
		next=current->getNext();
		delete current;
		current=next;
	}
	first=NULL;
	last=NULL;
	length=0;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::print() const {
	print(length);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::print(uint64_t count) const {
	uint64_t	i=0;
	for (linkedlistnode<valuetype> *current=first;
			current && i<count; current=current->getNext()) {
		stdoutput.printf("index %lld: ",(long long)i);
		current->print();
		stdoutput.printf("\n");
		i++;
	}
}

#define LINKEDLISTNODE_TEMPLATE template <class valuetype>

#define LINKEDLISTNODE_CLASS linkedlistnode<valuetype>

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLISTNODE_CLASS::linkedlistnode(valuetype value) {
	this->value=value;
	previous=NULL;
	next=NULL;
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLISTNODE_CLASS::~linkedlistnode() {
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLISTNODE_CLASS::setValue(valuetype value) {
	this->value=value;
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
valuetype LINKEDLISTNODE_CLASS::getValue() const {
	return value;
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLISTNODE_CLASS *LINKEDLISTNODE_CLASS::getPrevious() {
	return previous;
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLISTNODE_CLASS *LINKEDLISTNODE_CLASS::getNext() {
	return next;
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
int32_t LINKEDLISTNODE_CLASS::compare(valuetype value) const {
	return _linkedlistutil_compare(this->value,value);
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLISTNODE_CLASS::print() const {
	_linkedlistutil_print(value);
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLISTNODE_CLASS::setPrevious(LINKEDLISTNODE_CLASS *previous) {
	this->previous=previous;
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLISTNODE_CLASS::setNext(LINKEDLISTNODE_CLASS *next) {
	this->next=next;
}
