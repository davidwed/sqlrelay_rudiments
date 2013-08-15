// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS

#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

#include <rudiments/private/rudimentsinlines.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
int32_t linkedlistutil<datatype>::compare(datatype data1,
					datatype data2) const {
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
	stdoutput.printf("%s",data);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<const char *>::print(const char *data) const {
	stdoutput.printf("%s",data);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
int32_t linkedlistutil<char *>::compare(char *data1, char *data2) const {
	return charstring::compare(data1,data2);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
int32_t linkedlistutil<const char *>::compare(const char *data1,
						const char *data2) const {
	return charstring::compare(data1,data2);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<char>::print(char data) const {
	stdoutput.printf("%c",data);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<int32_t>::print(int32_t data) const {
	stdoutput.printf("%d",(int)data);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<int16_t>::print(int16_t data) const {
	stdoutput.printf("%hd",data);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<int64_t>::print(int64_t data) const {
	stdoutput.printf("%lld",(long long)data);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<float>::print(float data) const {
	stdoutput.printf("%f",data);
}

RUDIMENTS_EXPLICIT_SPECIALIZATION
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<double>::print(double data) const {
	stdoutput.printf("%f",data);
}

template <class datatype>
RUDIMENTS_TEMPLATE_INLINE
void linkedlistutil<datatype>::print(datatype data) const {
	stdoutput.printf("%p",data);
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
