// Copyright (c) 2003 David Muse
// See the COPYING file for more information

template <class keytype, class valuetype>
inline int primitivedictionarynode<keytype,valuetype>::compare(keytype key) {
	if (this->key<key) {
		return -1;
	} else if (this->key==key) {
		return 0;
	} else {
		return 1;
	}
}
