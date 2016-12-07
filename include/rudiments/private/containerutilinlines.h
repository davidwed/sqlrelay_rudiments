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
int32_t _containerutil_compare(char *value1, char *value2) {
	return charstring::compare(value1,value2);
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _containerutil_compare(const char *value1, const char *value2) {
	return charstring::compare(value1,value2);
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _containerutil_compare(const unsigned char *value1,
				const unsigned char *value2) {
	return charstring::compare((const char *)value1,(const char *)value2);
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _containerutil_compare(unsigned char *value1, unsigned char *value2) {
	return charstring::compare((const char *)value1,(const char *)value2);
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _containerutil_compare(char value1, char value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _containerutil_compare(int16_t value1, int16_t value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _containerutil_compare(int32_t value1, int32_t value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _containerutil_compare(int64_t value1, int64_t value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _containerutil_compare(unsigned char value1, unsigned char value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _containerutil_compare(uint16_t value1, uint16_t value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _containerutil_compare(uint32_t value1, uint32_t value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _containerutil_compare(uint64_t value1, uint64_t value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _containerutil_compare(float value1, float value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _containerutil_compare(double value1, double value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _containerutil_compare(long double value1, long double value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

RUDIMENTS_TEMPLATE_INLINE
int32_t _containerutil_compare(void *value1, void *value2) {
	if (value1<value2) {
		return -1;
	} else if (value1==value2) {
		return 0;
	} else {
		return 1;
	}
}

/*template <class valuetype>
RUDIMENTS_TEMPLATE_INLINE
int32_t _containerutil_compare(valuetype value1, valuetype value2) {
	if (!value1 && value2) {
		return 1;
	} else if (!value1 && !value2) {
		return 0;
	} else if (value1 && !value2) {
		return -1;
	} else {
		return value1->compare(value2);
	}
}*/

RUDIMENTS_TEMPLATE_INLINE
void _containerutil_print(const char *value) {
	stdoutput.printf("%s",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _containerutil_print(char *value) {
	stdoutput.printf("%s",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _containerutil_print(char value) {
	stdoutput.printf("%c",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _containerutil_print(int16_t value) {
	stdoutput.printf("%hd",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _containerutil_print(int32_t value) {
	stdoutput.printf("%d",(int)value);
}

RUDIMENTS_TEMPLATE_INLINE
void _containerutil_print(int64_t value) {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		stdoutput.printf("%lld",(long long)value);
	#else
		stdoutput.printf("%ld",(long)value);
	#endif
}

RUDIMENTS_TEMPLATE_INLINE
void _containerutil_print(unsigned const char *value) {
	stdoutput.printf("%s",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _containerutil_print(unsigned char *value) {
	stdoutput.printf("%s",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _containerutil_print(unsigned char value) {
	stdoutput.printf("%c",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _containerutil_print(uint16_t value) {
	stdoutput.printf("%hd",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _containerutil_print(uint32_t value) {
	stdoutput.printf("%d",(unsigned int)value);
}

RUDIMENTS_TEMPLATE_INLINE
void _containerutil_print(uint64_t value) {
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		stdoutput.printf("%lld",(unsigned long long)value);
	#else
		stdoutput.printf("%ld",(unsigned long)value);
	#endif
}

RUDIMENTS_TEMPLATE_INLINE
void _containerutil_print(float value) {
	stdoutput.printf("%f",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _containerutil_print(double value) {
	stdoutput.printf("%f",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _containerutil_print(long double value) {
	stdoutput.printf("%Lf",value);
}

RUDIMENTS_TEMPLATE_INLINE
void _containerutil_print(void *value) {
	stdoutput.printf("%08x",value);
}

/*template <class valuetype>
RUDIMENTS_TEMPLATE_INLINE
void _containerutil_print(valuetype value) {
	if (value) {
		value->print();
	} else {
		stdoutput.printf("(null)");
	}
}*/

#endif
