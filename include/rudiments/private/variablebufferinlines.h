// Copyright (c) 2002 David Muse
// See the COPYING file for more information

inline variablebuffer::~variablebuffer() {
	delete[] buffer;
}

inline unsigned char *variablebuffer::getBuffer() {
	return buffer;
}

inline unsigned long variablebuffer::getSize() {
	return end;
}

inline unsigned long variablebuffer::getPosition() {
	return position;
}

inline void variablebuffer::setPosition(unsigned long pos) {
	position=pos;
}

inline variablebuffer *variablebuffer::append(const unsigned char *data,
						unsigned long size) {
	position=end;
	return write(data,size);
}
