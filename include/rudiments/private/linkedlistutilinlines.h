// Copyright (c) 2003 David Muse
// See the COPYING file for more information
#ifndef RUDIMENTS_LINKEDLISTUTIL_H
#define RUDIMENTS_LINKEDLISTUTIL_H

#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include <rudiments/private/rudimentsinlines.h>

// Ideally we'd use explicit specialization here but old enough
// compilers don't support it and this isn't any less efficient.

RUDIMENTS_TEMPLATE_INLINE
int32_t _linkedlistutil_compare(char *value1, char *value2) {
	return charstring::compare(value1,value2);
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _linkedlistutil_compare(const char *value1, const char *value2) {
	return charstring::compare(value1,value2);
}

template <class valuetype>
RUDIMENTS_TEMPLATE_INLINE
int32_t _linkedlistutil_compare(valuetype value1, valuetype value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(const char *value) {
	stdoutput.printf("%s",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(char *value) {
	stdoutput.printf("%s",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(char value) {
	stdoutput.printf("%c",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(int16_t value) {
	stdoutput.printf("%hd",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(int32_t value) {
	stdoutput.printf("%d",(int)value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(int64_t value) {
	stdoutput.printf("%lld",(long long)value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(unsigned const char *value) {
	stdoutput.printf("%s",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(unsigned char *value) {
	stdoutput.printf("%s",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(unsigned char value) {
	stdoutput.printf("%c",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(uint16_t value) {
	stdoutput.printf("%hd",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(uint32_t value) {
	stdoutput.printf("%d",(unsigned int)value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(uint64_t value) {
	stdoutput.printf("%lld",(unsigned long long)value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(float value) {
	stdoutput.printf("%f",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(double value) {
	stdoutput.printf("%f",value);
}

template <class valuetype>
RUDIMENTS_TEMPLATE_INLINE
void _linkedlistutil_print(valuetype value) {
	stdoutput.printf("%p",value);
}

#endif
