// Copyright (c) 2003 David Muse
// See the COPYING file for more information

template <class keytype, class datatype>
inline int objectdictionarynode<keytype,datatype>::compare(keytype key) const {
	return this->key->compare(key);
}

template <class keytype, class datatype>
inline void objectdictionarynode<keytype,datatype>::print() const {
	key->print();
	printf("data: %x\n",data);
}
