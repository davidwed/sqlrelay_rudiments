// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <stdio.h>

#define DICTIONARY_TEMPLATE \
	template <class keytype, class datatype, class dictionarynodetype>

#define DICTIONARY_CLASS \
	dictionary<keytype,datatype,dictionarynodetype>

DICTIONARY_TEMPLATE
inline DICTIONARY_CLASS::dictionary() {
}

DICTIONARY_TEMPLATE
inline DICTIONARY_CLASS::~dictionary() {
	dict.clear();
}

DICTIONARY_TEMPLATE
inline void DICTIONARY_CLASS::setData(keytype key, datatype data) {
	dictionarynode<keytype,datatype>	*node;
	if (dict.getDataByKey(key,&node)) {
		node->setData(data);
	} else {
		node=new dictionarynodetype(key,data);
		dict.append(node);
	}
}

DICTIONARY_TEMPLATE
inline int DICTIONARY_CLASS::getData(keytype key, datatype *data) {
	dictionarynode<keytype,datatype>	*node;
	if (dict.getDataByKey(key,&node)) {
		*data=node->getData();
		return 1;
	}
	return 0;
}

DICTIONARY_TEMPLATE
inline int DICTIONARY_CLASS::removeData(keytype key) {
	return dict.removeByKey(key);
}

DICTIONARY_TEMPLATE
inline unsigned long DICTIONARY_CLASS::getLength() const {
	return dict.getLength();
}

DICTIONARY_TEMPLATE
inline int DICTIONARY_CLASS::getKey(unsigned long index, keytype *key) const {
	dictionarynode<keytype,datatype>	*node;
	if (dict.getDataByIndex(index,&node)) {
		*key=node->getKey();
		return 1;
	}
	return 0;
}

DICTIONARY_TEMPLATE
inline int DICTIONARY_CLASS::getData(unsigned long index, datatype *data)
									const {
	dictionarynode<keytype,datatype>	*node;
	if (dict.getDataByIndex(index,&node)) {
		*data=node->getData();
		return 1;
	}
	return 0;
}
