// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

#include <rudiments/text.h>

inline stringbuffer::stringbuffer() : variablebuffer(128,32) {
}

inline stringbuffer::stringbuffer(char *initialcontents,
					size_t initialsize, size_t increment) :
			variablebuffer((unsigned char *)initialcontents,
						initialsize,increment) {
}

inline stringbuffer::~stringbuffer() {
}

inline void stringbuffer::setPosition(size_t pos) {
	variablebuffer::setPosition(pos);
}

inline char *stringbuffer::getString() {
	terminate();
	return (char *)getBuffer();
}

inline size_t stringbuffer::getStringLength() {
	return strlen(getString());
}

inline size_t stringbuffer::getPosition() {
	return variablebuffer::getPosition();
}

inline void stringbuffer::clear() {
	variablebuffer::clear();
}

inline void stringbuffer::terminate() {
	if (buffer[endofbuffer]!='\0') {
		variablebuffer::append((unsigned char *)"\0",1);
		endofbuffer--;
		position--;
	}
}

inline stringbuffer *stringbuffer::append(const char *string) {
	variablebuffer::append((unsigned char *)string,strlen(string));
	return this;
}

inline stringbuffer *stringbuffer::append(const char *string, size_t size) {
	variablebuffer::append((unsigned char *)string,size);
	return this;
}

inline stringbuffer *stringbuffer::append(char character) {
	variablebuffer::append((unsigned char *)&character,sizeof(character));
	return this;
}

inline stringbuffer *stringbuffer::append(long number) {
	char	*numstr=text::parseNumber(number);
	variablebuffer::append((unsigned char *)numstr,strlen(numstr));
	return this;
}

inline stringbuffer *stringbuffer::append(double number) {
	return append(number,4);
}

inline stringbuffer *stringbuffer::append(double number, unsigned short scale) {
	char	*numstr=text::parseNumber(number,scale);
	variablebuffer::append((unsigned char *)numstr,strlen(numstr));
	return this;
}

inline stringbuffer *stringbuffer::append(double number,
			unsigned short precision, unsigned short scale) {
	char	*numstr=text::parseNumber(number,precision,scale);
	variablebuffer::append((unsigned char *)numstr,strlen(numstr));
	return this;
}

inline stringbuffer *stringbuffer::write(const char *string) {
	variablebuffer::write((unsigned char *)string,strlen(string));
	return this;
}

inline stringbuffer *stringbuffer::write(const char *string, size_t size) {
	variablebuffer::write((unsigned char *)string,size);
	return this;
}

inline stringbuffer *stringbuffer::write(char character) {
	variablebuffer::write((unsigned char *)&character,sizeof(character));
	return this;
}

inline stringbuffer *stringbuffer::write(long number) {
	char	*numstr=text::parseNumber(number);
	variablebuffer::write((unsigned char *)numstr,strlen(numstr));
	return this;
}

inline stringbuffer *stringbuffer::write(double number) {
	return write(number,4);
}

inline stringbuffer *stringbuffer::write(double number, unsigned short scale) {
	char	*numstr=text::parseNumber(number,scale);
	variablebuffer::write((unsigned char *)numstr,strlen(numstr));
	return this;
}

inline stringbuffer *stringbuffer::write(double number,
			unsigned short precision, unsigned short scale) {
	char	*numstr=text::parseNumber(number,precision,scale);
	variablebuffer::write((unsigned char *)numstr,strlen(numstr));
	return this;
}
