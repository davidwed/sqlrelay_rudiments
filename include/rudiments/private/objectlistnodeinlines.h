// Copyright (c) 2003 David Muse
// See the COPYING file for more information

template <class datatype, class keytype>
inline int objectlistnode<datatype,keytype>::compare(keytype key) {
	return this->data->compare(key);
}

template <class datatype, class keytype>
inline void objectlistnode<datatype,keytype>::print() const {
	data->print();
}
