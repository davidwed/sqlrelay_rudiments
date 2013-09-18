// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

#include <rudiments/private/rudimentsinlines.h>

// Ideally we'd use explicit specialization here but old enough
// compilers don't support it and this isn't any less efficient.

RUDIMENTS_TEMPLATE_INLINE
int32_t _linkedlistutil_compare(char *data1, char *data2) {
	return charstring::compare(data1,data2);
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _linkedlistutil_compare(const char *data1, const char *data2) {
	return charstring::compare(data1,data2);
}

template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
int32_t _linkedlistutil_compare(datatype data1, datatype data2) {
	if (data1<data2) {
		return -1;
	} else if (data1==data2) {
		return 0;
	} else {
		return 1;
	}
}

template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
int32_t linkedlistutil<datatype>::compare(datatype data1,
					datatype data2) const {
	return _linkedlistutil_compare(data1,data2);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(const char *data) {
	stdoutput.printf("%s",data);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(char data) {
	stdoutput.printf("%c",data);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(int32_t data) {
	stdoutput.printf("%d",(int)data);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(int16_t data) {
	stdoutput.printf("%hd",data);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(int64_t data) {
	stdoutput.printf("%lld",(long long)data);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(float data) {
	stdoutput.printf("%f",data);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(double data) {
	stdoutput.printf("%f",data);
}

template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(datatype data) {
	stdoutput.printf("%p",data);
}

template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<datatype>::print(datatype data) const {
	_linkedlistutil_print(data);
}
