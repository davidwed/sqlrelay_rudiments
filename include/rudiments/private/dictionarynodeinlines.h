// Copyright (c) 2003 David Muse
// See the COPYING file for more information

template <class keytype, class valuetype>
inline dictionarynode<keytype,valuetype>::dictionarynode() {
	key=0;
	value=0;
}

template <class keytype, class valuetype>
inline dictionarynode<keytype,valuetype>::dictionarynode(keytype key,
							valuetype value) {
	this->key=key;
	this->value=value;
}

template <class keytype, class valuetype>
inline void dictionarynode<keytype,valuetype>::setKey(keytype key) {
	this->key=key;
}

template <class keytype, class valuetype>
inline void dictionarynode<keytype,valuetype>::setValue(valuetype value) {
	this->value=value;
}

template <class keytype, class valuetype>
inline keytype dictionarynode<keytype,valuetype>::getKey() const {
	return key;
}

template <class keytype, class valuetype>
inline valuetype dictionarynode<keytype,valuetype>::getValue() const {
	return value;
}
