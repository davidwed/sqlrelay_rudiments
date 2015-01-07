// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/bytebuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/linkedlist.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_HAVE_VASPRINTF
	#include <stdio.h>
#endif


class bytebufferextent {
	friend class bytebuffer;
	private:
		unsigned char	*_buffer;
		size_t		_pos;
		size_t		_size;
		size_t		_avail;
};

class bytebufferprivate {
	friend class bytebuffer;
	private:
		linkedlist< bytebufferextent * >	_extents;
		linkedlistnode< bytebufferextent * >	*_curext;
		size_t		_initial;
		size_t		_extsize;
		size_t		_pos;
		size_t		_end;
		size_t		_totalbytes;
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
	pvt->_initial=initialsize;
	pvt->_extsize=increment;
	if (initialcontents) {
		bytebufferextent	*newext=new bytebufferextent;
		newext->_buffer=initialcontents;
		newext->_pos=0;
		newext->_size=initialsize;
		newext->_avail=0;
		pvt->_extents.append(newext);
		pvt->_curext=pvt->_extents.getFirst();
		pvt->_pos=initialsize;
		pvt->_end=initialsize;
		pvt->_totalbytes=initialsize;
	} else {
		pvt->_curext=NULL;
		pvt->_pos=0;
		pvt->_end=0;
		pvt->_totalbytes=0;
		extend(initialsize);
	}
}

bytebuffer::bytebuffer(const bytebuffer &v) {
	bytebufferClone(v);
}

bytebuffer &bytebuffer::operator=(const bytebuffer &v) {
	if (this!=&v) {
		clearExtents();
		delete pvt;
		bytebufferClone(v);
	}
	return *this;
}

bytebuffer::~bytebuffer() {
	clearExtents();
	delete pvt;
}

void bytebuffer::clearExtents() {
	pvt->_curext=pvt->_extents.getFirst();
	while (pvt->_curext) {
		linkedlistnode< bytebufferextent * >
					*next=pvt->_curext->getNext();
		bytebufferextent	*bbe=pvt->_curext->getValue();
		delete[] bbe->_buffer;
		delete bbe;
		pvt->_extents.remove(pvt->_curext);
 		pvt->_curext=next;
	}
}

void bytebuffer::bytebufferClone(const bytebuffer &v) {
	
	pvt=new bytebufferprivate;

	// clone the extents
	for (linkedlistnode< bytebufferextent * > *curext=
				v.pvt->_extents.getFirst();
				curext; curext=curext->getNext()) {

		bytebufferextent	*bbe=curext->getValue();

		bytebufferextent	*newext=new bytebufferextent;
		newext->_buffer=(unsigned char *)
				bytestring::duplicate(bbe->_buffer,bbe->_size);
		newext->_pos=bbe->_pos;
		newext->_size=bbe->_size;
		newext->_avail=bbe->_avail;
		pvt->_extents.append(newext);

		if (curext==v.pvt->_curext) {
			pvt->_curext=pvt->_extents.getLast();
		}
	}

	// clone sizes and positions
	pvt->_initial=v.pvt->_initial;
	pvt->_extsize=v.pvt->_extsize;
	pvt->_pos=v.pvt->_pos;
	pvt->_end=v.pvt->_end;
	pvt->_totalbytes=v.pvt->_totalbytes;
}

ssize_t bytebuffer::read(unsigned char *data, size_t size) {
	size_t	bytescopied;
	copy(data,size,false,&bytescopied);
	return bytescopied;
}

bytebuffer *bytebuffer::write(const unsigned char *data, size_t size) {
	size_t	bytescopied;
	return copy((unsigned char *)data,size,true,&bytescopied);
}

