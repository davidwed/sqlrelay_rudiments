// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/variablebuffer.h>

#include <stdio.h>

// need these for memcpy...
#include <string.h>
#ifdef HAVE_STRINGS
	#include <strings.h>
#endif

variablebuffer::variablebuffer(size_t initialsize, size_t increment) {
	init(new unsigned char[initialsize],initialsize,increment);
}

variablebuffer::variablebuffer(unsigned char *initialcontents,
				size_t initialsize, size_t increment) {
	init(initialcontents,initialsize,increment);
}

void variablebuffer::init(unsigned char *initialcontents,
				size_t initialsize, size_t increment) {
	buffer=initialcontents;
	buffersize=initialsize;
	this->initialsize=initialsize;
	this->increment=increment;
	position=0;
	endofbuffer=0;
}

variablebuffer::~variablebuffer() {
	delete[] buffer;
}

ssize_t variablebuffer::read(unsigned char *data, size_t size) {

	size_t	bytestoread=size;
	if (position>endofbuffer) {
		bytestoread=0;
	} else if (position+size>endofbuffer) {
		bytestoread=endofbuffer-position;
	}

	memcpy((void *)data,(void *)(buffer+position),bytestoread);
	position=position+bytestoread;

	return bytestoread;
}

variablebuffer *variablebuffer::write(const unsigned char *data, size_t size) {

	// if the buffer is too small, extend it
	if (position>=buffersize) {
		extend(position-buffersize+size);
	} else if (size>=buffersize-position) {
		extend(buffersize-position+size);
	}

	// copy the data into the buffer
	memcpy((void *)(buffer+position),(void *)data,size);

	// increment the position indices
	position=position+size;
	if (position>endofbuffer) {
		endofbuffer=position;
	}
	return this;
}

void variablebuffer::clear() {
	delete[] buffer;
	buffer=new unsigned char[initialsize];
	buffersize=initialsize;
	position=0;
	endofbuffer=0;
}

void variablebuffer::extend(size_t size) {
	size_t	newbuffersize=buffersize+((size/increment)*increment)+
					(((size%increment)>0)*increment);
	unsigned char	*newbuffer=new unsigned char[newbuffersize];
	memcpy((void *)newbuffer,(void *)buffer,buffersize);
	delete[] buffer;
	buffer=newbuffer;
	buffersize=newbuffersize;
}

unsigned char *variablebuffer::getBuffer() {
	return buffer;
}

size_t variablebuffer::getSize() {
	return endofbuffer;
}

size_t variablebuffer::getPosition() {
	return position;
}

size_t variablebuffer::getEnd() {
	return endofbuffer;
}

size_t variablebuffer::getActualSize() {
	return buffersize;
}

void variablebuffer::setPosition(size_t pos) {
	position=pos;
}

variablebuffer *variablebuffer::append(const unsigned char *data, size_t size) {
	position=endofbuffer;
	return write(data,size);
}
