// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>

stringbuffer::stringbuffer() : bytebuffer(128,32) {
}

stringbuffer::stringbuffer(const stringbuffer &s) : bytebuffer(s) {
}

stringbuffer &stringbuffer::operator=(const stringbuffer &s) {
	if (this!=&s) {
		bytebuffer::operator=(s);
	}
	return *this;
}

stringbuffer::stringbuffer(char *initialcontents,
				size_t initialsize, size_t increment) :
		bytebuffer(reinterpret_cast<unsigned char *>(initialcontents),
							initialsize,increment) {
}

stringbuffer::~stringbuffer() {
}

void stringbuffer::setPosition(size_t pos) {
	bytebuffer::setPosition(pos);
}

const char *stringbuffer::getString() {
	bytebuffer::append('\0');
	const char	*retval=reinterpret_cast<const char *>(getBuffer());
	_position(_position()-1);
	_end(_end()-1);
	return retval;
}

size_t stringbuffer::getStringLength() {
	return charstring::length(getString());
}

char *stringbuffer::detachString() {
	bytebuffer::append('\0');
	return reinterpret_cast<char *>(detachBuffer());
}

size_t stringbuffer::getPosition() {
	return bytebuffer::getPosition();
}

void stringbuffer::clear() {
	bytebuffer::clear();
}

stringbuffer *stringbuffer::append(const unsigned char *string) {
	bytebuffer::append(string,charstring::length(string));
	return this;
}

stringbuffer *stringbuffer::append(const unsigned char *string, size_t size) {
	bytebuffer::append(string,size);
	return this;
}

stringbuffer *stringbuffer::append(const char *string) {
	bytebuffer::append(string,charstring::length(string));
	return this;
}

stringbuffer *stringbuffer::append(const char *string, size_t size) {
	bytebuffer::append(string,size);
	return this;
}

stringbuffer *stringbuffer::append(char character) {
	bytebuffer::append(character);
	return this;
}

stringbuffer *stringbuffer::append(int16_t number) {
	return append(number,1);
}

stringbuffer *stringbuffer::append(int32_t number) {
	return append(number,1);
}

stringbuffer *stringbuffer::append(int64_t number) {
	return append(number,1);
}

stringbuffer *stringbuffer::append(unsigned char character) {
	bytebuffer::append(character);
	return this;
}

stringbuffer *stringbuffer::append(uint16_t number) {
	return append(number,1);
}

stringbuffer *stringbuffer::append(uint32_t number) {
	return append(number,1);
}

stringbuffer *stringbuffer::append(uint64_t number) {
	return append(number,1);
}

stringbuffer *stringbuffer::append(int16_t number, uint16_t zeropadding) {
	bytebuffer::appendFormatted("%0*hd",zeropadding,number);
	return this;
}

stringbuffer *stringbuffer::append(int32_t number, uint16_t zeropadding) {
	bytebuffer::appendFormatted("%0*d",zeropadding,number);
	return this;
}

stringbuffer *stringbuffer::append(int64_t number, uint16_t zeropadding) {
	bytebuffer::appendFormatted("%0*lld",zeropadding,number);
	return this;
}

stringbuffer *stringbuffer::append(uint16_t number, uint16_t zeropadding) {
	bytebuffer::appendFormatted("%0*hd",zeropadding,number);
	return this;
}

stringbuffer *stringbuffer::append(uint32_t number, uint16_t zeropadding) {
	bytebuffer::appendFormatted("%0*d",zeropadding,number);
	return this;
}

stringbuffer *stringbuffer::append(uint64_t number, uint16_t zeropadding) {
	bytebuffer::appendFormatted("%0*lld",zeropadding,number);
	return this;
}

stringbuffer *stringbuffer::append(float number) {
	return append(number,4);
}

stringbuffer *stringbuffer::append(float number, uint16_t scale) {
	bytebuffer::appendFormatted("%.*f",scale,number);
	return this;
}

stringbuffer *stringbuffer::append(float number, uint16_t precision,
							uint16_t scale) {
	bytebuffer::appendFormatted("%*.*f",precision,scale,number);
	return this;
}

stringbuffer *stringbuffer::append(double number) {
	return append(number,4);
}

stringbuffer *stringbuffer::append(double number, uint16_t scale) {
	bytebuffer::appendFormatted("%.*f",scale,number);
	return this;
}

stringbuffer *stringbuffer::append(double number, uint16_t precision,
							uint16_t scale) {
	bytebuffer::appendFormatted("%*.*f",precision,scale,number);
	return this;
}

stringbuffer *stringbuffer::write(const unsigned char *string) {
	bytebuffer::write(string,charstring::length(string));
	return this;
}

stringbuffer *stringbuffer::write(const unsigned char *string, size_t size) {
	bytebuffer::write(string,size);
	return this;
}

stringbuffer *stringbuffer::write(const char *string) {
	bytebuffer::write(string,charstring::length(string));
	return this;
}

stringbuffer *stringbuffer::write(const char *string, size_t size) {
	bytebuffer::write(string,size);
	return this;
}

stringbuffer *stringbuffer::write(char character) {
	bytebuffer::write(character);
	return this;
}

stringbuffer *stringbuffer::write(int16_t number) {
	bytebuffer::writeFormatted("%hd",number);
	return this;
}

stringbuffer *stringbuffer::write(int32_t number) {
	bytebuffer::writeFormatted("%d",number);
	return this;
}

stringbuffer *stringbuffer::write(int64_t number) {
	bytebuffer::writeFormatted("%lld",number);
	return this;
}

stringbuffer *stringbuffer::write(unsigned char character) {
	bytebuffer::write(character);
	return this;
}

stringbuffer *stringbuffer::write(uint16_t number) {
	bytebuffer::writeFormatted("%hd",number);
	return this;
}

stringbuffer *stringbuffer::write(uint32_t number) {
	bytebuffer::writeFormatted("%d",number);
	return this;
}

stringbuffer *stringbuffer::write(uint64_t number) {
	bytebuffer::writeFormatted("%lld",number);
	return this;
}

stringbuffer *stringbuffer::write(float number) {
	return write(number,4);
}

stringbuffer *stringbuffer::write(float number, uint16_t scale) {
	bytebuffer::writeFormatted("%.*f",scale,number);
	return this;
}

stringbuffer *stringbuffer::write(float number, uint16_t precision,
							uint16_t scale) {
	bytebuffer::writeFormatted("%*.*f",precision,scale,number);
	return this;
}

stringbuffer *stringbuffer::write(double number) {
	return write(number,4);
}

stringbuffer *stringbuffer::write(double number, uint16_t scale) {
	bytebuffer::writeFormatted("%.*f",scale,number);
	return this;
}

stringbuffer *stringbuffer::write(double number, uint16_t precision,
							uint16_t scale) {
	bytebuffer::writeFormatted("%*.*f",precision,scale,number);
	return this;
}

void stringbuffer::truncate(size_t pos) {
	bytebuffer::truncate(pos);
}

void stringbuffer::truncate() {
	bytebuffer::truncate();
}
