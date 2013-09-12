// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/variablebuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>
#include <rudiments/null.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class variablebufferprivate {
	friend class variablebuffer;
	private:
		unsigned char	*_buffer;
		size_t		_buffersize;
		size_t		_initialsize;
		size_t		_increment;
		size_t		_position;
		size_t		_endofbuffer;
};

variablebuffer::variablebuffer(size_t initialsize, size_t increment) {
	init(NULL,initialsize,increment);
}

variablebuffer::variablebuffer(unsigned char *initialcontents,
				size_t initialsize, size_t increment) {
	init(initialcontents,initialsize,increment);
}

void variablebuffer::init(unsigned char *initialcontents,
				size_t initialsize, size_t increment) {
	pvt=new variablebufferprivate;
	if (initialcontents) {
		pvt->_buffer=initialcontents;
		pvt->_endofbuffer=initialsize;
	} else {
		pvt->_buffer=new unsigned char[initialsize];
		pvt->_endofbuffer=0;
	}
	pvt->_buffersize=initialsize;
	pvt->_initialsize=initialsize;
	pvt->_increment=increment;
	pvt->_position=0;
}

variablebuffer::variablebuffer(const variablebuffer &v) {
	variablebufferClone(v);
}

variablebuffer &variablebuffer::operator=(const variablebuffer &v) {
	if (this!=&v) {
		delete[] pvt->_buffer;
		delete pvt;
		variablebufferClone(v);
	}
	return *this;
}

variablebuffer::~variablebuffer() {
	delete[] pvt->_buffer;
	delete pvt;
}

void variablebuffer::variablebufferClone(const variablebuffer &v) {
	pvt=new variablebufferprivate;
	pvt->_initialsize=v.pvt->_initialsize;
	pvt->_increment=v.pvt->_increment;
	pvt->_buffersize=v.pvt->_buffersize;
	pvt->_position=v.pvt->_position;
	pvt->_endofbuffer=v.pvt->_endofbuffer;
	pvt->_buffer=new unsigned char[v.pvt->_buffersize];
	rawbuffer::copy(pvt->_buffer,v.pvt->_buffer,pvt->_buffersize);
}

ssize_t variablebuffer::read(unsigned char *data, size_t size) {

	size_t	bytestoread=size;
	if (pvt->_position>pvt->_endofbuffer) {
		bytestoread=0;
	} else if (pvt->_position+size>pvt->_endofbuffer) {
		bytestoread=pvt->_endofbuffer-pvt->_position;
	}

	rawbuffer::copy(data,pvt->_buffer+pvt->_position,bytestoread);
	pvt->_position=pvt->_position+bytestoread;

	return bytestoread;
}

variablebuffer *variablebuffer::write(const unsigned char *data, size_t size) {

	// if the buffer is too small, extend it
	if (pvt->_position>=pvt->_buffersize) {
		extend(pvt->_position-pvt->_buffersize+size);
	} else if (size>pvt->_buffersize-pvt->_position) {
		extend(size-(pvt->_buffersize-pvt->_position));
	}

	// copy the data into the buffer
	rawbuffer::copy(pvt->_buffer+pvt->_position,data,size);

	// increment the position indices
	pvt->_position=pvt->_position+size;
	if (pvt->_position>pvt->_endofbuffer) {
		pvt->_endofbuffer=pvt->_position;
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

variablebuffer *variablebuffer::write(int16_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int16_t));
}

variablebuffer *variablebuffer::write(int32_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int32_t));
}

variablebuffer *variablebuffer::write(int64_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int64_t));
}

variablebuffer *variablebuffer::write(unsigned char character) {
	return write(reinterpret_cast<const unsigned char *>(&character),
							sizeof(unsigned char));
}

variablebuffer *variablebuffer::write(uint16_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint16_t));
}

variablebuffer *variablebuffer::write(uint32_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint32_t));
}

variablebuffer *variablebuffer::write(uint64_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint64_t));
}

variablebuffer *variablebuffer::write(float number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
								sizeof(float));
}

variablebuffer *variablebuffer::write(double number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
								sizeof(double));
}

variablebuffer *variablebuffer::writeFormatted(const char *format, ...) {
	va_list	argp;
	va_start(argp,format);
	variablebuffer	*retval=writeFormatted(format,&argp);
	va_end(argp);
	return retval;
}