bytebuffer *bytebuffer::copy(unsigned char *data, size_t size,
					bool copyin, size_t *bytescopied) {

	// bail if we're not actually writing anything
	// or, if copying out, if the current position
	// is set to or set beyond the end
	if (!size || (!copyin && pvt->_pos>=pvt->_end)) {
		*bytescopied=0;
		return this;
	}

	// if we're copying in, extend the list of buffers
	// to accommodate "size" bytes beyond the current position
	if (copyin) {
		extend(size);
	}

	// determine which extent to start from
	for (;;) {
		bytebufferextent	*bbe=pvt->_curext->getValue();
		if (bbe->_pos>pvt->_pos) {
			pvt->_curext=pvt->_curext->getPrevious();
		} else if (bbe->_pos<=pvt->_pos &&
				bbe->_pos+bbe->_size<pvt->_pos) {
			pvt->_curext=pvt->_curext->getNext();
		} else {
			break;
		}
	}

	// detemine how many bytes are available in that extent
	// beyond this position
	pvt->_curext->getValue()->_avail=
		pvt->_curext->getValue()->_pos+
		pvt->_curext->getValue()->_size-
		pvt->_pos;

	// copy-in "size" bytes, updating the current extent and
	// number of bytes available in that extent as we go
	size_t	remaintocopy=size;
	size_t	pos=0;
	*bytescopied=0;
	while (remaintocopy) {
		bytebufferextent	*bbe=pvt->_curext->getValue();
		size_t	avail=bbe->_avail;
		size_t	bytestocopy=(avail<remaintocopy)?avail:remaintocopy;
		if (copyin) {
			bytestring::copy(bbe->_buffer+bbe->_size-avail,
							data+pos,bytestocopy);
		} else {
			bytestring::copy(data+pos,
				bbe->_buffer+bbe->_size-avail,bytestocopy);
		}
		remaintocopy=remaintocopy-bytestocopy;
		pos=pos+bytestocopy;
		*bytescopied=*bytescopied+bytestocopy;
		if (remaintocopy) {
			bbe->_avail=0;
			pvt->_curext=pvt->_curext->getNext();
			if (!copyin && !pvt->_curext) {
				break;
			}
		} else {
			bbe->_avail=avail-bytestocopy;
		}
	}

	// update the current position and, if we're copyin in, end of buffer
	pvt->_pos=pvt->_pos+size;
	if (copyin && pvt->_pos>pvt->_end) {
		pvt->_end=pvt->_pos;
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

	// If vasprintf is available, then use it, otherwise play games with
	// charstring::printf().
	char	*buffer=NULL;
	ssize_t	size=0;
	#ifdef RUDIMENTS_HAVE_VASPRINTF
	size=vasprintf(&buffer,format,*argp);
	#else
	// Some compilers throw a warning if they see "printf(NULL..." at all,
	// whether it's the global function printf() or one that you've defined
	// yourself.  Using buffer here works around that.
	size=charstring::printf(buffer,0,format,argp);
	buffer=new char[size+1];
	size=charstring::printf(buffer,size+1,format,argp);
	#endif

	// extend the list of buffers to accommodate
	// "size" bytes beyond the current position
	extend(size);

	// write the buffer
	write(buffer,size);

	// clean up
	delete[] buffer;
	
	return this;
}

void bytebuffer::clear() {
	clear(true);
}

void bytebuffer::clear(bool resetpositions) {

	// remove all but the first extent
	bytebufferextent	*bbe;
	pvt->_curext=pvt->_extents.getLast();
	while (pvt->_curext!=pvt->_extents.getFirst()) {
		linkedlistnode< bytebufferextent * >
					*prev=pvt->_curext->getPrevious();
		bbe=pvt->_curext->getValue();
		delete[] bbe->_buffer;
		delete bbe;
		pvt->_extents.remove(pvt->_curext);
		pvt->_curext=prev;
	}

	// FIXME: arguably if the initial extent is larger than pvt->_initial
	// then its buffer ought to be reallocated.  Maybe this should be done
	// periodically.

	// reset available bytes in the first extent
	bbe=pvt->_curext->getValue();
	bbe->_avail=bbe->_size;

	// reset total size
	pvt->_totalbytes=bbe->_size;

	// reset positions
	if (resetpositions) {
		pvt->_pos=0;
		pvt->_end=0;
	}
}

void bytebuffer::extend(size_t bytestowrite) {

	// figure out how many bytes are available between the current
	// position and the end of all extents...
	// if the position is already set past the end of all extents then
	// handle that by pretending that we just have more bytes to write...
	size_t	totalavail=0;
	if (pvt->_pos>=pvt->_totalbytes) {
		bytestowrite=bytestowrite+pvt->_pos-pvt->_totalbytes;
	} else {
		totalavail=pvt->_totalbytes-pvt->_pos;
	}

	// bail if we have enough space to accommodate
	// the specified number of bytes
	if (bytestowrite<=totalavail) {
		return;
	}

	// calculate how many new extents we need
	size_t	newextents=((bytestowrite-totalavail-1)/pvt->_extsize)+1;

	// create those extents
	for (size_t i=0; i<newextents; i++) {
		bytebufferextent	*newext=new bytebufferextent;
		newext->_buffer=new unsigned char[pvt->_extsize];
		if (!pvt->_extents.getLength()) {
			newext->_pos=0;
		} else {
			bytebufferextent	*bbe=
					pvt->_extents.getLast()->getValue();
			newext->_pos=bbe->_pos+bbe->_size;
		}
		newext->_size=pvt->_extsize;
		newext->_avail=pvt->_extsize;
		pvt->_extents.append(newext);
	}

	// update the total number of bytes in all extents
	pvt->_totalbytes=pvt->_totalbytes+pvt->_extsize*newextents;

	// set the curent extent if necessary
	if (!pvt->_curext) {
		pvt->_curext=pvt->_extents.getFirst();
	}
}

const unsigned char *bytebuffer::getBuffer() {
	return coalesce(false);
}

unsigned char *bytebuffer::detachBuffer() {
	return coalesce(true);
}

unsigned char *bytebuffer::coalesce(bool detach) {

	// combine the data from all extents into a single buffer and place
	// that buffer in the first extent...

	// if we're not detaching and there is only 1
	// extent then we don't need to do anything
	if (!detach && pvt->_extents.getLength()==1) {
		return pvt->_curext->getValue()->_buffer;
	}

	// allocate a buffer to copy the data into
	// (since we're probably going to replace the initial extent with this
	// buffer later, make sure the buffer is at least as large as the size
	// specified for the initial segment)
	size_t		coalescedbuffersize=
			(pvt->_initial>pvt->_end)?pvt->_initial:pvt->_end;
	unsigned char	*coalescedbuffer=new unsigned char[coalescedbuffersize];

	// copy data into the buffer
	size_t	pos=0;
	bytebufferextent	*bbe;
	for (pvt->_curext=pvt->_extents.getFirst();
		pvt->_curext; pvt->_curext=pvt->_curext->getNext()) {
	
		bbe=pvt->_curext->getValue();
		size_t	bytestocopy=bbe->_size;
		if (pvt->_end-pos<bytestocopy) {
			bytestocopy=pvt->_end-pos;
		}
		bytestring::copy(coalescedbuffer+pos,bbe->_buffer,bytestocopy);
		pos=pos+bytestocopy;
	}

	// clear all but the first extent
	// (but preserve position and end if we're not detaching)
	clear(detach);

	if (!detach) {

		// replace the first extent's buffer with the coalesced buffer
		pvt->_curext=pvt->_extents.getFirst();
		bbe=pvt->_curext->getValue();
		delete[] bbe->_buffer;
		bbe->_buffer=coalescedbuffer;
		bbe->_size=coalescedbuffersize;
		bbe->_avail=0;
		pvt->_totalbytes=coalescedbuffersize;
	}

	return coalescedbuffer;
}

size_t bytebuffer::getSize() {
	return pvt->_end;
}

size_t bytebuffer::getPosition() {
	return pvt->_pos;
}

size_t bytebuffer::getEnd() {
	return pvt->_end;
}

size_t bytebuffer::getActualSize() {
	return pvt->_totalbytes;
}

void bytebuffer::setPosition(size_t pos) {
	pvt->_pos=pos;
}

bytebuffer *bytebuffer::append(const unsigned char *data, size_t size) {
	pvt->_pos=pvt->_end;
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
	pvt->_pos=pvt->_end;
	return writeFormatted(format,argp);
}

void bytebuffer::truncate(size_t pos) {
	pvt->_end=pos;
}

void bytebuffer::truncate() {
	pvt->_end=pvt->_pos;
}

size_t bytebuffer::_end() {
	return pvt->_end;
}

void bytebuffer::_end(size_t e) {
	pvt->_end=e;
}

size_t bytebuffer::_position() {
	return pvt->_pos;
}

void bytebuffer::_position(size_t pos) {
	pvt->_pos=pos;
}
