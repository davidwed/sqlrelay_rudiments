// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#ifndef EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS

#include <rudiments/private/rudimentsinlines.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#define DICTIONARYNODE_TEMPLATE \
	template <class keytype, class datatype>

#define DICTIONARYNODE_CLASS \
	dictionarynode<keytype,datatype>

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
DICTIONARYNODE_CLASS::dictionarynode() {
	key=0;
	data=0;
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
void DICTIONARYNODE_CLASS::setData(datatype data) {
	this->data=data;
}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
keytype DICTIONARYNODE_CLASS::getKey() const {
	return key;
}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
datatype DICTIONARYNODE_CLASS::getData() const {
	return data;
}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
int DICTIONARYNODE_CLASS::compare(keytype testkey) const {
	return keyutil.compare(key,testkey);
}

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARYNODE_CLASS::print() const {
	keyutil.print(key);
	printf(":");
	datautil.print(data);
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
