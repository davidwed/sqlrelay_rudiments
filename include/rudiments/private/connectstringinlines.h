// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE connectstring::connectstring() {
}

RUDIMENTS_INLINE int connectstring::parseNameLength(const char *data) {
	return parsePartLength(data,'=',0,0);
}

RUDIMENTS_INLINE char *connectstring::parseName(const char *data,
							char **outbuffer) {
	return parsePart(parseNameLength(data),'=',data,outbuffer,0,0);
}

RUDIMENTS_INLINE int connectstring::parseValueLength(const char *data) {
	return parsePartLength(data,';',1,1);
}

RUDIMENTS_INLINE char *connectstring::parseValue(const char *data,
							char **outbuffer) {
	return parsePart(parseValueLength(data),';',data,outbuffer,1,1);
}

RUDIMENTS_INLINE char *connectstring::getValue(const char *name) {
	char	*retval;
	return (nvp.getData((char *)name,&retval))?retval:NULL;
}
