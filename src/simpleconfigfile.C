// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <simpleconfigfile.h>
#include <file.h>
#ifndef ENABLE_INLINES
	#include <rudiments/private/simpleconfigfileinlines.h>
#endif

int simpleconfigfile::parseFile(const char *varfilename) {

	// open file
	file	configfile;
	if (!configfile.open(varfilename,O_RDONLY,NULL)) {
		return 0;
	}

	// get the contents of the file
	char	*filestring=configfile.getContents();
	configfile.close();
	
	// parse the string
	parseString(filestring);

	// clean up
	delete[] filestring;

	return 1;
}

void simpleconfigfile::parseString(const char *string) {
	currentbuffer=new stringbuffer();
	this->stringptr=string;
	while (parseWhitespace() && parseComment() &&
		parseWhitespace() && parseName() &&
		parseWhitespace() && parseValue());
}

int simpleconfigfile::parseWhitespace() {
	return parseWhile(" 	\n\r");
}

int simpleconfigfile::parseComment() {
	if (*stringptr=='#') {
		if (!parseUntil("\n\r")) {
			return 0;
		}
	}
	return 1;
}

int simpleconfigfile::parseName() {
	if (*stringptr!='#') {
		if (!parseUntil(":")) {
			return 0;
		}
		name();
		return advance();
	}
	return 1;
}

int simpleconfigfile::parseValue() {
	if (!parseUntil("\r\n")) {
		return 0;
	}
	value();
	return 1;
}

int simpleconfigfile::parseWhile(char *instring) {
	while (strchr(instring,*stringptr)) {
		if (!advance()) {
			return 0;
		}
	}
	return 1;
}

int simpleconfigfile::parseUntil(char *instring) {
	while (!strchr(instring,*stringptr)) {
		if (!advance()) {
			return 0;
		}
	}
	return 1;
}

int simpleconfigfile::advance() {
	currentbuffer->append(*stringptr);
	stringptr++;
	if (!*stringptr) {
		return 0;
	}
	return 1;
}

void simpleconfigfile::name() {
}

void simpleconfigfile::value() {
}
