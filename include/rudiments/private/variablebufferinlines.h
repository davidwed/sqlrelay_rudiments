// Copyright (c) 2002 David Muse
// See the COPYING file for more information

inline variablebuffer::~variablebuffer() {
	delete[] buffer;
}

inline unsigned char *variablebuffer::getBuffer() {
	return buffer;
}

inline size_t variablebuffer::getSize() {
	return endofbuffer;
}

inline size_t variablebuffer::getPosition() {
	return position;
}

inline size_t variablebuffer::getEnd() {
	return endofbuffer;
}

inline size_t variablebuffer::getActualSize() {
	return buffersize;
}

inline void variablebuffer::setPosition(size_t pos) {
	position=pos;
}

inline variablebuffer *variablebuffer::append(const unsigned char *data,
								size_t size) {
	position=endofbuffer;
	return write(data,size);
}
