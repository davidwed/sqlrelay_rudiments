// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/bytebuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/linkedlist.h>
#include <rudiments/error.h>

class bytebufferextent {
	friend class bytebuffer;
	private:
		unsigned char	*_buffer;
		size_t		_size;
		size_t		_remaining;
};

class bytebufferprivate {
	friend class bytebuffer;
	private:
		linkedlist< bytebufferextent * >	_extents;
		linkedlistnode< bytebufferextent * >	*_curext;
		size_t		_initial;
		size_t		_extsize;
		size_t		_position;
		size_t		_end;
		size_t		_totalbytes;
};

bytebuffer::bytebuffer(size_t initial, size_t extsize) {
	init(NULL,initial,extsize);
}

bytebuffer::bytebuffer(unsigned char *initialcontents,
				size_t initial, size_t extsize) {
	init(initialcontents,initial,extsize);
}

void bytebuffer::init(unsigned char *initialcontents,
				size_t initial, size_t extsize) {
	pvt=new bytebufferprivate;
	pvt->_curext=NULL;
	pvt->_initial=initial;
	pvt->_extsize=extsize;
	pvt->_position=0;
	pvt->_end=0;
	pvt->_totalbytes=0;
	extend(initial);
}

bytebuffer::bytebuffer(const bytebuffer &v) {
	bytebufferClone(v);
}

bytebuffer &bytebuffer::operator=(const bytebuffer &v) {
	if (this!=&v) {
		delete pvt;
		bytebufferClone(v);
	}
	return *this;
}

bytebuffer::~bytebuffer() {
	clear();
	delete pvt;
}

void bytebuffer::bytebufferClone(const bytebuffer &v) {
	// FIXME: implement this
}

ssize_t bytebuffer::read(unsigned char *data, size_t size) {

	// bail if we're not actually reading anything
	if (!size) {
		return 0;
	}

	// extend the list of buffers to accommodate "size"
	// bytes beyond the current position (in case "size"
	// would put us past the current end)
	extend(size);

	// copy-out "size" bytes, updating the current extent and
	// number of bytes remaining in that extent as we go
	size_t	pos=0;
	while (size) {
		unsigned char	*buffer=pvt->_curext->getValue()->_buffer;
		size_t		extsize=pvt->_curext->getValue()->_size;
		size_t		remaining=pvt->_curext->getValue()->_remaining;
		size_t		bytestocopy=(remaining<size)?remaining:size;
		bytestring::copy(data+pos,buffer+extsize-remaining,bytestocopy);
		size=size-bytestocopy;
		if (size) {
			pvt->_curext=pvt->_curext->getNext();
			pvt->_curext->getValue()->_remaining=
				pvt->_curext->getValue()->_size;
		} else {
			pvt->_curext->getValue()->_remaining=
				pvt->_curext->getValue()->_remaining-
				bytestocopy;
		}
		pos=pos+bytestocopy;
	}

	// update the current position and end of buffer
	pvt->_position=pvt->_position+size;
	if (pvt->_position>pvt->_end) {
		pvt->_end=pvt->_position;
	}

	return size;
}

