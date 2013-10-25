// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include <rudiments/private/rudimentsinlines.h>

// Ideally we'd use explicit specialization here but old enough
// compilers don't support it and this isn't any less efficient.

RUDIMENTS_TEMPLATE_INLINE
int32_t _linkedlistutil_compare(char *value1, char *value2) {
	return charstring::compare(value1,value2);
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _linkedlistutil_compare(const char *value1, const char *value2) {
	return charstring::compare(value1,value2);
}

template <class valuetype>
RUDIMENTS_TEMPLATE_INLINE
int32_t _linkedlistutil_compare(valuetype value1, valuetype value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(const char *value) {
	stdoutput.printf("%s",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(char *value) {
	stdoutput.printf("%s",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(char value) {
	stdoutput.printf("%c",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(int32_t value) {
	stdoutput.printf("%d",(int)value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(int16_t value) {
	stdoutput.printf("%hd",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(int64_t value) {
	stdoutput.printf("%lld",(long long)value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(float value) {
	stdoutput.printf("%f",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(double value) {
	stdoutput.printf("%f",value);
}

template <class valuetype>
RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(valuetype value) {
	stdoutput.printf("%p",value);
}

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
	return baselinkedlist::getFirstNode();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getLastNode() {
	return baselinkedlist::getLastNode();
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getNodeByIndex(uint64_t index) {
	return baselinkedlist::getNodeByIndex(index);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getNodeByValue(valuetype value) {
	return baselinkedlist::getNodeByValue((uint64_t)value);
}

LINKEDLIST_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode<valuetype> *LINKEDLIST_CLASS::getNodeByValue(
					linkedlistnode<valuetype> *startnode,
					valuetype value) {
	return baselinkedlist::getNodeByValue(startnode,(uint64_t)value);
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

#define LINKEDLISTNODE_TEMPLATE template <class valuetype>

#define LINKEDLISTNODE_CLASS linkedlistnode<valuetype>

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLISTNODE_CLASS::linkedlistnode() : baselinkedlistnode() {
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLISTNODE_CLASS::~linkedlistnode() {
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLISTNODE_CLASS::setValue(valuetype value) {
	baselinkedlistnode::setValue((uint64_t)value);
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
valuetype LINKEDLISTNODE_CLASS::getValue() const {
	return (valuetype)baselinkedlistnode::getValue();
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLISTNODE_CLASS::setPrevious(LINKEDLISTNODE_CLASS *previous) {
	return baselinkedlistnode::setPrevious(previous);
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLISTNODE_CLASS::setNext(LINKEDLISTNODE_CLASS *next) {
	return baselinkedlistnode::setNext(next);
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLISTNODE_CLASS *LINKEDLISTNODE_CLASS::getPrevious() {
	return baselinkedlistnode::getPrevious();
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
LINKEDLISTNODE_CLASS *LINKEDLISTNODE_CLASS::getNext() {
	return baselinkedlistnode::getNext();
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
int32_t LINKEDLISTNODE_CLASS::compare(valuetype value) const {
	return compare((uint64_t)value);
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
int32_t LINKEDLISTNODE_CLASS::compare(uint64_t value) const {
	return _linkedlistutil_compare(getValue(),(valuetype)value);
}

LINKEDLISTNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void LINKEDLISTNODE_CLASS::print() const {
	_linkedlistutil_print((valuetype)getValue());
}
