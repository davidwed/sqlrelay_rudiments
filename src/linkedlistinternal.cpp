// Copyright (c) 2014 David Muse
// See the COPYING file for more information

#include <rudiments/private/linkedlistinternal.h>

#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>

class linkedlistnodeinternalprivate {
	friend class linkedlistnodeinternal;
	private:
		unsigned char		*_value;
		linkedlistnodeinternal	*_next;
		linkedlistnodeinternal	*_prev;
};

linkedlistnodeinternal::linkedlistnodeinternal(
				unsigned char *value, uint64_t size) {
	pvt=new linkedlistnodeinternalprivate();
	pvt->_value=(unsigned char *)bytestring::duplicate(value,size);
	pvt->_prev=NULL;
	pvt->_next=NULL;
}

linkedlistnodeinternal::~linkedlistnodeinternal() {
	delete[] pvt->_value;
	delete pvt;
}

void linkedlistnodeinternal::setValue(unsigned char *value, uint64_t size) {
	bytestring::copy(pvt->_value,value,size);
}

unsigned char *linkedlistnodeinternal::getValue() const {
	return pvt->_value;
}

linkedlistnodeinternal *linkedlistnodeinternal::getPrevious() {
	return pvt->_prev;
}

linkedlistnodeinternal *linkedlistnodeinternal::getNext() {
	return pvt->_next;
}


void linkedlistnodeinternal::setPrevious(linkedlistnodeinternal *prev) {
	pvt->_prev=prev;
}

void linkedlistnodeinternal::setNext(linkedlistnodeinternal *next) {
	pvt->_next=next;
}

#define COMPARECASE(type) \
case sizeof(type): { \
	type	val1=*((type *)pvt->_value); \
	type	val2=*((type *)value); \
	if (val1<val2) { \
		return -1; \
	} else if (val1==val2) { \
		return 0; \
	} else { \
		return 1; \
	} \
}


int32_t linkedlistnodeinternal::compare(unsigned char *value,
					uint64_t size,
					_linkedlistutil_valuetype_t valuetype)
					const {
	switch (valuetype) {
		case INTEGER_TYPE:
			switch (size) {
				COMPARECASE(char)
				COMPARECASE(uint16_t)
				COMPARECASE(uint32_t)
				COMPARECASE(uint64_t)
			}
			return 1;
		case FLOAT_TYPE:
			switch (size) {
				COMPARECASE(float)
				COMPARECASE(double)
			}
			return 1;
		case STRING_TYPE:
			return charstring::compare(
				*((const char * const *)pvt->_value),
				*((const char * const *)value));
		default:
			return bytestring::compare(pvt->_value,value,size);
	}
}

void linkedlistnodeinternal::print(uint64_t size,
					_linkedlistutil_valuetype_t valuetype)
					const {
	switch (valuetype) {
		case INTEGER_TYPE:
			switch (size) {
				case sizeof(char):
					stdoutput.printf("%c",
						*((char *)pvt->_value));
					break;
				case sizeof(uint16_t):
					stdoutput.printf("%hd",
						*((uint16_t *)pvt->_value));
					break;
				case sizeof(uint32_t):
					stdoutput.printf("%d",
						*((uint32_t *)pvt->_value));
					break;
				case sizeof(uint64_t):
					stdoutput.printf("%lld",
						*((uint64_t *)pvt->_value));
					break;
			}
			break;
		case FLOAT_TYPE:
			stdoutput.printf("%f",*((float *)pvt->_value));
			break;
		case STRING_TYPE:
			stdoutput.printf("%s",
				*((const char * const *)pvt->_value));
			break;
		default:
			stdoutput.printf("%p",
				*((const void * const *)pvt->_value));
	}
}





class linkedlistinternalprivate {
	friend class linkedlistinternal;
	private:
		linkedlistnodeinternal		*_first;
		linkedlistnodeinternal		*_last;
		uint64_t			_length;
		uint64_t			_size;
		_linkedlistutil_valuetype_t	_valuetype;
		
};

