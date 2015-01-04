// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/rudimentsinlines.h>
#include <rudiments/private/linkedlistutilinlines.h>

#define DICTIONARY_TEMPLATE \
	template <class keytype, class valuetype>

#define DICTIONARY_CLASS \
	dictionary<keytype,valuetype>

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
DICTIONARY_CLASS::dictionary() {
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
DICTIONARY_CLASS::~dictionary() {
	dict.clear();
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARY_CLASS::setValue(keytype key, valuetype value) {
	linkedlistnode< dictionarynode<keytype,valuetype> *> *node=find(key);
	if (node) {
		node->getValue()->setValue(value);
	} else {
		dict.append(new dictionarynode<keytype,valuetype>(key,value));
	}
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool DICTIONARY_CLASS::getValue(keytype key, valuetype *value) {
	linkedlistnode< dictionarynode<keytype,valuetype> *> *node=find(key);
	if (node) {
		*value=node->getValue()->getValue();
		return true;
	}
	return false;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
valuetype DICTIONARY_CLASS::getValue(keytype key) {
	linkedlistnode< dictionarynode<keytype,valuetype> *> *node=find(key);
	if (node) {
		return node->getValue()->getValue();
	}
	return (valuetype)0;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
dictionarynode<keytype,valuetype> *DICTIONARY_CLASS::getNode(keytype key) {
	linkedlistnode< dictionarynode<keytype,valuetype> *> *node=find(key);
	if (node) {
		return node->getValue();
	}
	return NULL;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool DICTIONARY_CLASS::remove(keytype key) {
	linkedlistnode< dictionarynode<keytype,valuetype> *> *node=find(key);
	if (node) {
		return dict.remove(node);
	}
	return false;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
dictionarynode<keytype,valuetype> *DICTIONARY_CLASS::detach(keytype key) {
	linkedlistnode< dictionarynode<keytype,valuetype> *> *node=find(key);
	if (node) {
		dict.detach(node);
		dictionarynode<keytype,valuetype> *contents=node->getValue();
		delete node;
		return contents;
	}
	return NULL;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode< dictionarynode<keytype,valuetype> *> *DICTIONARY_CLASS::
	find(keytype key) {
	for (linkedlistnode< dictionarynode<keytype,valuetype> *> *node=
			dict.getFirst(); node; node=node->getNext()) {
		if (!node->getValue()->compare(key)) {
			return node;
		}
	}
	return NULL;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlist< keytype > *DICTIONARY_CLASS::getKeys() {
	linkedlist< keytype >	*keys=new linkedlist< keytype >();
	for (linkedlistnode< dictionarynode< keytype, valuetype > *> *node=
			dict.getFirst(); node; node=node->getNext()) {
		keys->append(node->getValue()->getKey());
	}
	return keys;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlist< dictionarynode<keytype,valuetype> *> *DICTIONARY_CLASS::getList() {
	return &dict;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARY_CLASS::clear() {
	for (linkedlistnode< dictionarynode<keytype,valuetype> *> *node=
			dict.getFirst(); node; node=node->getNext()) {
		delete node->getValue();
	}
	dict.clear();
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARY_CLASS::print() {
	for (linkedlistnode< dictionarynode<keytype,valuetype> *> *node=
			dict.getFirst(); node; node=node->getNext()) {
		node->getValue()->print();
		stdoutput.printf("\n");
	}
}

#define DICTIONARYNODE_TEMPLATE \
	template <class keytype, class valuetype>

#define DICTIONARYNODE_CLASS \
	dictionarynode<keytype,valuetype>

DICTIONARYNODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
DICTIONARYNODE_CLASS::dictionarynode(keytype key, valuetype value) {
	this->key=key;
	this->value=value;
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
