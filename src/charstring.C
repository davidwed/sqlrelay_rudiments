// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>

#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

charstring::charstring(const char *str, size_t size) {
	memcpy((void *)buffer,(void *)str,size);
	this->size=size;
}

charstring::charstring(size_t size) {
	buffer=new char[size];
	this->size=size;
}

charstring::charstring(const char *str) {
	buffer=strdup(str);
	this->size=strlen(str);
}

charstring::~charstring() {
	delete[] buffer;
}

char *charstring::getString() const {
	return buffer;
}

size_t charstring::getLength() const {
	return strlen(buffer);
}

size_t charstring::getSize() const {
	return size;
}

void charstring::zero() {
	memset((void *)buffer,0,size);
}

void charstring::zero(char *str, size_t size) {
	memset((void *)str,0,size);
}

char *charstring::append(const char *str) {
	return strcat(buffer,str);
}

char *charstring::append(const char *str, size_t size) {
	return strncat(buffer,str,size);
}

char *charstring::append(char *dest, const char *source) {
	return strcat(dest,source);
}

char *charstring::append(char *dest, const char *source, size_t size) {
	return strncat(dest,source,size);
}

char *charstring::copyInto(const char *str) {
	return strcpy(buffer,str);
}

char *charstring::copyInto(const char *str, size_t size) {
	return strncpy(buffer,str,size);
}

char *charstring::copyInto(size_t location, const char *str) {
	return strcpy(buffer+location,str);
}

char *charstring::copyInto(size_t location, const char *str, size_t size) {
	return strncpy(buffer+location,str,size);
}

char *charstring::copyInto(char *dest, const char *source) {
	return strcpy(dest,source);
}

char *charstring::copyInto(char *dest, const char *source, size_t size) {
	return strncpy(dest,source,size);
}

char *charstring::copyInto(char *dest, size_t location, const char *source) {
	return strcpy(dest+location,source);
}

char *charstring::copyInto(char *dest, size_t location,
					const char *source, size_t size) {
	return strncpy(dest+location,source,size);
}

int charstring::compare(const char *str) const {
	return strcmp(buffer,str);
}

int charstring::compare(const char *str, size_t size) const {
	return strncmp(buffer,str,size);
}

int charstring::compareIgnoringCase(const char *str) const {
	return strcasecmp(buffer,str);
}


int charstring::compareIgnoringCase(const char *str, size_t size) const {
	return strncasecmp(buffer,str,size);
}

int charstring::compare(const char *str1, const char *str2) {
	return strcmp(str1,str2);
}

int charstring::compare(const char *str1, const char *str2, size_t size) {
	return strncmp(str1,str2,size);
}

int charstring::compareIgnoringCase(const char *str1, const char *str2) {
	return strcasecmp(str1,str2);
}

int charstring::compareIgnoringCase(const char *str1,
						const char *str2, size_t size) {
	return strncasecmp(str1,str2,size);
}

char *charstring::contains(const char *str) const {
	return strstr(buffer,str);
}

char *charstring::contains(const char ch) const {
	return strchr(buffer,ch);
}

char *charstring::contains(const char *haystack, const char *needle) {
	return strstr(haystack,needle);
}

char *charstring::contains(const char *haystack, const char needle) {
	return strchr(haystack,needle);
}

char *charstring::duplicate() const {
	return strdup(buffer);
}

char *charstring::duplicate(const char *str) {
	return strdup(str);
}

void charstring::rightTrim(char *string) {
	rightTrim(string,' ');
}

void charstring::leftTrim(char *string) {
	leftTrim(string,' ');
}

void charstring::bothTrim(char *string) {
	bothTrim(string,' ');
}

void charstring::upper(char *str) {
	for (int i=0; str[i]; i++) {
		if (str[i]>='a' && str[i]<='z') {
			str[i]=str[i]-32;
		}
	}
}

void charstring::lower(char *str) {
	for (int i=0; str[i]; i++) {
		if (str[i]>='A' && str[i]<='Z') {
			str[i]=str[i]+32;
		}
	}
}

void charstring::rightTrim(char *str, char character) {

	if (str && str[0]) {

		int	i=0;

		// advance to the last character in the string
		while (str[i]) {
			i++;
		}
		i--;

		// back up to the first instance of the character to trim
		while (str[i]==character) { 
			i--;
		}
		i++;

		// terminate the string there
		str[i]=(char)NULL;
	}
}

void charstring::leftTrim(char *str, char character) {

	if (str && str[0]) {

		int	i=0;
		int	j=0;

		// advance past all of the characters we want to replace
		while (str[i]==character) {
			i++;
		}
	
		// swap chars to front of string
		while (str[i]) {
			str[j]=str[i];
			j++;
			i++;
		}
		// store a null to the new end of string
		str[j]=(char)NULL;
	}
}

void charstring::strip(char *str, char character) {

	int	index=0;
	int	total=0;

	while (str[index]) {
		if (str[index]==character) {
			total++;
		} else {
			if (total) {
				str[index-total]=str[index];
			}
		}
		index++;
	}
	str[index-total]=(char)NULL;
}

