// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS
	#include <strings.h>
#endif

template <class datatype>
inline int linkedlistutil<datatype>::
			compare(datatype data1, datatype data2) const {
	if (data1<data2) {
		return -1;
	} else if (data1==data2) {
		return 0;
	} else {
		return 1;
	}
}

template <>
inline void linkedlistutil<char *>::print(char *data) const {
	printf("%s",data);
}

template <>
inline int linkedlistutil<char *>::compare(char *data1, char *data2) const {
	return strcmp(data1,data2);
}

template <>
inline void linkedlistutil<char>::print(char data) const {
	printf("%c",data);
}

template <>
inline void linkedlistutil<int>::print(int data) const {
	printf("%d",data);
}

template <>
inline void linkedlistutil<short>::print(short data) const {
	printf("%hd",data);
}

template <>
inline void linkedlistutil<long>::print(long data) const {
	printf("%ld",data);
}

template <>
inline void linkedlistutil<float>::print(float data) const {
	printf("%f",data);
}

template <>
inline void linkedlistutil<double>::print(double data) const {
	printf("%f",data);
}

template <class datatype>
inline void linkedlistutil<datatype>::print(datatype data) const {
	printf("%lx",(unsigned long)data);
}