linkedlistinternal::linkedlistinternal() {
	pvt=new linkedlistinternalprivate;
	pvt->_first=NULL;
	pvt->_last=NULL;
	pvt->_length=0;
	pvt->_size=0;
	pvt->_valuetype=OBJECT_TYPE;
}

linkedlistinternal::~linkedlistinternal() {
	clear();
	delete pvt;
}

void linkedlistinternal::init(uint64_t size,
				_linkedlistutil_valuetype_t valuetype) {
	pvt->_size=size;
	pvt->_valuetype=valuetype;
}

void linkedlistinternal::prepend(unsigned char *value) {
	prependNode(new linkedlistnodeinternal(value,pvt->_size));
}

void linkedlistinternal::prependNode(linkedlistnodeinternal *node) {
	if (!node) {
		return;
	} else if (pvt->_first) {
		pvt->_first->setPrevious(node);
		node->setNext(pvt->_first);
		pvt->_first=node;
	} else {
		pvt->_first=node;
		pvt->_last=pvt->_first;
	}
	pvt->_length++;
}

void linkedlistinternal::append(unsigned char *value) {
	appendNode(new linkedlistnodeinternal(value,pvt->_size));
}

void linkedlistinternal::appendNode(linkedlistnodeinternal *node) {
	if (!node) {
		return;
	} else if (pvt->_last) {
		pvt->_last->setNext(node);
		node->setPrevious(pvt->_last);
		pvt->_last=node;
	} else {
		pvt->_first=node;
		pvt->_last=pvt->_first;
	}
	pvt->_length++;
}

void linkedlistinternal::insertBefore(linkedlistnodeinternal *node,
						unsigned char *value) {
	insertNodeBefore(node,new linkedlistnodeinternal(value,pvt->_size));
}

void linkedlistinternal::insertNodeBefore(linkedlistnodeinternal *node,
					linkedlistnodeinternal *newnode) {
	if (!node) {
		return;
	} else if (node==pvt->_first) {
		prependNode(newnode);
	} else {
		newnode->setNext(node);
		newnode->setPrevious(node->getPrevious());
		node->getPrevious()->setNext(newnode);
		node->setPrevious(newnode);
		pvt->_length++;
	}
}

void linkedlistinternal::insertAfter(linkedlistnodeinternal *node,
						unsigned char *value) {
	insertNodeAfter(node,new linkedlistnodeinternal(value,pvt->_size));
}

void linkedlistinternal::insertNodeAfter(linkedlistnodeinternal *node,
					linkedlistnodeinternal *newnode) {
	if (!node) {
		return;
	} else if (node==pvt->_last) {
		appendNode(newnode);
	} else {
		newnode->setNext(node->getNext());
		newnode->setPrevious(node);
		node->getNext()->setPrevious(newnode);
		node->setNext(newnode);
		pvt->_length++;
	}
}

void linkedlistinternal::moveBefore(linkedlistnodeinternal *node,
					linkedlistnodeinternal *nodetomove) {
	move(node,nodetomove,true);
}

void linkedlistinternal::moveAfter(linkedlistnodeinternal *node,
					linkedlistnodeinternal *nodetomove) {
	move(node,nodetomove,false);
}

void linkedlistinternal::move(linkedlistnodeinternal *node,
					linkedlistnodeinternal *nodetomove,
					bool before) {

	if (!node || !nodetomove || node==nodetomove) {
		return;
	}

	if (nodetomove==pvt->_first) {
		pvt->_first=nodetomove->getNext();
	} else if (nodetomove==pvt->_last) {
		pvt->_last=nodetomove->getPrevious();
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
		if (node==pvt->_first) {
			pvt->_first=nodetomove;
		}
	} else {
		nodetomove->setNext(node->getNext());
		nodetomove->setPrevious(node);
		node->setNext(nodetomove);
		if (node==pvt->_last) {
			pvt->_last=nodetomove;
		}
	}
}

