// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/rudimentsinlines.h>

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
	linkedlistnode< dictionarynode<keytype,valuetype> *>
						*node=findNode(key);
	if (node) {
		node->getValue()->setValue(value);
	} else {
		dictionarynode<keytype,valuetype>	*dictnode=
					new dictionarynode<keytype,valuetype>();
		dictnode->setKey(key);
		dictnode->setValue(value);
		dict.append(dictnode);
	}
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool DICTIONARY_CLASS::getValue(keytype key, valuetype *value) {
	linkedlistnode< dictionarynode<keytype,valuetype> *>
						*node=findNode(key);
	if (node) {
		*value=node->getValue()->getValue();
		return true;
	}
	return false;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool DICTIONARY_CLASS::removeValue(keytype key) {
	linkedlistnode< dictionarynode<keytype,valuetype> *>
						*node=findNode(key);
	if (node) {
		return dict.removeNode(node);
	}
	return false;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode< dictionarynode<keytype,valuetype> *> *DICTIONARY_CLASS::
	findNode(keytype key) {
	for (linkedlistnode< dictionarynode<keytype,valuetype> *> *node=
		dict.getFirstNode();
		node; node=node->getNext()) {
		if (!node->getValue()->compare(key)) {
			return node;
		}
	}
	return NULL;
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
		dict.getFirstNode();
		node; node=node->getNext()) {
		delete node->getValue();
	}
	dict.clear();
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARY_CLASS::print() {
	for (linkedlistnode< dictionarynode<keytype,valuetype> *> *node=
		dict.getFirstNode();
		node; node=node->getNext()) {
		node->getValue()->print();
		stdoutput.printf("\n");
	}
}
