// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#ifndef EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS

#include <stdlib.h>
#include <stdio.h>

#include <rudiments/private/rudimentsinlines.h>

#define DICTIONARY_TEMPLATE \
	template <class keytype, class datatype, \
			class dictionarynodetype, \
			class dictionarylistnodetype, \
			class dictionarylisttype>

#define DICTIONARY_CLASS \
	dictionary<keytype,datatype,dictionarynodetype,\
			dictionarylistnodetype,dictionarylisttype>

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
DICTIONARY_CLASS::dictionary() {
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
DICTIONARY_CLASS::~dictionary() {
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARY_CLASS::setData(keytype key, datatype data) {
	dictionarylistnodetype	*node=findNode(key);
	if (node) {
		node->getData()->setData(data);
	} else {
		dictionarynodetype	*dictnode=new dictionarynodetype();
		dictnode->setKey(key);
		dictnode->setData(data);
		dict.append(dictnode);
	}
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool DICTIONARY_CLASS::getData(keytype key, datatype *data) {
	dictionarylistnodetype	*node=findNode(key);
	if (node) {
		*data=node->getData()->getData();
		return true;
	}
	return false;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool DICTIONARY_CLASS::removeData(keytype key) {
	dictionarylistnodetype	*node=findNode(key);
	if (node) {
		return dict.removeNode(node);
	}
	return false;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
dictionarylistnodetype *DICTIONARY_CLASS::findNode(keytype key) {
	dictionarylistnodetype	*node=
			(dictionarylistnodetype *)dict.getNodeByIndex(0);
	while (node) {
		if (!node->getData()->compare(key)) {
			return node;
		}
		node=(dictionarylistnodetype *)node->getNext();
	}
	return NULL;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
dictionarylisttype *DICTIONARY_CLASS::getList() {
	return &dict;
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARY_CLASS::clear() {
	dict.clear();
}

DICTIONARY_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void DICTIONARY_CLASS::print() {
	dictionarylistnodetype	*node=
			(dictionarylistnodetype *)dict.getNodeByIndex(0);
	while (node) {
		node->getData()->print();
		printf("\n");
		node=(dictionarylistnodetype *)node->getNext();
	}
}



template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
stringdictionarynode<datatype>::~stringdictionarynode() {}

template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
stringdictionarylistnode<datatype>::~stringdictionarylistnode() {}

template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
stringdictionarylist<datatype>::~stringdictionarylist() {}

template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
stringdictionary<datatype>::~stringdictionary() {}



template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
numericdictionarynode<datatype>::~numericdictionarynode() {}

template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
numericdictionarylistnode<datatype>::~numericdictionarylistnode() {}

template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
numericdictionarylist<datatype>::~numericdictionarylist() {}

template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
numericdictionary<datatype>::~numericdictionary() {}

#endif
