// Copyright (c) 2002 David Muse
// See the COPYING file for more information

variablebuffer::~variablebuffer() {
	delete[] buffer;
}

unsigned char	*variablebuffer::getBuffer() {
	return buffer;
}

unsigned long	variablebuffer::getSize() {
	return position;
}