variablebuffer *variablebuffer::writeFormatted(const char *format,
							va_list *argp) {

	// find out how much space we need...
	//
	// Some compilers throw a warning if they see "printf(NULL..." at all,
	// independent of whether it's the global function printf() or one that
	// you've defined yourself.  This buffer=NULL thing works around it.
	char	*buffer=NULL;
	size_t	size=charstring::printf(buffer,0,format,argp);

	// extend the buffer if necessary
	if (pvt->_position>=pvt->_buffersize) {
		extend(pvt->_position-pvt->_buffersize+size);
	} else if (size>pvt->_buffersize-pvt->_position) {
		extend(size-(pvt->_buffersize-pvt->_position));
	}

	// copy the data into the buffer
	size=charstring::printf((char *)(pvt->_buffer+pvt->_position),
						pvt->_buffersize-pvt->_position,
						format,argp);

	// increment the position indices
	pvt->_position=pvt->_position+size;
	if (pvt->_position>pvt->_endofbuffer) {
		pvt->_endofbuffer=pvt->_position;
	}
	return this;
}

void variablebuffer::clear() {
	// If the buffer has grown beyond the initial size then reallocate it,
	// otherwise just reset the start and end indices.
	if (pvt->_buffersize>pvt->_initialsize) {
		delete[] pvt->_buffer;
		pvt->_buffer=new unsigned char[pvt->_initialsize];
		pvt->_buffersize=pvt->_initialsize;
	}
	pvt->_position=0;
	pvt->_endofbuffer=0;
}

void variablebuffer::extend(size_t size) {
	size_t	newbuffersize=pvt->_buffersize+
				((size/pvt->_increment)*pvt->_increment)+
				(((size%pvt->_increment)>0)*pvt->_increment);
	unsigned char	*newbuffer=new unsigned char[newbuffersize];
	rawbuffer::copy(newbuffer,pvt->_buffer,pvt->_buffersize);
	delete[] pvt->_buffer;
	pvt->_buffer=newbuffer;
	pvt->_buffersize=newbuffersize;
}

const unsigned char *variablebuffer::getBuffer() {
	return pvt->_buffer;
}

unsigned char *variablebuffer::detachBuffer() {
	unsigned char	*retval=pvt->_buffer;
	pvt->_buffer=new unsigned char[pvt->_initialsize];
	pvt->_buffersize=pvt->_initialsize;
	pvt->_position=0;
	pvt->_endofbuffer=0;
	return retval;
}

size_t variablebuffer::getSize() {
	return pvt->_endofbuffer;
}

size_t variablebuffer::getPosition() {
	return pvt->_position;
}

size_t variablebuffer::getEnd() {
	return pvt->_endofbuffer;
}

size_t variablebuffer::getActualSize() {
	return pvt->_buffersize;
}

void variablebuffer::setPosition(size_t pos) {
	pvt->_position=pos;
}

variablebuffer *variablebuffer::append(const unsigned char *data, size_t size) {
	pvt->_position=pvt->_endofbuffer;
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

variablebuffer *variablebuffer::append(int16_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int16_t));
}

variablebuffer *variablebuffer::append(int32_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int32_t));
}

variablebuffer *variablebuffer::append(int64_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int64_t));
}

variablebuffer *variablebuffer::append(unsigned char character) {
	return append(reinterpret_cast<const unsigned char *>(&character),
							sizeof(unsigned char));
}

variablebuffer *variablebuffer::append(uint16_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint16_t));
}

variablebuffer *variablebuffer::append(uint32_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint32_t));
}

variablebuffer *variablebuffer::append(uint64_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint64_t));
}

variablebuffer *variablebuffer::append(float number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
								sizeof(float));
}

variablebuffer *variablebuffer::append(double number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
								sizeof(double));
}

variablebuffer *variablebuffer::appendFormatted(const char *format, ...) {
	va_list	argp;
	va_start(argp,format);
	variablebuffer	*retval=appendFormatted(format,&argp);
	va_end(argp);
	return retval;
}

variablebuffer *variablebuffer::appendFormatted(const char *format,
							va_list *argp) {
	pvt->_position=pvt->_endofbuffer;
	return writeFormatted(format,argp);
}

void variablebuffer::truncate(size_t pos) {
	pvt->_endofbuffer=pos;
}

void variablebuffer::truncate() {
	pvt->_endofbuffer=pvt->_position;
}

unsigned char *variablebuffer::_buffer() {
	return pvt->_buffer;
}

size_t variablebuffer::_endofbuffer() {
	return pvt->_endofbuffer;
}

void variablebuffer::_endofbuffer(size_t eob) {
	pvt->_endofbuffer=eob;
}

size_t variablebuffer::_position() {
	return pvt->_position;
}

void variablebuffer::_position(size_t pos) {
	pvt->_position=pos;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
