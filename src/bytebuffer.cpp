// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/bytebuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

class bytebufferprivate {
	friend class bytebuffer;
	private:
		unsigned char	*_buffer;
		size_t		_buffersize;
		size_t		_initialsize;
		size_t		_increment;
		size_t		_position;
		size_t		_endofbuffer;
};

bytebuffer::bytebuffer(size_t initialsize, size_t increment) {
	init(NULL,initialsize,increment);
}

bytebuffer::bytebuffer(unsigned char *initialcontents,
				size_t initialsize, size_t increment) {
	init(initialcontents,initialsize,increment);
}

void bytebuffer::init(unsigned char *initialcontents,
				size_t initialsize, size_t increment) {
	pvt=new bytebufferprivate;
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

bytebuffer::bytebuffer(const bytebuffer &v) {
	bytebufferClone(v);
}

bytebuffer &bytebuffer::operator=(const bytebuffer &v) {
	if (this!=&v) {
		delete[] pvt->_buffer;
		delete pvt;
		bytebufferClone(v);
	}
	return *this;
}

bytebuffer::~bytebuffer() {
	delete[] pvt->_buffer;
	delete pvt;
}

void bytebuffer::bytebufferClone(const bytebuffer &v) {
	pvt=new bytebufferprivate;
	pvt->_initialsize=v.pvt->_initialsize;
	pvt->_increment=v.pvt->_increment;
	pvt->_buffersize=v.pvt->_buffersize;
	pvt->_position=v.pvt->_position;
	pvt->_endofbuffer=v.pvt->_endofbuffer;
	pvt->_buffer=new unsigned char[v.pvt->_buffersize];
	rawbuffer::copy(pvt->_buffer,v.pvt->_buffer,pvt->_buffersize);
}

ssize_t bytebuffer::read(unsigned char *data, size_t size) {

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

bytebuffer *bytebuffer::write(const unsigned char *data, size_t size) {

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

bytebuffer *bytebuffer::write(const char *string) {
	return write(reinterpret_cast<const unsigned char *>(string),
					charstring::length(string));
}

bytebuffer *bytebuffer::write(const char *string, size_t size) {
	return write(reinterpret_cast<const unsigned char *>(string),size);
}

bytebuffer *bytebuffer::write(char character) {
	return write(reinterpret_cast<const unsigned char *>(&character),
								sizeof(char));
}

bytebuffer *bytebuffer::write(int16_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int16_t));
}

bytebuffer *bytebuffer::write(int32_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int32_t));
}

bytebuffer *bytebuffer::write(int64_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int64_t));
}

bytebuffer *bytebuffer::write(unsigned char character) {
	return write(reinterpret_cast<const unsigned char *>(&character),
							sizeof(unsigned char));
}

bytebuffer *bytebuffer::write(uint16_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint16_t));
}

bytebuffer *bytebuffer::write(uint32_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint32_t));
}

bytebuffer *bytebuffer::write(uint64_t number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint64_t));
}

bytebuffer *bytebuffer::write(float number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
								sizeof(float));
}

bytebuffer *bytebuffer::write(double number) {
	return write(reinterpret_cast<const unsigned char *>(&number),
								sizeof(double));
}

bytebuffer *bytebuffer::writeFormatted(const char *format, ...) {
	va_list	argp;
	va_start(argp,format);
	bytebuffer	*retval=writeFormatted(format,&argp);
	va_end(argp);
	return retval;
}

bytebuffer *bytebuffer::writeFormatted(const char *format,
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

void bytebuffer::clear() {
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

void bytebuffer::extend(size_t size) {
	size_t	newbuffersize=pvt->_buffersize+
				((size/pvt->_increment)*pvt->_increment)+
				(((size%pvt->_increment)>0)*pvt->_increment);
	unsigned char	*newbuffer=new unsigned char[newbuffersize];
	rawbuffer::copy(newbuffer,pvt->_buffer,pvt->_buffersize);
	delete[] pvt->_buffer;
	pvt->_buffer=newbuffer;
	pvt->_buffersize=newbuffersize;
}

const unsigned char *bytebuffer::getBuffer() {
	return pvt->_buffer;
}

unsigned char *bytebuffer::detachBuffer() {
	unsigned char	*retval=pvt->_buffer;
	pvt->_buffer=new unsigned char[pvt->_initialsize];
	pvt->_buffersize=pvt->_initialsize;
	pvt->_position=0;
	pvt->_endofbuffer=0;
	return retval;
}

size_t bytebuffer::getSize() {
	return pvt->_endofbuffer;
}

size_t bytebuffer::getPosition() {
	return pvt->_position;
}

size_t bytebuffer::getEnd() {
	return pvt->_endofbuffer;
}

size_t bytebuffer::getActualSize() {
	return pvt->_buffersize;
}

void bytebuffer::setPosition(size_t pos) {
	pvt->_position=pos;
}

bytebuffer *bytebuffer::append(const unsigned char *data, size_t size) {
	pvt->_position=pvt->_endofbuffer;
	return write(data,size);
}

bytebuffer *bytebuffer::append(const char *string) {
	return append(reinterpret_cast<const unsigned char *>(string),
						charstring::length(string));
}

bytebuffer *bytebuffer::append(const char *string, size_t size) {
	return append(reinterpret_cast<const unsigned char *>(string),size);
}

bytebuffer *bytebuffer::append(char character) {
	return append(reinterpret_cast<const unsigned char *>(&character),
								sizeof(char));
}

bytebuffer *bytebuffer::append(int16_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int16_t));
}

bytebuffer *bytebuffer::append(int32_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int32_t));
}

bytebuffer *bytebuffer::append(int64_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(int64_t));
}

bytebuffer *bytebuffer::append(unsigned char character) {
	return append(reinterpret_cast<const unsigned char *>(&character),
							sizeof(unsigned char));
}

bytebuffer *bytebuffer::append(uint16_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint16_t));
}

bytebuffer *bytebuffer::append(uint32_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint32_t));
}

bytebuffer *bytebuffer::append(uint64_t number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
							sizeof(uint64_t));
}

bytebuffer *bytebuffer::append(float number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
								sizeof(float));
}

bytebuffer *bytebuffer::append(double number) {
	return append(reinterpret_cast<const unsigned char *>(&number),
								sizeof(double));
}

bytebuffer *bytebuffer::appendFormatted(const char *format, ...) {
	va_list	argp;
	va_start(argp,format);
	bytebuffer	*retval=appendFormatted(format,&argp);
	va_end(argp);
	return retval;
}

bytebuffer *bytebuffer::appendFormatted(const char *format,
							va_list *argp) {
	pvt->_position=pvt->_endofbuffer;
	return writeFormatted(format,argp);
}

void bytebuffer::truncate(size_t pos) {
	pvt->_endofbuffer=pos;
}

void bytebuffer::truncate() {
	pvt->_endofbuffer=pvt->_position;
}

unsigned char *bytebuffer::_buffer() {
	return pvt->_buffer;
}

size_t bytebuffer::_endofbuffer() {
	return pvt->_endofbuffer;
}

void bytebuffer::_endofbuffer(size_t eob) {
	pvt->_endofbuffer=eob;
}

size_t bytebuffer::_position() {
	return pvt->_position;
}

void bytebuffer::_position(size_t pos) {
	pvt->_position=pos;
}
