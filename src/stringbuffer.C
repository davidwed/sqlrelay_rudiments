// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>

#include <stdio.h>


stringbuffer::stringbuffer() : variablebuffer(128,32) {
}

stringbuffer::stringbuffer(char *initialcontents,
					size_t initialsize, size_t increment) :
			variablebuffer((unsigned char *)initialcontents,
						initialsize,increment) {
}

stringbuffer::~stringbuffer() {
}

void stringbuffer::setPosition(size_t pos) {
	variablebuffer::setPosition(pos);
}

char *stringbuffer::getString() {
	terminate();
	return (char *)getBuffer();
}

size_t stringbuffer::getPosition() {
	return variablebuffer::getPosition();
}

void stringbuffer::clear() {
	variablebuffer::clear();
}

void stringbuffer::terminate() {
	if (buffer[endofbuffer]!='\0') {
		variablebuffer::append((unsigned char *)"\0",1);
		endofbuffer--;
		position--;
	}
}

stringbuffer *stringbuffer::append(const char *string, size_t size) {
	variablebuffer::append((unsigned char *)string,size);
	return this;
}

stringbuffer *stringbuffer::append(char character) {
	variablebuffer::append((unsigned char *)&character,sizeof(character));
	return this;
}

stringbuffer *stringbuffer::append(double number) {
	return append(number,4);
}

stringbuffer *stringbuffer::write(const char *string, size_t size) {
	variablebuffer::write((unsigned char *)string,size);
	return this;
}

stringbuffer *stringbuffer::write(char character) {
	variablebuffer::write((unsigned char *)&character,sizeof(character));
	return this;
}

stringbuffer *stringbuffer::write(double number) {
	return write(number,4);
}

size_t stringbuffer::getStringLength() {
	return charstring::length(getString());
}

stringbuffer *stringbuffer::append(const char *string) {
	if (string) {
		variablebuffer::append((unsigned char *)string,
					charstring::length(string));
	}
	return this;
}

stringbuffer *stringbuffer::append(long number) {
	return append(number,0);
}

stringbuffer *stringbuffer::append(unsigned long number) {
	return append(number,0);
}

stringbuffer *stringbuffer::append(long number, unsigned short zeropadding) {
	char	*numstr=charstring::parseNumber(number,zeropadding);
	variablebuffer::append((unsigned char *)numstr,
					charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(unsigned long number,
					unsigned short zeropadding) {
	char	*numstr=charstring::parseNumber(number,zeropadding);
	variablebuffer::append((unsigned char *)numstr,
					charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(double number, unsigned short scale) {
	char	*numstr=charstring::parseNumber(number,scale);
	variablebuffer::append((unsigned char *)numstr,
					charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::append(double number, unsigned short precision,
							unsigned short scale) {
	char	*numstr=charstring::parseNumber(number,precision,scale);
	variablebuffer::append((unsigned char *)numstr,
					charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(const char *string) {
	variablebuffer::write((unsigned char *)string,
					charstring::length(string));
	return this;
}

stringbuffer *stringbuffer::write(long number) {
	char	*numstr=charstring::parseNumber(number);
	variablebuffer::write((unsigned char *)numstr,
					charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(unsigned long number) {
	char	*numstr=charstring::parseNumber(number);
	variablebuffer::write((unsigned char *)numstr,
					charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(double number, unsigned short scale) {
	char	*numstr=charstring::parseNumber(number,scale);
	variablebuffer::write((unsigned char *)numstr,
					charstring::length(numstr));
	delete[] numstr;
	return this;
}

stringbuffer *stringbuffer::write(double number, unsigned short precision,
							unsigned short scale) {
	char	*numstr=charstring::parseNumber(number,precision,scale);
	variablebuffer::write((unsigned char *)numstr,
					charstring::length(numstr));
	delete[] numstr;
	return this;
}
