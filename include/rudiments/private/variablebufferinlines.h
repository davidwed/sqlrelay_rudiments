// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE variablebuffer::~variablebuffer() {
	delete[] buffer;
}

RUDIMENTS_INLINE unsigned char *variablebuffer::getBuffer() {
	return buffer;
}

RUDIMENTS_INLINE size_t variablebuffer::getSize() {
	return endofbuffer;
}

RUDIMENTS_INLINE size_t variablebuffer::getPosition() {
	return position;
}

RUDIMENTS_INLINE size_t variablebuffer::getEnd() {
	return endofbuffer;
}

RUDIMENTS_INLINE size_t variablebuffer::getActualSize() {
	return buffersize;
}

RUDIMENTS_INLINE void variablebuffer::setPosition(size_t pos) {
	position=pos;
}

RUDIMENTS_INLINE variablebuffer *variablebuffer::append(const
							unsigned char *data,
								size_t size) {
	position=endofbuffer;
	return write(data,size);
}