void linkedlistinternal::detach(linkedlistnodeinternal *node) {

	if (node==pvt->_first) {
		pvt->_first=node->getNext();
	}
	if (node==pvt->_last) {
		pvt->_last=node->getPrevious();
	}
	if (node->getPrevious()) {
		node->getPrevious()->setNext(node->getNext());
	}
	if (node->getNext()) {
		node->getNext()->setPrevious(node->getPrevious());
	}
	node->setNext(NULL);
	node->setPrevious(NULL);
	pvt->_length--;
}

bool linkedlistinternal::remove(unsigned char *value) {
	linkedlistnodeinternal	*current=find(value);
	return (current)?removeNode(current):false;
}

bool linkedlistinternal::removeAll(unsigned char *value) {

	linkedlistnodeinternal	*current=pvt->_first;
	linkedlistnodeinternal	*next;
	while (current) {
		next=current->getNext();
		if (!current->compare(value,pvt->_size,pvt->_valuetype) &&
							!removeNode(current)) {
			return false;
		}
		current=next;
	}
	return true;
}

bool linkedlistinternal::removeNode(linkedlistnodeinternal *node) {
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
	pvt->_length--;
	return true;
}

uint64_t linkedlistinternal::getLength() const {
	return pvt->_length;
}

linkedlistnodeinternal *linkedlistinternal::getFirst() {
	return pvt->_first;
}

linkedlistnodeinternal *linkedlistinternal::getLast() {
	return pvt->_last;
}

linkedlistnodeinternal *linkedlistinternal::getPrevious(
					linkedlistnodeinternal *node) {
	return (node)?node->getPrevious():NULL;
}

linkedlistnodeinternal *linkedlistinternal::getNext(
					linkedlistnodeinternal *node) {
	return (node)?node->getNext():NULL;
}

linkedlistnodeinternal *linkedlistinternal::find(unsigned char *value) {
	return find(pvt->_first,value);
}

linkedlistnodeinternal *linkedlistinternal::find(
					linkedlistnodeinternal *startnode,
					unsigned char *value) {
	for (linkedlistnodeinternal *current=startnode;
			current; current=current->getNext()) {
		if (!current->compare(value,pvt->_size,pvt->_valuetype)) {
			return current;
		}
	}
	return NULL;
}

