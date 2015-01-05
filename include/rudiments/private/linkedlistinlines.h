// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/rudimentsinlines.h>
#include <rudiments/private/linkedlistinternal.h>
#include <rudiments/private/linkedlistutilinlines.h>

#define LINKEDLIST_TEMPLATE template <class valuetype>

#define LINKEDLIST_CLASS linkedlist<valuetype>

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLIST_CLASS::linkedlist() : linkedlistinternal() {
	valuetype	v=0;
	linkedlistinternal::init(sizeof(valuetype),
				_linkedlistutil_valuetype(v));
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLIST_CLASS::~linkedlist() {
	clear();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::prepend(valuetype value) {
	linkedlistinternal::prepend((unsigned char *)&value);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::prepend(linkedlistnode<valuetype> *node) {
	linkedlistinternal::prepend(node);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::append(valuetype value) {
	linkedlistinternal::append((unsigned char *)&value);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::append(linkedlistnode<valuetype> *node) {
	linkedlistinternal::appendNode(node);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::insertBefore(linkedlistnode<valuetype> *node,
							valuetype value) {
	linkedlistinternal::insertBefore(node,(unsigned char *)&value);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::insertBefore(linkedlistnode<valuetype> *node,
					linkedlistnode<valuetype> *newnode) {
	linkedlistinternal::insertBefore(node,newnode);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::insertAfter(linkedlistnode<valuetype> *node,
							valuetype value) {
	linkedlistinternal::insertAfter(node,(unsigned char *)&value);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::insertAfter(linkedlistnode<valuetype> *node,
					linkedlistnode<valuetype> *newnode) {
	linkedlistinternal::insertAfter(node,newnode);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::moveBefore(linkedlistnode<valuetype> *node,
					linkedlistnode<valuetype> *nodetomove) {
	linkedlistinternal::moveBefore(node,nodetomove);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::moveAfter(linkedlistnode<valuetype> *node,
					linkedlistnode<valuetype> *nodetomove) {
	linkedlistinternal::moveAfter(node,nodetomove);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::move(linkedlistnode<valuetype> *node,
					linkedlistnode<valuetype> *nodetomove,
					bool before) {
	linkedlistinternal::move(node,nodetomove,before);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::detach(linkedlistnode<valuetype> *node) {
	linkedlistinternal::detach(node);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::remove(valuetype value) {
	return linkedlistinternal::remove((unsigned char *)&value);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeAll(valuetype value) {
	return linkedlistinternal::removeAll((unsigned char *)&value);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::remove(linkedlistnode<valuetype> *node) {
	return linkedlistinternal::removeNode(node);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
uint64_t LINKEDLIST_CLASS::getLength() const {
	return linkedlistinternal::getLength();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getFirst() {
	return (linkedlistnode<valuetype> *)linkedlistinternal::getFirst();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getLast() {
	return (linkedlistnode<valuetype> *)
		linkedlistinternal::getLast();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getPrevious(
					linkedlistnode<valuetype> *node) {
	return (linkedlistnode<valuetype> *)
		linkedlistinternal::getPrevious(node);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getNext(
					linkedlistnode<valuetype> *node) {
	return (linkedlistnode<valuetype> *)
		linkedlistinternal::getNext(node);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::find(valuetype value) {
	return (linkedlistnode<valuetype> *)
		linkedlistinternal::find((unsigned char *)&value);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::find(
					linkedlistnode<valuetype> *startnode,
					valuetype value) {
	return (linkedlistnode<valuetype> *)
		linkedlistinternal::find(startnode,(unsigned char *)&value);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::insertionSort() {
	linkedlistinternal::insertionSort();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::heapSort() {
	linkedlistinternal::heapSort();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::clear() {
	linkedlistinternal::clear();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::print() const {
	linkedlistinternal::print();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::print(uint64_t count) const {
	linkedlistinternal::print(count);
}

#define LINKEDLISTNODE_TEMPLATE template <class valuetype>

#define LINKEDLISTNODE_CLASS linkedlistnode<valuetype>

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLISTNODE_CLASS::linkedlistnode(valuetype value) :
		linkedlistnodeinternal((unsigned char *)&value,
						sizeof(valuetype)) {
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLISTNODE_CLASS::~linkedlistnode() {
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLISTNODE_CLASS::setValue(valuetype value) {
	linkedlistnodeinternal::setValue((unsigned char *)&value,
						sizeof(valuetype));
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
valuetype LINKEDLISTNODE_CLASS::getValue() const {
	return *(valuetype *)(linkedlistnodeinternal::getValue());
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLISTNODE_CLASS *LINKEDLISTNODE_CLASS::getPrevious() {
	return (LINKEDLISTNODE_CLASS *)linkedlistnodeinternal::getPrevious();
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLISTNODE_CLASS *LINKEDLISTNODE_CLASS::getNext() {
	return (LINKEDLISTNODE_CLASS *)linkedlistnodeinternal::getNext();
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
int32_t LINKEDLISTNODE_CLASS::compare(valuetype value) const {
	valuetype	v=0;
	return linkedlistnodeinternal::compare(
				(unsigned char *)&value,
				sizeof(valuetype),
				_linkedlistutil_valuetype(v));
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
int32_t LINKEDLISTNODE_CLASS::compare(linkedlistnode<valuetype> *peer) const {
	return compare(peer->getValue());
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLISTNODE_CLASS::print() const {
	valuetype	v=0;
	linkedlistnodeinternal::print(sizeof(valuetype),
				_linkedlistutil_valuetype(v));
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLISTNODE_CLASS::setPrevious(LINKEDLISTNODE_CLASS *previous) {
	linkedlistnodeinternal::setPrevious(previous);
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLISTNODE_CLASS::setNext(LINKEDLISTNODE_CLASS *next) {
	linkedlistnodeinternal::setNext(next);
}
