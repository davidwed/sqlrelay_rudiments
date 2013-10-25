// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/rudimentsinlines.h>
#include <rudiments/private/linkedlistutilinlines.h>

#define LINKEDLIST_TEMPLATE template <class valuetype>

#define LINKEDLIST_CLASS linkedlist<valuetype>

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLIST_CLASS::linkedlist() : baselinkedlist() {
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLIST_CLASS::~linkedlist() {
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
baselinkedlistnode *LINKEDLIST_CLASS::newNode() {
	return new linkedlistnode< valuetype >();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::append(valuetype value) {
	baselinkedlist::append((int64_t)value,sizeof(value));
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::append(linkedlistnode<valuetype> *node) {
	baselinkedlist::append(node);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::insert(uint64_t index, valuetype value) {
	return baselinkedlist::insert(index,(uint64_t)value,sizeof(value));
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::insert(uint64_t index, linkedlistnode<valuetype> *node) {
	return baselinkedlist::insert(index,node);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::setValueByIndex(uint64_t index, valuetype value) {
	return baselinkedlist::setValueByIndex(index,
					(uint64_t)value,sizeof(value));
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeByIndex(uint64_t index) {
	return baselinkedlist::removeByIndex(index);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeByValue(valuetype value) {
	return baselinkedlist::removeByValue((uint64_t)value);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeAllByValue(valuetype value) {
	return baselinkedlist::removeAllByValue((uint64_t)value);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::removeNode(linkedlistnode<valuetype> *node) {
	return baselinkedlist::removeNode(node);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool LINKEDLIST_CLASS::getValueByIndex(uint64_t index, valuetype *value) {
	uint64_t	val;
	if (!baselinkedlist::getValueByIndex(index,&val)) {
		return false;
	}
	*value=(valuetype)val;
	return true;
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
uint64_t LINKEDLIST_CLASS::getLength() const {
	return baselinkedlist::getLength();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getFirstNode() {
	return (linkedlistnode<valuetype> *)
		baselinkedlist::getFirstNode();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getLastNode() {
	return (linkedlistnode<valuetype> *)
		baselinkedlist::getLastNode();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getNodeByIndex(uint64_t index) {
	return (linkedlistnode<valuetype> *)
		baselinkedlist::getNodeByIndex(index);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getNodeByValue(valuetype value) {
	return (linkedlistnode<valuetype> *)
		baselinkedlist::getNodeByValue((uint64_t)value);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getNodeByValue(
					linkedlistnode<valuetype> *startnode,
					valuetype value) {
	return (linkedlistnode<valuetype> *)
		baselinkedlist::getNodeByValue(startnode,(uint64_t)value);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::clear() {
	baselinkedlist::clear();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLIST_CLASS::print() const {
	baselinkedlist::print();
}

template <class valuetype>
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype>::linkedlistnode() : baselinkedlistnode() {
}

template <class valuetype>
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype>::~linkedlistnode() {
}

template <class valuetype>
RUDIMENTS_TEMPLATE_INLINE
void linkedlistnode<valuetype>::setValue(valuetype value) {
	baselinkedlistnode::setValue((uint64_t)value,sizeof(value));
}

template <class valuetype>
RUDIMENTS_TEMPLATE_INLINE
valuetype linkedlistnode<valuetype>::getValue() const {
	return (valuetype)baselinkedlistnode::getValue();
}

template <class valuetype>
RUDIMENTS_TEMPLATE_INLINE
void linkedlistnode<valuetype>::setPrevious(
				linkedlistnode<valuetype> *previous) {
	return baselinkedlistnode::setPrevious(previous);
}

template <class valuetype>
RUDIMENTS_TEMPLATE_INLINE
void linkedlistnode<valuetype>::setNext(linkedlistnode<valuetype> *next) {
	return baselinkedlistnode::setNext(next);
}

template <class valuetype>
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *linkedlistnode<valuetype>::getPrevious() {
	return (linkedlistnode<valuetype> *)baselinkedlistnode::getPrevious();
}

template <class valuetype>
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *linkedlistnode<valuetype>::getNext() {
	return (linkedlistnode<valuetype> *)baselinkedlistnode::getNext();
}

template <class valuetype>
RUDIMENTS_TEMPLATE_INLINE
int32_t linkedlistnode<valuetype>::compare(valuetype value) const {
	return compareInternal((uint64_t)value);
}

template <class valuetype>
RUDIMENTS_TEMPLATE_INLINE
int32_t linkedlistnode<valuetype>::compareInternal(uint64_t value) const {
	return _linkedlistutil_compare(getValue(),(valuetype)value);
}

template <class valuetype>
RUDIMENTS_TEMPLATE_INLINE
void linkedlistnode<valuetype>::print() const {
	_linkedlistutil_print((valuetype)getValue());
}
