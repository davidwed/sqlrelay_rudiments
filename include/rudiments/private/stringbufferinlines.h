// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE stringbuffer::stringbuffer() : variablebuffer(128,32) {
}

RUDIMENTS_INLINE stringbuffer::stringbuffer(char *initialcontents,
					size_t initialsize, size_t increment) :
			variablebuffer((unsigned char *)initialcontents,
						initialsize,increment) {
}

RUDIMENTS_INLINE stringbuffer::~stringbuffer() {
}

RUDIMENTS_INLINE void stringbuffer::setPosition(size_t pos) {
	variablebuffer::setPosition(pos);
}

RUDIMENTS_INLINE char *stringbuffer::getString() {
	terminate();
	return (char *)getBuffer();
}

RUDIMENTS_INLINE size_t stringbuffer::getPosition() {
	return variablebuffer::getPosition();
}

RUDIMENTS_INLINE void stringbuffer::clear() {
	variablebuffer::clear();
}

RUDIMENTS_INLINE void stringbuffer::terminate() {
	if (buffer[endofbuffer]!='\0') {
		variablebuffer::append((unsigned char *)"\0",1);
		endofbuffer--;
		position--;
	}
}

RUDIMENTS_INLINE stringbuffer *stringbuffer::append(const char *string,
								size_t size) {
	variablebuffer::append((unsigned char *)string,size);
	return this;
}

RUDIMENTS_INLINE stringbuffer *stringbuffer::append(char character) {
	variablebuffer::append((unsigned char *)&character,sizeof(character));
	return this;
}

RUDIMENTS_INLINE stringbuffer *stringbuffer::append(double number) {
	return append(number,4);
}

RUDIMENTS_INLINE stringbuffer *stringbuffer::write(const char *string,
								size_t size) {
	variablebuffer::write((unsigned char *)string,size);
	return this;
}

RUDIMENTS_INLINE stringbuffer *stringbuffer::write(char character) {
	variablebuffer::write((unsigned char *)&character,sizeof(character));
	return this;
}

RUDIMENTS_INLINE stringbuffer *stringbuffer::write(double number) {
	return write(number,4);
}
