// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#define DICTIONARYNODE_TEMPLATE \
	template <class keytype, class datatype>

#define DICTIONARYNODE_CLASS \
	dictionarynode<keytype,datatype>

DICTIONARYNODE_TEMPLATE
inline DICTIONARYNODE_CLASS::dictionarynode() {
	key=0;
	data=0;
}

DICTIONARYNODE_TEMPLATE
inline void DICTIONARYNODE_CLASS::setKey(keytype key) {
	this->key=key;
}

DICTIONARYNODE_TEMPLATE
inline void DICTIONARYNODE_CLASS::setData(datatype data) {
	this->data=data;
}

DICTIONARYNODE_TEMPLATE
inline keytype DICTIONARYNODE_CLASS::getKey() const {
	return key;
}

DICTIONARYNODE_TEMPLATE
inline datatype DICTIONARYNODE_CLASS::getData() const {
	return data;
}

DICTIONARYNODE_TEMPLATE
inline int DICTIONARYNODE_CLASS::compare(keytype testkey) const {
	return keyutil.compare(key,testkey);
}

DICTIONARYNODE_TEMPLATE
inline void DICTIONARYNODE_CLASS::print() const {
	keyutil.print(key);
	datautil.print(data);
}
