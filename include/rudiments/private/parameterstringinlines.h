// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE parameterstring::parameterstring() {
}

RUDIMENTS_INLINE int parameterstring::parseNameLength(const char *data) {
	return parsePartLength(data,'=',0,0);
}

RUDIMENTS_INLINE char *parameterstring::parseName(const char *data,
							char **outbuffer) {
	return parsePart(parseNameLength(data),'=',data,outbuffer,0,0);
}

RUDIMENTS_INLINE int parameterstring::parseValueLength(const char *data) {
	return parsePartLength(data,';',1,1);
}

RUDIMENTS_INLINE char *parameterstring::parseValue(const char *data,
							char **outbuffer) {
	return parsePart(parseValueLength(data),';',data,outbuffer,1,1);
}

RUDIMENTS_INLINE char *parameterstring::getValue(const char *name) {
	char	*retval;
	return (nvp.getData((char *)name,&retval))?retval:NULL;
}
