// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#include <rudiments/charstring.h>

#include <stdio.h>

#include <rudiments/private/rudimentsinlines.h>

template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
int linkedlistutil<datatype>::
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
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<char *>::print(char *data) const {
	printf("%s",data);
}

template <>
RUDIMENTS_TEMPLATE_INLINE
int linkedlistutil<char *>::compare(char *data1, char *data2) const {
	return charstring::compare(data1,data2);
}

template <>
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<char>::print(char data) const {
	printf("%c",data);
}

template <>
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<int>::print(int data) const {
	printf("%d",data);
}

template <>
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<short>::print(short data) const {
	printf("%hd",data);
}

template <>
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<long>::print(long data) const {
	printf("%ld",data);
}

template <>
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<float>::print(float data) const {
	printf("%f",data);
}

template <>
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<double>::print(double data) const {
	printf("%f",data);
}

template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<datatype>::print(datatype data) const {
	printf("%lx",(unsigned long)data);
}
