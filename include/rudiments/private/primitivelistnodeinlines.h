// Copyright (c) 2003 David Muse
// See the COPYING file for more information

template <class datatype>
inline int primitivelistnode<datatype>::compare(datatype key) {
	if (value<key) {
		return -1;
	} else if (value==key) {
		return 0;
	} else {
		return 1;
	}
}
