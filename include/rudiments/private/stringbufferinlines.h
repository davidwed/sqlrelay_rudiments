// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

#include <rudiments/text.h>

inline stringbuffer::stringbuffer() {
	buffer=new variablebuffer(128,32);
}

inline stringbuffer::~stringbuffer() {
	delete buffer;
}

inline char *stringbuffer::getString() {
	buffer->append((unsigned char *)"\0",1);
	buffer->setPosition(buffer->getPosition()-1);
	return (char *)buffer->getBuffer();
}

inline size_t stringbuffer::getStringLength() {
	return strlen(getString());
}

inline void stringbuffer::clear() {
	buffer->clear();
}

inline stringbuffer *stringbuffer::append(const char *string) {
	buffer->append((unsigned char *)string,strlen(string));
	return this;
}

inline stringbuffer *stringbuffer::append(char character) {
	buffer->append((unsigned char *)&character,sizeof(character));
	return this;
}

inline stringbuffer *stringbuffer::append(long number) {
	char	*numstr=text::parseNumber(number);
	buffer->append((unsigned char *)numstr,strlen(numstr));
	return this;
}

inline stringbuffer *stringbuffer::append(double number) {
	return append(number,0,0);
}

inline stringbuffer *stringbuffer::append(double number,
			unsigned short precision, unsigned short scale) {
	char	*numstr=text::parseNumber(number,precision,scale);
	buffer->append((unsigned char *)numstr,strlen(numstr));
	return this;
}
