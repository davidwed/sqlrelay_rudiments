// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>

#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

stringbuffer::stringbuffer() : variablebuffer(128,32) {}

stringbuffer::stringbuffer(const stringbuffer &s) : variablebuffer(s) {}

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
}

stringbuffer::~stringbuffer() {
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
}

void stringbuffer::terminate() {
	// If you've come here chasing down valgrind errors...
	// Valgrind doesn't like it if we test buffer[endofbuffer] unless
	// buffer[endofbuffer] has actually been written to.  In reality, it
	// doesn't matter if buffer[endofbuffer] has had a NULL written to it
	// of if it just happens to have a NULL in it already.  Valgrind is
	// being a bit over cautious.
	if (buffer[endofbuffer]!=(unsigned char)NULL) {
		variablebuffer::append((unsigned char)NULL);
		endofbuffer--;
		position--;
	}
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

stringbuffer *stringbuffer::append(short number) {
	return append(number,0);
}

stringbuffer *stringbuffer::append(long number) {
	return append(number,0);
}

stringbuffer *stringbuffer::append(long long number) {
	return append(number,0);
}

stringbuffer *stringbuffer::append(unsigned short number) {
	return append(number,0);
}

stringbuffer *stringbuffer::append(unsigned long number) {
	return append(number,0);
}

stringbuffer *stringbuffer::append(unsigned long long number) {
	return append(number,0);
}

stringbuffer *stringbuffer::append(short number, unsigned short zeropadding) {
	char	*numstr=charstring::parseNumber(number,zeropadding);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(long number, unsigned short zeropadding) {
	char	*numstr=charstring::parseNumber(number,zeropadding);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(long long number,
					unsigned short zeropadding) {
	char	*numstr=charstring::parseNumber(number,zeropadding);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(unsigned short number,
					unsigned short zeropadding) {
	char	*numstr=charstring::parseNumber(number,zeropadding);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(unsigned long number,
					unsigned short zeropadding) {
	char	*numstr=charstring::parseNumber(number,zeropadding);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(unsigned long long number,
					unsigned short zeropadding) {
	char	*numstr=charstring::parseNumber(number,zeropadding);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(float number) {
	return append(number,4);
}

stringbuffer *stringbuffer::append(float number, unsigned short scale) {
	char	*numstr=charstring::parseNumber(number,scale);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(float number, unsigned short precision,
							unsigned short scale) {
	char	*numstr=charstring::parseNumber(number,precision,scale);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(double number) {
	return append(number,4);
}

stringbuffer *stringbuffer::append(double number, unsigned short scale) {
	char	*numstr=charstring::parseNumber(number,scale);
	variablebuffer::append(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(double number, unsigned short precision,
							unsigned short scale) {
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

stringbuffer *stringbuffer::write(short number) {
	char	*numstr=charstring::parseNumber(number);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(long number) {
	char	*numstr=charstring::parseNumber(number);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(long long number) {
	char	*numstr=charstring::parseNumber(number);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(unsigned char character) {
	variablebuffer::write(character);
	return this;
}

stringbuffer *stringbuffer::write(unsigned short number) {
	char	*numstr=charstring::parseNumber(number);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(unsigned long number) {
	char	*numstr=charstring::parseNumber(number);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(unsigned long long number) {
	char	*numstr=charstring::parseNumber(number);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(float number) {
	return write(number,4);
}

stringbuffer *stringbuffer::write(float number, unsigned short scale) {
	char	*numstr=charstring::parseNumber(number,scale);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(float number, unsigned short precision,
							unsigned short scale) {
	char	*numstr=charstring::parseNumber(number,precision,scale);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(double number) {
	return write(number,4);
}

stringbuffer *stringbuffer::write(double number, unsigned short scale) {
	char	*numstr=charstring::parseNumber(number,scale);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(double number, unsigned short precision,
							unsigned short scale) {
	char	*numstr=charstring::parseNumber(number,precision,scale);
	variablebuffer::write(numstr,charstring::length(numstr));
	delete[] numstr;
	return this;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
