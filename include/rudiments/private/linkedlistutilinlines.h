// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS

#include <rudiments/charstring.h>

#include <stdio.h>

#include <rudiments/private/rudimentsinlines.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
int linkedlistutil<datatype>::compare(datatype data1, datatype data2) const {
	if (data1<data2) {
		return -1;
	} else if (data1==data2) {
		return 0;
	} else {
		return 1;
	}
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<char *>::print(char *data) const {
	printf("%s",data);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<const char *>::print(const char *data) const {
	printf("%s",data);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
int linkedlistutil<char *>::compare(char *data1, char *data2) const {
	return charstring::compare(data1,data2);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
int linkedlistutil<const char *>::compare(const char *data1,
						const char *data2) const {
	return charstring::compare(data1,data2);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<char>::print(char data) const {
	printf("%c",data);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<int>::print(int data) const {
	printf("%d",data);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<short>::print(short data) const {
	printf("%hd",data);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<long>::print(long data) const {
	printf("%ld",data);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<float>::print(float data) const {
	printf("%f",data);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<double>::print(double data) const {
	printf("%f",data);
}

template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<datatype>::print(datatype data) const {
	printf("%lx",(unsigned long)data);
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
