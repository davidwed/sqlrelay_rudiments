// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>

// might need this in the future
class stringbufferprivate {
	friend stringbuffer;
};

stringbuffer::stringbuffer() : bytebuffer() {
	// to avoid compiler warnings/errors
	pvt=NULL;
}

stringbuffer::stringbuffer(size_t initialsize, size_t increment) :
			bytebuffer(initialsize,increment) {
}

stringbuffer::stringbuffer(char *initialcontents,
				size_t initialsize, size_t increment) :
			bytebuffer((unsigned char *)initialcontents,
						initialsize,increment) {
}

stringbuffer::stringbuffer(const stringbuffer &s) : bytebuffer(s) {
}

stringbuffer &stringbuffer::operator=(const stringbuffer &s) {
	if (this!=&s) {
		bytebuffer::operator=(s);
	}
	return *this;
}

stringbuffer::~stringbuffer() {
}

void stringbuffer::setPosition(size_t pos) {
	bytebuffer::setPosition(pos);
}

const char *stringbuffer::getString() {
	bytebuffer::append('\0');
	const char	*retval=(const char *)getBuffer();
	_position(_position()-1);
	_end(_end()-1);
	return retval;
}

size_t stringbuffer::getStringLength() {
	return charstring::length(getString());
}

char *stringbuffer::detachString() {
	bytebuffer::append('\0');
	return (char *)detachBuffer();
}

size_t stringbuffer::getPosition() {
	return bytebuffer::getPosition();
}

void stringbuffer::clear() {
	bytebuffer::clear();
}

stringbuffer *stringbuffer::append(const unsigned char *string) {
	return (stringbuffer *)bytebuffer::
				append(string,charstring::length(string));
}

stringbuffer *stringbuffer::append(const unsigned char *string, size_t size) {
	return (stringbuffer *)bytebuffer::append(string,size);
}

stringbuffer *stringbuffer::append(const char *string) {
	return (stringbuffer *)bytebuffer::
				append(string,charstring::length(string));
}

stringbuffer *stringbuffer::append(const char *string, size_t size) {
	return (stringbuffer *)bytebuffer::append(string,size);
}

stringbuffer *stringbuffer::append(char character) {
	return (stringbuffer *)bytebuffer::append(character);
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

stringbuffer *stringbuffer::append(int16_t number, uint16_t zeropadding) {
	return (stringbuffer *)appendFormatted("%0*hd",zeropadding,number);
}

stringbuffer *stringbuffer::append(int32_t number, uint16_t zeropadding) {
	return (stringbuffer *)appendFormatted("%0*d",zeropadding,number);
}

stringbuffer *stringbuffer::append(int64_t number, uint16_t zeropadding) {
	return (stringbuffer *)appendFormatted("%0*lld",zeropadding,number);
}

stringbuffer *stringbuffer::append(unsigned char character) {
	return (stringbuffer *)bytebuffer::append(character);
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

stringbuffer *stringbuffer::append(uint16_t number, uint16_t zeropadding) {
	return (stringbuffer *)appendFormatted("%0*hd",zeropadding,number);
}

stringbuffer *stringbuffer::append(uint32_t number, uint16_t zeropadding) {
	return (stringbuffer *)appendFormatted("%0*d",zeropadding,number);
}

stringbuffer *stringbuffer::append(uint64_t number, uint16_t zeropadding) {
	return (stringbuffer *)appendFormatted("%0*lld",zeropadding,number);
}

stringbuffer *stringbuffer::append(float number) {
	return append(number,4);
}

stringbuffer *stringbuffer::append(float number, uint16_t scale) {
	return (stringbuffer *)appendFormatted("%.*f",scale,number);
}

stringbuffer *stringbuffer::append(float number, uint16_t precision,
							uint16_t scale) {
	return (stringbuffer *)appendFormatted("%*.*f",precision,scale,number);
}

stringbuffer *stringbuffer::append(double number) {
	return append(number,4);
}

stringbuffer *stringbuffer::append(double number, uint16_t scale) {
	return (stringbuffer *)appendFormatted("%.*f",scale,number);
}

stringbuffer *stringbuffer::append(double number, uint16_t precision,
							uint16_t scale) {
	return (stringbuffer *)appendFormatted("%*.*f",precision,scale,number);
}

stringbuffer *stringbuffer::write(const unsigned char *string) {
	return (stringbuffer *)bytebuffer::
				write(string,charstring::length(string));
}

stringbuffer *stringbuffer::write(const unsigned char *string, size_t size) {
	return (stringbuffer *)bytebuffer::write(string,size);
}

stringbuffer *stringbuffer::write(const char *string) {
	return (stringbuffer *)bytebuffer::
				write(string,charstring::length(string));
}

stringbuffer *stringbuffer::write(const char *string, size_t size) {
	return (stringbuffer *)bytebuffer::write(string,size);
}

stringbuffer *stringbuffer::write(char character) {
	return (stringbuffer *)bytebuffer::write(character);
}

stringbuffer *stringbuffer::write(int16_t number) {
	return (stringbuffer *)writeFormatted("%hd",number);
}

stringbuffer *stringbuffer::write(int32_t number) {
	return (stringbuffer *)writeFormatted("%d",number);
}

stringbuffer *stringbuffer::write(int64_t number) {
	return (stringbuffer *)writeFormatted("%lld",number);
}

stringbuffer *stringbuffer::write(unsigned char character) {
	return (stringbuffer *)bytebuffer::write(character);
}

stringbuffer *stringbuffer::write(uint16_t number) {
	return (stringbuffer *)writeFormatted("%hd",number);
}

stringbuffer *stringbuffer::write(uint32_t number) {
	return (stringbuffer *)writeFormatted("%d",number);
}

stringbuffer *stringbuffer::write(uint64_t number) {
	return (stringbuffer *)bytebuffer::writeFormatted("%lld",number);
}

stringbuffer *stringbuffer::write(float number) {
	return write(number,4);
}

stringbuffer *stringbuffer::write(float number, uint16_t scale) {
	return (stringbuffer *)writeFormatted("%.*f",scale,number);
}

stringbuffer *stringbuffer::write(float number, uint16_t precision,
							uint16_t scale) {
	return (stringbuffer *)writeFormatted("%*.*f",precision,scale,number);
}

stringbuffer *stringbuffer::write(double number) {
	return write(number,4);
}

stringbuffer *stringbuffer::write(double number, uint16_t scale) {
	return (stringbuffer *)writeFormatted("%.*f",scale,number);
}

stringbuffer *stringbuffer::write(double number, uint16_t precision,
							uint16_t scale) {
	return (stringbuffer *)writeFormatted("%*.*f",precision,scale,number);
}

void stringbuffer::truncate(size_t pos) {
	bytebuffer::truncate(pos);
}

void stringbuffer::truncate() {
	bytebuffer::truncate();
}
