// Copyright (c) 2002 David Muse
// See the COPYING file for more information

INLINE variablebuffer::~variablebuffer() {
	delete[] buffer;
}

INLINE unsigned char *variablebuffer::getBuffer() {
	return buffer;
}

INLINE size_t variablebuffer::getSize() {
	return endofbuffer;
}

INLINE size_t variablebuffer::getPosition() {
	return position;
}

INLINE size_t variablebuffer::getEnd() {
	return endofbuffer;
}

INLINE size_t variablebuffer::getActualSize() {
	return buffersize;
}

INLINE void variablebuffer::setPosition(size_t pos) {
	position=pos;
}

INLINE variablebuffer *variablebuffer::append(const unsigned char *data,
								size_t size) {
	position=endofbuffer;
	return write(data,size);
}
