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
	end=0;
}

unsigned long variablebuffer::read(unsigned char *data, unsigned long size) {

	unsigned long	bytestoread=size;
	if (position>end) {
		bytestoread=0;
	} else if (position+size>end) {
		bytestoread=end-position;
	}

	memcpy((void *)data,(void *)(buffer+position),bytestoread);
	position=position+bytestoread;

	return bytestoread;
}

variablebuffer *variablebuffer::write(const unsigned char *data,
						unsigned long size) {

	// if the buffer is too small, extend it
	if (position>buffersize) {
		extend(position-buffersize+size);
	} else if (size>buffersize-position) {
		extend(position+size-buffersize);
	}

	// copy the data into the buffer
	memcpy((void *)(buffer+position),(void *)data,size);

	// increment the position indices
	position=position+size;
	if (position>end) {
		end=position;
	}
	return this;
}

void variablebuffer::clear() {
	delete[] buffer;
	buffer=new unsigned char[initialsize];
	buffersize=initialsize;
	position=0;
	end=0;
}

void variablebuffer::extend(unsigned long size) {
	unsigned long	newbuffersize=buffersize+
					((size/increment)*increment)+
					(((size%increment)>0)*increment);
	unsigned char	*newbuffer=new unsigned char[newbuffersize];
	memcpy((void *)newbuffer,(void *)buffer,buffersize);
	delete[] buffer;
	buffer=newbuffer;
	buffersize=newbuffersize;
}
