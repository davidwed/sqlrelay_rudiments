// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/rudimentsinlines.h>

#define DICTIONARY_TEMPLATE \
	template <class keytype, class datatype>

#define DICTIONARY_CLASS \
	dictionary<keytype,datatype>

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
void DICTIONARY_CLASS::setData(keytype key, datatype data) {
	linkedlistnode< dictionarynode<keytype,datatype> *> *node=findNode(key);
	if (node) {
		node->getData()->setData(data);
	} else {
		dictionarynode<keytype,datatype>	*dictnode=
					new dictionarynode<keytype,datatype>();
		dictnode->setKey(key);
		dictnode->setData(data);
		dict.append(dictnode);
	}
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool DICTIONARY_CLASS::getData(keytype key, datatype *data) {
	linkedlistnode< dictionarynode<keytype,datatype> *> *node=findNode(key);
	if (node) {
		*data=node->getData()->getData();
		return true;
	}
	return false;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool DICTIONARY_CLASS::removeData(keytype key) {
	linkedlistnode< dictionarynode<keytype,datatype> *> *node=findNode(key);
	if (node) {
		return dict.removeNode(node);
	}
	return false;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlistnode< dictionarynode<keytype,datatype> *> *DICTIONARY_CLASS::
	findNode(keytype key) {
	for (linkedlistnode< dictionarynode<keytype,datatype> *> *node=
		dict.getFirstNode();
		node; node=node->getNext()) {
		if (!node->getData()->compare(key)) {
			return node;
		}
	}
	return NULL;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
linkedlist< dictionarynode<keytype,datatype> *> *DICTIONARY_CLASS::getList() {
	return &dict;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARY_CLASS::clear() {
	for (linkedlistnode< dictionarynode<keytype,datatype> *> *node=
		dict.getFirstNode();
		node; node=node->getNext()) {
		delete node->getData();
	}
	dict.clear();
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARY_CLASS::print() {
	for (linkedlistnode< dictionarynode<keytype,datatype> *> *node=
		dict.getFirstNode();
		node; node=node->getNext()) {
		node->getData()->print();
		stdoutput.printf("\n");
	}
}
