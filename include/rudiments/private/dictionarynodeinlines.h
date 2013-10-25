// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>
#include <rudiments/private/linkedlistutilinlines.h>

#define DICTIONARYNODE_TEMPLATE \
	template <class keytype, class valuetype>

#define DICTIONARYNODE_CLASS \
	dictionarynode<keytype,valuetype>

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
DICTIONARYNODE_CLASS::dictionarynode() {
	key=0;
	value=0;
}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
DICTIONARYNODE_CLASS::~dictionarynode() {}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARYNODE_CLASS::setKey(keytype key) {
	this->key=key;
}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARYNODE_CLASS::setValue(valuetype value) {
	this->value=value;
}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
keytype DICTIONARYNODE_CLASS::getKey() const {
	return key;
}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
valuetype DICTIONARYNODE_CLASS::getValue() const {
	return value;
}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
int32_t DICTIONARYNODE_CLASS::compare(keytype testkey) const {
	return _linkedlistutil_compare(key,testkey);
}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARYNODE_CLASS::print() const {
	_linkedlistutil_print(key);
	stdoutput.printf(":");
	_linkedlistutil_print(value);
}
