// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/variablebuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>

#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
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

variablebuffer::variablebuffer(const variablebuffer &v) {
	variablebufferClone(v);
}

variablebuffer &variablebuffer::operator=(const variablebuffer &v) {
	if (this!=&v) {
		delete[] buffer;
		variablebufferClone(v);
	}
	return *this;
}

variablebuffer::~variablebuffer() {
	delete[] buffer;
}

void variablebuffer::variablebufferClone(const variablebuffer &v) {
	initialsize=v.initialsize;
	increment=v.increment;
	buffersize=v.buffersize;
	position=v.position;
	endofbuffer=v.endofbuffer;
	buffer=new unsigned char[v.buffersize];
	rawbuffer::copy(buffer,v.buffer,buffersize);
}

ssize_t variablebuffer::read(unsigned char *data, size_t size) {

	size_t	bytestoread=size;
	if (position>endofbuffer) {
		bytestoread=0;
	} else if (position+size>endofbuffer) {
		bytestoread=endofbuffer-position;
	}

	rawbuffer::copy(static_cast<void *>(data),
			static_cast<const void *>(buffer+position),
			bytestoread);
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
	rawbuffer::copy(static_cast<void *>(buffer+position),
			static_cast<const void *>(data),
			size);

	// increment the position indices
	position=position+size;
	if (position>endofbuffer) {
		endofbuffer=position;
	}
	return this;
}

variablebuffer *variablebuffer::write(const char *string) {
	return write(reinterpret_cast<const unsigned char *>(string),
					charstring::length(string));
}

variablebuffer *variablebuffer::write(const char *string, size_t size) {
	return write(reinterpret_cast<const unsigned char *>(string),size);
}

variablebuffer *variablebuffer::write(char character) {
	return write(reinterpret_cast<const unsigned char *>(&character),
								sizeof(char));
}

variablebuffer *variablebuffer::write(short number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
								sizeof(short));
}

variablebuffer *variablebuffer::write(long number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
								sizeof(long));
}

variablebuffer *variablebuffer::write(long long number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(long long));
}

variablebuffer *variablebuffer::write(unsigned char character) {
	return write(reinterpret_cast<const unsigned char *>(&character),
							sizeof(unsigned char));
}

variablebuffer *variablebuffer::write(unsigned short number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(unsigned short));
}

variablebuffer *variablebuffer::write(unsigned long number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(unsigned long));
}

variablebuffer *variablebuffer::write(unsigned long long number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
						sizeof(unsigned long long));
}

variablebuffer *variablebuffer::write(float number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
								sizeof(float));
}

variablebuffer *variablebuffer::write(double number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
								sizeof(double));
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
	rawbuffer::copy(static_cast<void *>(newbuffer),
			static_cast<const void *>(buffer),
			buffersize);
	delete[] buffer;
	buffer=newbuffer;
	buffersize=newbuffersize;
}

unsigned char *variablebuffer::getBuffer() {
	return buffer;
}

unsigned char *variablebuffer::detachBuffer() {
	unsigned char	*retval=buffer;
	buffer=new unsigned char[initialsize];
	buffersize=initialsize;
	position=0;
	endofbuffer=0;
	return retval;
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

variablebuffer *variablebuffer::append(const char *string) {
	return append(reinterpret_cast<const unsigned char *>(string),
						charstring::length(string));
}

variablebuffer *variablebuffer::append(const char *string, size_t size) {
	return append(reinterpret_cast<const unsigned char *>(string),size);
}

variablebuffer *variablebuffer::append(char character) {
	return append(reinterpret_cast<const unsigned char *>(&character),
								sizeof(char));
}

variablebuffer *variablebuffer::append(short number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
								sizeof(short));
}

variablebuffer *variablebuffer::append(long number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
								sizeof(long));
}

variablebuffer *variablebuffer::append(long long number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(long long));
}

variablebuffer *variablebuffer::append(unsigned char character) {
	return append(reinterpret_cast<const unsigned char *>(&character),
							sizeof(unsigned char));
}

variablebuffer *variablebuffer::append(unsigned short number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(unsigned short));
}

variablebuffer *variablebuffer::append(unsigned long number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(unsigned long));
}

variablebuffer *variablebuffer::append(unsigned long long number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
						sizeof(unsigned long long));
}

variablebuffer *variablebuffer::append(float number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
								sizeof(float));
}

variablebuffer *variablebuffer::append(double number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
								sizeof(double));
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