void linkedlistinternal::insertionSort() {

	// insertion sort with a few optimizations...

	// if there are 0 or 1 items in the list then it's already sorted
	if (pvt->_length<2) {
		return;
	}

	// first and last pointers for the new list
	linkedlistnodeinternal	*newfirst=NULL;
	linkedlistnodeinternal	*newlast=NULL;

	// pointer for iterating through the new list
	linkedlistnodeinternal	*currentfromfirst=NULL;
	linkedlistnodeinternal	*currentfromlast=NULL;

	// iterate through the current list, building a new one as we go
	linkedlistnodeinternal	*node=pvt->_first;
	linkedlistnodeinternal	*next=NULL;
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
		if (newfirst->compare(node->getValue(),
					pvt->_size,pvt->_valuetype)>0) {
			node->setNext(newfirst);
			node->setPrevious(NULL);
			newfirst->setPrevious(node);
			newfirst=node;
		} else

		// if the node belongs at the end of the new list
		// (optimization for lists that are already largely backwards)
		if (newlast->compare(node->getValue(),
					pvt->_size,pvt->_valuetype)<=0) {
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
				if (currentfromfirst->compare(
						node->getValue(),
						pvt->_size,
						pvt->_valuetype)>0) {

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
				if (currentfromlast->compare(
						node->getValue(),
						pvt->_size,
						pvt->_valuetype)<=0) {

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
	pvt->_first=newfirst;
	pvt->_last=newlast;
}

void linkedlistinternal::heapSort() {

	// if there are 0 or 1 items in the list then it's already sorted
	if (pvt->_length<2) {
		return;
	}

	// build heap as a binary tree mapped to an array:
	//	parentindex = floor((childindex-1)/2)
	//	leftchildindex = parent*2+1
	//	rightchildindex = parent*2+2
	linkedlistnodeinternal	**heap=
				new linkedlistnodeinternal *[pvt->_length];
	linkedlistnodeinternal	*temp=NULL;
	uint64_t		heapend=0;
	for (linkedlistnodeinternal *node=pvt->_first;
					node; node=node->getNext()) {

		// insert node into heap
		heap[heapend]=node;

		// walk up the tree, maintaining the heap property
		// (higher values higher up in the tree)
		uint64_t	child=heapend;
		while (child) {

			// get the parent index
			uint64_t	parent=(child-1)/2;

			// swap nodes if necessary
			if (heap[parent]->compare(heap[child]->getValue(),
						pvt->_size,pvt->_valuetype)<0) {
				temp=heap[parent];
				heap[parent]=heap[child];
				heap[child]=temp;
			}

			// move up
			child=parent;
		}
		
		// move on
		heapend++;
	}

	// reset the heap end index
	heapend--;

	// Build a new list from the heap by swapping the root and last leaf
	// node (index 0 is the root and the last index is the last leaf),
	// pulling the value off of the last leaf node, and sifting the tree to
	// maintain the heap property (higher values higher up in the tree),
	// over and over again.  We'll shortcut the swap and pull-off part a
	// bit...

	// first and last pointers for the new list
	linkedlistnodeinternal	*newfirst=NULL;
	linkedlistnodeinternal	*newlast=NULL;

	// extract values from the heap...
	for (;;) {

		// pull off the highest value (which is always at the root
		// of the tree, index 0 in the array) and prepend it to the
		// new array
		linkedlistnodeinternal	*node=heap[0];
		if (!newfirst) {
			node->setPrevious(NULL);
			node->setNext(NULL);
			newfirst=node;
			newlast=node;
		} else {
			node->setPrevious(NULL);
			node->setNext(newfirst);
			newfirst->setPrevious(node);
			newfirst=node;
		}

		// when the tree is empty, we're done
		if (!heapend) {

			// make the new list the current list
			pvt->_first=newfirst;
			pvt->_last=newlast;

			// clean up
			delete[] heap;
			return;
		}

		// move the value at the last leaf node (end of the array)
		// to the root node (index 0 of the array)
		heap[0]=heap[heapend];
		heapend--;

		// sift the tree to maintain the heap property
		// (higher values higher up in the tree)
		uint64_t parent=0;
		for (;;) {

			// make sure there's at least a left child
			uint64_t	leftchild=parent*2+1;
			if (leftchild>heapend) {
				break;
			}

			// is the left child greater?
			uint64_t	greater=parent;
			if (heap[parent]->compare(heap[leftchild]->getValue(),
						pvt->_size,pvt->_valuetype)<0) {
				greater=leftchild;
			}

			// is the right child greater?
			uint64_t	rightchild=leftchild+1;
			if (rightchild<=heapend &&
				heap[rightchild]->compare(
						heap[greater]->getValue(),
						pvt->_size,pvt->_valuetype)>0) {
				greater=rightchild;
			}

			// if the parent was greater than each child then
			// we don't need to continue sifting
			if (greater==parent) {
				break;
			}

			// if one of the children was greater than the parent
			// then swap them and continue down the tree in the
			// direction of the child that was swapped
			temp=heap[parent];
			heap[parent]=heap[greater];
			heap[greater]=temp;
			parent=greater;
		}
	}
}

void linkedlistinternal::clear() {
	linkedlistnodeinternal	*next;
	linkedlistnodeinternal	*current=pvt->_first;
	while (current) {
		next=current->getNext();
		delete current;
		current=next;
	}
	pvt->_first=NULL;
	pvt->_last=NULL;
	pvt->_length=0;
}

void linkedlistinternal::print() const {
	print(pvt->_length);
}

void linkedlistinternal::print(uint64_t count) const {
	uint64_t	i=0;
	for (linkedlistnodeinternal *current=pvt->_first;
			current && i<count; current=current->getNext()) {
		stdoutput.printf("index %lld: ",(long long)i);
		current->print(pvt->_size,pvt->_valuetype);
		stdoutput.printf("\n");
		i++;
	}
}
