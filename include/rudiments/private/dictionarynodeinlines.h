// Copyright (c) 2003 David Muse
// See the COPYING file for more information

template <class keytype, class datatype>
inline dictionarynode<keytype,datatype>::dictionarynode() {
	key=0;
	data=0;
}

template <class keytype, class datatype>
inline dictionarynode<keytype,datatype>::dictionarynode(keytype key,
							datatype data) {
	this->key=key;
	this->data=data;
}

template <class keytype, class datatype>
inline void dictionarynode<keytype,datatype>::setKey(keytype key) {
	this->key=key;
}

template <class keytype, class datatype>
inline void dictionarynode<keytype,datatype>::setData(datatype data) {
	this->data=data;
}

template <class keytype, class datatype>
inline keytype dictionarynode<keytype,datatype>::getKey() const {
	return key;
}

template <class keytype, class datatype>
inline datatype dictionarynode<keytype,datatype>::getData() const {
	return data;
}
