// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/variablebuffer.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/variablebufferinlines.h>
#endif
#include <string.h>

variablebuffer::variablebuffer(unsigned long initialsize,
					unsigned long increment) {
	buffer=new unsigned char[initialsize];
	buffersize=initialsize;
	this->initialsize=initialsize;
	this->increment=increment;
	position=0;
}

variablebuffer	*variablebuffer::append(const unsigned char *data,
						unsigned long size) {

	// if the buffer is too small, extend it
	if (size>buffersize-position) {
		buffersize=buffersize+size+increment;
		unsigned char	*newbuffer=new unsigned char[buffersize];
		memcpy((void *)newbuffer,(void *)buffer,position);
		delete[] buffer;
		buffer=newbuffer;
	}

	// copy the data into the buffer
	memcpy((void *)(buffer+position),(void *)data,size);
	position=position+size;
	return this;
}

void	variablebuffer::clear() {
	delete[] buffer;
	buffer=new unsigned char[initialsize];
	buffersize=initialsize;
	position=0;
}
