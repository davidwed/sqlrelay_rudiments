// Copyright (c) 2003 David Muse
// See the COPYING file for more information

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
inline DICTIONARY_CLASS::dictionary() {
}

DICTIONARY_TEMPLATE
inline DICTIONARY_CLASS::~dictionary() {
}

DICTIONARY_TEMPLATE
inline int DICTIONARY_CLASS::setData(keytype key, datatype data) {
	dictionarylistnodetype	*node=findNode(key);
	if (node) {
		node->getData()->setData(data);
		return 0;
	} else {
		dictionarynodetype	*dictnode=new dictionarynodetype();
		dictnode->setKey(key);
		dictnode->setData(data);
		dict.append(dictnode);
		return 1;
	}
}

DICTIONARY_TEMPLATE
inline int DICTIONARY_CLASS::getData(keytype key, datatype *data) {
	dictionarylistnodetype	*node=findNode(key);
	if (node) {
		*data=node->getData()->getData();
		return 1;
	}
	return 0;
}

DICTIONARY_TEMPLATE
inline int DICTIONARY_CLASS::removeData(keytype key) {
	dictionarylistnodetype	*node=findNode(key);
	if (node) {
		return dict.removeNode(node);
	}
	return 0;
}

DICTIONARY_TEMPLATE
inline dictionarylistnodetype *DICTIONARY_CLASS::findNode(keytype key) const {
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
inline dictionarylisttype *DICTIONARY_CLASS::getList() {
	return &dict;
}
