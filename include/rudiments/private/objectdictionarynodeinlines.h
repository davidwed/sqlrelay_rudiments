// Copyright (c) 2003 David Muse
// See the COPYING file for more information

template <class keytype, class valuetype>
inline int objectdictionarynode<keytype,valuetype>::compare(keytype key) {
	return key->compare(key);
}