void charstring::strip(char *str1, char *str2) {

	int	str2len=strlen(str2);
	int	index=0;
	int	total=0;

	while (str1[index]) {
		if (!strncmp(str1+index,str2,str2len)) {
			total=total+str2len;
			index=index+str2len;
		} else {
			if (total) {
				str1[index-total]=str1[index];
			}
			index++;
		}
	}
	str1[index-total]=(char)NULL;
}

bool charstring::isInteger(const char *str) {

	for (char *ptr=(char *)str; *ptr; ptr++) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-') || 
			(ptr>str && *ptr=='-')) {
			return false;
		}
	}
	return true;
}

bool charstring::isInteger(const char *str, int size) {

	char	*ptr=(char *)str;
	for (int index=0; index<size; index++) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-') || 
			(ptr>str && *ptr=='-')) {
			return false;
		}
		ptr++;
	}
	return true;
}

bool charstring::isNumber(const char *str) {

	int	decimal=0;
	for (char *ptr=(char *)str; *ptr; ptr++) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-' && *ptr!='.') || 
			(ptr>str && *ptr=='-') || (decimal && *ptr=='.')) {
			return false;
		}
		if (*ptr=='.') {
			decimal=1;
		}
	}
	return true;
}

bool charstring::isNumber(const char *str, int size) {

	int	decimal=0;
	char	*ptr=(char *)str;
	for (int index=0; index<size; index++) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-' && *ptr!='.') || 
			(ptr>str && *ptr=='-') || (decimal && *ptr=='.')) {
			return false;
		}
		if (*ptr=='.') {
			decimal=1;
		}
		ptr++;
	}
	return true;
}

char *charstring::httpEscape(const char *input) {

	char	*output=new char[strlen(input)*3+1];
	char	*outptr=output;
	char	*ptr=(char *)input;
	
	while (*ptr) {
		if (*ptr==' ') {
			(*outptr)='+';
		} else if ((*ptr>='a' && *ptr<='z') || 
				(*ptr>='A' && *ptr<='Z') ||
				(*ptr>='0' && *ptr<='9')) {
			(*outptr)=*ptr;
		} else {
			sprintf(outptr,"%c%02X",'%',*ptr);
			outptr=outptr+2;
		}
		outptr++;
		ptr++;
	}
	(*outptr)=(char)NULL;

	return output;
}

void charstring::leftJustify(char *str, int length) {

	// count leading spaces
	int	spaces=countLeadingSpaces(str,length);

	// replace characters
	int	index;
	int	stop=length-spaces;
	for (index=0; index<stop; index++) {
		str[index]=str[index+spaces];
	}

	// right-pad with spaces
	for (; index<length; index++) {
		str[index]=' ';
	}
}

void charstring::rightJustify(char *str, int length) {

	// count trailing spaces
	int	spaces=countTrailingSpaces(str,length);

	// replace characters
	int	index;
	int	stop=spaces-1;
	for (index=length-1; index>stop; index--) {
		str[index]=str[index-spaces];
	}

	// right-pad with spaces
	for (; index>-1; index--) {
		str[index]=' ';
	}
}

void charstring::center(char *str, int length) {

	int	leadingspaces=countLeadingSpaces(str,length);
	int	trailingspaces=countTrailingSpaces(str,length);

	int	leftpad=(leadingspaces+trailingspaces)/2;

	if (leftpad>leadingspaces) {
		// shift everything right
		int	difference=leftpad-leadingspaces;
		int	index;
		for (index=length-1; index>difference-1; index--) {
			str[index]=str[index-difference];
		}
		for (; index>-1; index--) {
			str[index]=' ';
		}
	} else if (leftpad<leadingspaces) {
		// shift everything left
		int	difference=leadingspaces-leftpad;
		int	index;
		for (index=0; index<length-difference; index++) {
			str[index]=str[index+difference];
		}
		for (; index<length; index++) {
			str[index]=' ';
		}
	}
}

int charstring::countLeadingSpaces(char *str, int length) {
	int	leadingspaces=0;
	for (int index=0; str[index]==' ' && index<length; index++) {
		leadingspaces++;
	}
	return leadingspaces;
}

int charstring::countTrailingSpaces(char *str, int length) {
	int	trailingspaces=0;
	for (int index=length-1; str[index]==' ' && index>-1; index--) {
		trailingspaces++;
	}
	return trailingspaces;
}

char *charstring::parseNumber(long number) {
	char	*str=new char[integerLength(number)+1];
	sprintf(str,"%ld",number);
	return str;
}

char *charstring::parseNumber(double number) {
	char	*str=new char[22];
	sprintf(str,"%f",number);
	return str;
}

char *charstring::parseNumber(double number,
				unsigned short scale) {
	char	*str=new char[22];
	sprintf(str,"%.*f",scale,number);
	return str;
}

char *charstring::parseNumber(double number,
				unsigned short precision,
				unsigned short scale) {
	char	*str=new char[precision+3];
	sprintf(str,"%*.*f",precision,scale,number);
	return str;
}

char *charstring::append(const charstring *str) {
	return strcat(buffer,str->getString());
}

char *charstring::append(const charstring *str, size_t size) {
	return strncat(buffer,str->getString(),size);
}

void charstring::bothTrim(char *string, char character) {
	leftTrim(string,character);
	rightTrim(string,character);
}

int charstring::integerLength(long number) {
	int	length=(number>0)?0:1;
	for (long num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		length++;
	}
	return length;
}
