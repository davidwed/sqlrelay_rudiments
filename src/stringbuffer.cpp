// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>

#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class stringbufferprivate {
	friend class stringbuffer;
	private:
};

stringbuffer::stringbuffer() : variablebuffer(128,32) {
	pvt=new stringbufferprivate;
}

stringbuffer::stringbuffer(const stringbuffer &s) : variablebuffer(s) {
	pvt=new stringbufferprivate;
}

stringbuffer &stringbuffer::operator=(const stringbuffer &s) {
	if (this!=&s) {
		variablebuffer::operator=(s);
	}
	return *this;
}

stringbuffer::stringbuffer(char *initialcontents,
					size_t initialsize, size_t increment) :
			variablebuffer(reinterpret_cast<unsigned char *>(
							initialcontents),
							initialsize,increment) {
	pvt=new stringbufferprivate;
}

stringbuffer::~stringbuffer() {
	delete pvt;
}

void stringbuffer::setPosition(size_t pos) {
	variablebuffer::setPosition(pos);
}

const char *stringbuffer::getString() {
	terminate();
	return reinterpret_cast<const char *>(getBuffer());
}

size_t stringbuffer::getStringLength() {
	return charstring::length(getString());
}

char *stringbuffer::detachString() {
	terminate();
	return reinterpret_cast<char *>(detachBuffer());
}

size_t stringbuffer::getPosition() {
	return variablebuffer::getPosition();
}

void stringbuffer::clear() {
	variablebuffer::clear();
	_buffer()[0]='\0';
}

void stringbuffer::terminate() {
	variablebuffer::append((unsigned char)NULL);
	_endofbuffer(_endofbuffer()-1);
	_position(_position()-1);
}

stringbuffer *stringbuffer::append(const unsigned char *string) {
	if (string) {
		variablebuffer::append(string,charstring::length(string));
	}
	return this;
}

stringbuffer *stringbuffer::append(const unsigned char *string, size_t size) {
	variablebuffer::append(string,size);
	return this;
}

stringbuffer *stringbuffer::append(const char *string) {
	if (string) {
		variablebuffer::append(string,charstring::length(string));
	}
	return this;
}

stringbuffer *stringbuffer::append(const char *string, size_t size) {
	variablebuffer::append(string,size);
	return this;
}

stringbuffer *stringbuffer::append(char character) {
	variablebuffer::append(character);
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
	variablebuffer::append(character);
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
	char	*numstr=charstring::parseNumber(number,zeropadding);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(int32_t number, uint16_t zeropadding) {
	char	*numstr=charstring::parseNumber(number,zeropadding);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(int64_t number, uint16_t zeropadding) {
	char	*numstr=charstring::parseNumber(number,zeropadding);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(uint16_t number, uint16_t zeropadding) {
	char	*numstr=charstring::parseNumber(number,zeropadding);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(uint32_t number, uint16_t zeropadding) {
	char	*numstr=charstring::parseNumber(number,zeropadding);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(uint64_t number, uint16_t zeropadding) {
	char	*numstr=charstring::parseNumber(number,zeropadding);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(float number) {
	return append(number,4);
}

stringbuffer *stringbuffer::append(float number, uint16_t scale) {
	char	*numstr=charstring::parseNumber(number,scale);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(float number, uint16_t precision,
							uint16_t scale) {
	char	*numstr=charstring::parseNumber(number,precision,scale);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(double number) {
	return append(number,4);
}

stringbuffer *stringbuffer::append(double number, uint16_t scale) {
	char	*numstr=charstring::parseNumber(number,scale);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(double number, uint16_t precision,
							uint16_t scale) {
	char	*numstr=charstring::parseNumber(number,precision,scale);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(const unsigned char *string) {
	variablebuffer::write(string,charstring::length(string));
	return this;
}

stringbuffer *stringbuffer::write(const unsigned char *string, size_t size) {
	variablebuffer::write(string,size);
	return this;
}

stringbuffer *stringbuffer::write(const char *string) {
	variablebuffer::write(string,charstring::length(string));
	return this;
}

stringbuffer *stringbuffer::write(const char *string, size_t size) {
	variablebuffer::write(string,size);
	return this;
}

stringbuffer *stringbuffer::write(char character) {
	variablebuffer::write(character);
	return this;
}

stringbuffer *stringbuffer::write(int16_t number) {
	char	*numstr=charstring::parseNumber(number);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(int32_t number) {
	char	*numstr=charstring::parseNumber(number);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(int64_t number) {
	char	*numstr=charstring::parseNumber(number);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(unsigned char character) {
	variablebuffer::write(character);
	return this;
}

stringbuffer *stringbuffer::write(uint16_t number) {
	char	*numstr=charstring::parseNumber(number);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(uint32_t number) {
	char	*numstr=charstring::parseNumber(number);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(uint64_t number) {
	char	*numstr=charstring::parseNumber(number);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(float number) {
	return write(number,4);
}

stringbuffer *stringbuffer::write(float number, uint16_t scale) {
	char	*numstr=charstring::parseNumber(number,scale);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(float number, uint16_t precision,
							uint16_t scale) {
	char	*numstr=charstring::parseNumber(number,precision,scale);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(double number) {
	return write(number,4);
}

stringbuffer *stringbuffer::write(double number, uint16_t scale) {
	char	*numstr=charstring::parseNumber(number,scale);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(double number, uint16_t precision,
							uint16_t scale) {
	char	*numstr=charstring::parseNumber(number,precision,scale);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

void stringbuffer::truncate(size_t pos) {
	variablebuffer::truncate(pos);
	terminate();
}

void stringbuffer::truncate() {
	variablebuffer::truncate();
	terminate();
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