bytebuffer *bytebuffer::write(const unsigned char *data, size_t size) {

	// bail if we're not actually writing anything
	if (!size) {
		return this;
	}

	// extend the list of buffers to accommodate "size"
	// bytes beyond the current position
	extend(size);

	// copy-out "size" bytes, updating the current extent and
	// number of bytes remaining in that extent as we go
	size_t	pos=0;
	while (size) {
		unsigned char	*buffer=pvt->_curext->getValue()->_buffer;
		size_t		extsize=pvt->_curext->getValue()->_size;
		size_t		remaining=pvt->_curext->getValue()->_remaining;
		size_t		bytestocopy=(remaining<size)?remaining:size;
		bytestring::copy(buffer+extsize-remaining,data+pos,bytestocopy);
		size=size-bytestocopy;
		if (size) {
			pvt->_curext=pvt->_curext->getNext();
			pvt->_curext->getValue()->_remaining=
				pvt->_curext->getValue()->_size;
		} else {
			pvt->_curext->getValue()->_remaining=
				pvt->_curext->getValue()->_remaining-
				bytestocopy;
		}
		pos=pos+bytestocopy;
	}

	// update the current position and end of buffer
	pvt->_position=pvt->_position+size;
	if (pvt->_position>pvt->_end) {
		pvt->_end=pvt->_position;
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

bytebuffer *bytebuffer::writeFormatted(const char *format, va_list *argp) {

	// find out how much space we need...
	//
	// Some compilers throw a warning if they see "printf(NULL..." at all,
	// independent of whether it's the global function printf() or one that
	// you've defined yourself.  This buffer=NULL thing works around it.
	/*char	*buffer=NULL;
	size_t	size=charstring::printf(buffer,0,format,argp);*/

	// FIXME: extend the buffer if necessary

	// copy the data into the buffer
	/*size=charstring::printf((char *)(pvt->_buffer+pvt->_position),
						pvt->_buffersize-pvt->_position,
						format,argp);*/

	// FIXME: increment the position indices
	return this;
}

void bytebuffer::clear() {

	// delete all extents
	for (pvt->_curext=pvt->_extents.getFirst();
		pvt->_curext; pvt->_curext=pvt->_curext->getNext()) {
		delete[] pvt->_curext->getValue();
	}
	pvt->_extents.clear();

	// re-init
	init(NULL,pvt->_initial,pvt->_extsize);
}

void bytebuffer::extend(size_t bytestowrite) {

	// figure out how many bytes are available between the current
	// position and the end of the extent list...
	// if the position is already set past the end of the extent list then
	// handle that by pretending that we just have more bytes to write...
	size_t	availablebytes=0;
	if (pvt->_position>=pvt->_totalbytes) {
		bytestowrite=bytestowrite+pvt->_position-pvt->_totalbytes;
	} else {
		availablebytes=pvt->_totalbytes-pvt->_position;
	}

	// bail if we have enough space to accommodate
	// the specified number of bytes
	if (bytestowrite<=availablebytes) {
		return;
	}

	// calculate how many new extents we need
	size_t	newextents=(bytestowrite-availablebytes)/pvt->_extsize;

	// create those extents
	for (size_t i=0; i<newextents; i++) {
		bytebufferextent	*newext=new bytebufferextent;
		newext->_buffer=new unsigned char[pvt->_extsize];
		newext->_size=pvt->_extsize;
		newext->_remaining=pvt->_extsize;
		pvt->_extents.append(newext);
	}

	// update the total number of bytes in the buffer list
	pvt->_totalbytes=pvt->_totalbytes+pvt->_extsize*newextents;
}

const unsigned char *bytebuffer::getBuffer() {
	coalesce();
	return pvt->_curext->getValue()->_buffer;
}

unsigned char *bytebuffer::detachBuffer() {
	coalesce();
	unsigned char	*retval=pvt->_curext->getValue()->_buffer;
	pvt->_curext->getValue()->_buffer=NULL;
	clear();
	return retval;
}

void bytebuffer::coalesce() {

	// re-allocate the coalesced-data buffer
	size_t		coalescedbuffersize=pvt->_end;
	unsigned char	*coalescedbuffer=new unsigned char[coalescedbuffersize];

	// copy data into the buffer
	size_t	pos=0;
	for (pvt->_curext=pvt->_extents.getFirst();
		pvt->_curext; pvt->_curext=pvt->_curext->getNext()) {
	
		bytebufferextent	*bbe=pvt->_curext->getValue();
		size_t	bytestocopy=bbe->_size-bbe->_remaining;
		bytestring::copy(coalescedbuffer+pos,bbe->_buffer,bytestocopy);
	}

	// clear everything
	clear();

	// create a single extent with this data as the buffer
	bytebufferextent	*newext=new bytebufferextent;
	newext->_buffer=coalescedbuffer;
	newext->_size=coalescedbuffersize;
	newext->_remaining=0;
	pvt->_extents.append(newext);
	pvt->_curext=pvt->_extents.getFirst();
	pvt->_position=coalescedbuffersize;
	pvt->_end=coalescedbuffersize;
	pvt->_totalbytes=coalescedbuffersize;
}

size_t bytebuffer::getSize() {
	return pvt->_end;
}

size_t bytebuffer::getPosition() {
	return pvt->_position;
}

size_t bytebuffer::getEnd() {
	return pvt->_end;
}

size_t bytebuffer::getActualSize() {
	return pvt->_totalbytes;
}

void bytebuffer::setPosition(size_t pos) {

	// set the position
	pvt->_position=pos;

	// determine which extent to move to and set the current extent
	pvt->_curext=pvt->_extents.getFirst();
	for (;;) {
		size_t	size=pvt->_curext->getValue()->_size;
		if (size>pos) {
			break;
		}
		pvt->_curext=pvt->_curext->getNext();
		pos=pos-pvt->_curext->getValue()->_size;
	}

	// determine how many bytes remain in that extent beyond this position
	pvt->_curext->getValue()->_remaining=
		pvt->_curext->getValue()->_size-pos;
}

bytebuffer *bytebuffer::append(const unsigned char *data, size_t size) {
	pvt->_position=pvt->_end;
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

bytebuffer *bytebuffer::appendFormatted(const char *format, va_list *argp) {
	pvt->_position=pvt->_end;
	return writeFormatted(format,argp);
}

void bytebuffer::truncate(size_t pos) {
	pvt->_end=pos;
}

void bytebuffer::truncate() {
	pvt->_end=pvt->_position;
}

size_t bytebuffer::_end() {
	return pvt->_end;
}

void bytebuffer::_end(size_t e) {
	pvt->_end=e;
}

size_t bytebuffer::_position() {
	return pvt->_position;
}

void bytebuffer::_position(size_t pos) {
	pvt->_position=pos;
}
