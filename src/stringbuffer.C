// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/stringbuffer.h>
#ifndef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/stringbufferinlines.h>
#endif

#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

size_t stringbuffer::getStringLength() {
	return strlen(getString());
}

stringbuffer *stringbuffer::append(const char *string) {
	if (string) {
		variablebuffer::append((unsigned char *)string,strlen(string));
	}
	return this;
}

stringbuffer *stringbuffer::append(long number) {
	char	*numstr=string::parseNumber(number);
	variablebuffer::append((unsigned char *)numstr,strlen(numstr));
	return this;
}

stringbuffer *stringbuffer::append(double number, unsigned short scale) {
	char	*numstr=string::parseNumber(number,scale);
	variablebuffer::append((unsigned char *)numstr,strlen(numstr));
	return this;
}

stringbuffer *stringbuffer::append(double number, unsigned short precision,
							unsigned short scale) {
	char	*numstr=string::parseNumber(number,precision,scale);
	variablebuffer::append((unsigned char *)numstr,strlen(numstr));
	return this;
}

stringbuffer *stringbuffer::write(const char *string) {
	variablebuffer::write((unsigned char *)string,strlen(string));
	return this;
}

stringbuffer *stringbuffer::write(long number) {
	char	*numstr=string::parseNumber(number);
	variablebuffer::write((unsigned char *)numstr,strlen(numstr));
	return this;
}

stringbuffer *stringbuffer::write(double number, unsigned short scale) {
	char	*numstr=string::parseNumber(number,scale);
	variablebuffer::write((unsigned char *)numstr,strlen(numstr));
	return this;
}

stringbuffer *stringbuffer::write(double number, unsigned short precision,
							unsigned short scale) {
	char	*numstr=string::parseNumber(number,precision,scale);
	variablebuffer::write((unsigned char *)numstr,strlen(numstr));
	return this;
}
