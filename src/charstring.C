// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>

#include <rudiments/rawbuffer.h>
#include <rudiments/character.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

char *charstring::findLast(const char *haystack, const char *needle) {

	if (!haystack || !needle) {
		return NULL;
	}

	size_t	haystacklen=length(haystack);
	size_t	needlelen=length(needle);
	if (needlelen>haystacklen) {
		return NULL;
	}
	
	const char	*ptr=haystack+haystacklen-needlelen;
	while (ptr>haystack) {
		if (!compare(ptr,needle)) {
			return const_cast<char *>(ptr);
		}
		ptr--;
	}
	return NULL;
}

void charstring::upper(char *str) {

	if (!str) {
		return;
	}

	for (int i=0; str[i]; i++) {
		if (str[i]>='a' && str[i]<='z') {
			str[i]=str[i]-32;
		}
	}
}

void charstring::lower(char *str) {

	if (!str) {
		return;
	}

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
		str[i]='\0';
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
		str[j]='\0';
	}
}

void charstring::strip(char *str, char character) {

	if (!str) {
		return;
	}

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
	str[index-total]='\0';
}

void charstring::strip(char *str1, char *str2) {

	if (!str1 || !str2) {
		return;
	}

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
	str1[index-total]='\0';
}

bool charstring::isInteger(const char *str) {

	if (!str) {
		return false;
	}

	for (const char *ptr=str; *ptr; ptr++) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-') || 
			(ptr>str && *ptr=='-')) {
			return false;
		}
	}
	return true;
}

bool charstring::isInteger(const char *str, int size) {

	if (!str) {
		return false;
	}

	const char	*ptr=str;
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

	if (!str) {
		return false;
	}

	int	decimal=0;
	for (const char *ptr=str; *ptr; ptr++) {
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

	if (!str) {
		return false;
	}

	const char	*ptr=str;
	int		decimal=0;
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

	if (!input) {
		return NULL;
	}

	char		*output=new char[strlen(input)*3+1];
	char		*outptr=output;
	const char	*ptr=input;
	
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
	(*outptr)='\0';

	return output;
}

void charstring::leftJustify(char *str, int length) {

	if (!str) {
		return;
	}

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

	if (!str) {
		return;
	}

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

	if (!str) {
		return;
	}

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

	if (!str) {
		return 0;
	}

	int	leadingspaces=0;
	for (int index=0; str[index]==' ' && index<length; index++) {
		leadingspaces++;
	}
	return leadingspaces;
}

int charstring::countTrailingSpaces(char *str, int length) {

	if (!str) {
		return 0;
	}

	int	trailingspaces=0;
	for (int index=length-1; str[index]==' ' && index>-1; index--) {
		trailingspaces++;
	}
	return trailingspaces;
}

char *charstring::parseNumber(short number) {
	return parseNumber(number,0);
}

char *charstring::parseNumber(short number,
				unsigned short zeropadding) {
	int	len=integerLength(number);
	char	*str=new char[((zeropadding>len)?zeropadding:len)+1];
	sprintf(str,"%0*hd",zeropadding,number);
	return str;
}

char *charstring::parseNumber(unsigned short number) {
	return parseNumber(number,0);
}

char *charstring::parseNumber(unsigned short number,
				unsigned short zeropadding) {
	int	len=integerLength(number);
	char	*str=new char[((zeropadding>len)?zeropadding:len)+1];
	sprintf(str,"%0*hd",zeropadding,number);
	return str;
}

char *charstring::parseNumber(long number) {
	return parseNumber(number,0);
}

char *charstring::parseNumber(long number,
				unsigned short zeropadding) {
	int	len=integerLength(number);
	char	*str=new char[((zeropadding>len)?zeropadding:len)+1];
	sprintf(str,"%0*ld",zeropadding,number);
	return str;
}

char *charstring::parseNumber(unsigned long number) {
	return parseNumber(number,0);
}

char *charstring::parseNumber(unsigned long number,
				unsigned short zeropadding) {
	int	len=integerLength(number);
	char	*str=new char[((zeropadding>len)?zeropadding:len)+1];
	sprintf(str,"%0*ld",zeropadding,number);
	return str;
}

char *charstring::parseNumber(long long number) {
	return parseNumber(number,0);
}

char *charstring::parseNumber(long long number,
				unsigned short zeropadding) {
	int	len=integerLength(number);
	char	*str=new char[((zeropadding>len)?zeropadding:len)+1];
	sprintf(str,"%0*lld",zeropadding,number);
	return str;
}

char *charstring::parseNumber(unsigned long long number) {
	return parseNumber(number,0);
}

char *charstring::parseNumber(unsigned long long number,
				unsigned short zeropadding) {
	int	len=integerLength(number);
	char	*str=new char[((zeropadding>len)?zeropadding:len)+1];
	sprintf(str,"%0*lld",zeropadding,number);
	return str;
}

char *charstring::parseNumber(float number) {
	// FIXME: use (q)(e|f|g)cvt(_r)?
	char	*str=new char[22];
	sprintf(str,"%f",number);
	return str;
}

char *charstring::parseNumber(float number,
				unsigned short scale) {
	// FIXME: use (q)(e|f|g)cvt(_r)?
	char	*str=new char[22];
	sprintf(str,"%.*f",scale,number);
	return str;
}

char *charstring::parseNumber(float number,
				unsigned short precision,
				unsigned short scale) {
	// FIXME: use (e|f|g)cvt(_r)?
	char	*str=new char[precision+3];
	sprintf(str,"%*.*f",precision,scale,number);
	return str;
}

char *charstring::parseNumber(double number) {
	// FIXME: use (q)(e|f|g)cvt(_r)?
	char	*str=new char[22];
	sprintf(str,"%f",number);
	return str;
}

char *charstring::parseNumber(double number,
				unsigned short scale) {
	// FIXME: use (q)(e|f|g)cvt(_r)?
	char	*str=new char[22];
	sprintf(str,"%.*f",scale,number);
	return str;
}

char *charstring::parseNumber(double number,
				unsigned short precision,
				unsigned short scale) {
	// FIXME: use (e|f|g)cvt(_r)?
	char	*str=new char[precision+3];
	sprintf(str,"%*.*f",precision,scale,number);
	return str;
}

void charstring::bothTrim(char *string, char character) {
	leftTrim(string,character);
	rightTrim(string,character);
}

int charstring::integerLength(short number) {
	int	length=(number>0)?0:1;
	for (short num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		length++;
	}
	return length;
}

int charstring::integerLength(long number) {
	int	length=(number>0)?0:1;
	for (long num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		length++;
	}
	return length;
}

int charstring::integerLength(long long number) {
	int	length=(number>0)?0:1;
	for (long long num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		length++;
	}
	return length;
}

int charstring::integerLength(unsigned short number) {
	int	length=0;
	for (unsigned short num=number; num>0; num=num/10) {
		length++;
	}
	return length;
}

int charstring::integerLength(unsigned long number) {
	int	length=0;
	for (unsigned long num=number; num>0; num=num/10) {
		length++;
	}
	return length;
}

int charstring::integerLength(unsigned long long number) {
	int	length=0;
	for (unsigned long long num=number; num>0; num=num/10) {
		length++;
	}
	return length;
}

size_t charstring::length(const char *string) {
	return (string)?strlen(string):0;
}

size_t charstring::length(const unsigned char *string) {
	// FIXME: I'd think a static_cast would work here...
	return length(reinterpret_cast<const char *>(string));
}

void charstring::zero(char *str, size_t size) {
	rawbuffer::set(static_cast<void *>(str),0,size);
}

char *charstring::append(char *dest, const char *source) {
	return (dest && source)?strcat(dest,source):NULL;
}

char *charstring::append(char *dest, const char *source, size_t size) {
	return (dest && source)?strncat(dest,source,size):NULL;
}

char *charstring::append(char *dest, long number) {
	char	*str=charstring::parseNumber(number);
	char	*retval=append(dest,str);
	delete[] str;
	return retval;
}

char *charstring::append(char *dest, unsigned long number) {
	char	*str=charstring::parseNumber(number);
	char	*retval=append(dest,str);
	delete[] str;
	return retval;
}

char *charstring::append(char *dest, double number) {
	char	*str=charstring::parseNumber(number);
	char	*retval=append(dest,str);
	delete[] str;
	return retval;
}

char *charstring::append(char *dest, double number, unsigned short scale) {
	char	*str=charstring::parseNumber(number,scale);
	char	*retval=append(dest,str);
	delete[] str;
	return retval;
}

char *charstring::append(char *dest, double number, unsigned short precision,
							unsigned short scale) {
	char	*str=charstring::parseNumber(number,precision,scale);
	char	*retval=append(dest,str);
	delete[] str;
	return retval;
}

char *charstring::copy(char *dest, const char *source) {
	return (dest && source)?strcpy(dest,source):NULL;
}

char *charstring::copy(char *dest, const char *source, size_t size) {
	return (dest && source)?strncpy(dest,source,size):NULL;
}

char *charstring::copy(char *dest, size_t location, const char *source) {
	return (dest && source)?strcpy(dest+location,source):NULL;
}

char *charstring::copy(char *dest, size_t location,
					const char *source, size_t size) {
	return (dest && source)?strncpy(dest+location,source,size):NULL;
}

int charstring::compare(const char *str1, const char *str2) {
	// FIXME: use strcoll?
	return (str1 && str2)?strcmp(str1,str2):(str1!=str2);
}

int charstring::compare(const char *str1, const char *str2, size_t size) {
	return (str1 && str2)?strncmp(str1,str2,size):(str1!=str2);
}

int charstring::compareIgnoringCase(const char *str1, const char *str2) {
	return (str1 && str2)?strcasecmp(str1,str2):(str1!=str2);
}

int charstring::compareIgnoringCase(const char *str1,
						const char *str2, size_t size) {
	return (str1 && str2)?strncasecmp(str1,str2,size):(str1!=str2);
}

bool charstring::contains(const char *haystack, const char *needle) {
	return (findFirst(haystack,needle)!=NULL);
}

bool charstring::contains(const char *haystack, const char needle) {
	return (findFirst(haystack,needle)!=NULL);
}

char *charstring::findFirst(const char *haystack, const char *needle) {
	return (haystack && needle)?strstr(haystack,needle):NULL;
}

char *charstring::findFirst(const char *haystack, const char needle) {
	return (haystack)?strchr(haystack,needle):NULL;
}

char *charstring::findLast(const char *haystack, const char needle) {
	return (haystack)?strrchr(haystack,needle):NULL;
}

// If you've come here chasing valgrind errors...
// strdup() uses malloc(), not new. No doubt some other code has used delete[]
// rather than free() to free the string.  This should be legal.  Is there
// a platform where it isn't?
char *charstring::duplicate(const char *str) {
	return (str)?strdup(str):NULL;
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

long charstring::toLong(const char *string) {
	return (string)?strtol(string,NULL,10):0;
}

long charstring::toLong(const char *string, char **endptr) {
	return (string)?strtol(string,endptr,10):0;
}

long charstring::toLong(const char *string, int base) {
	return (string)?strtol(string,NULL,base):0;
}

long charstring::toLong(const char *string, char **endptr, int base) {
	return (string)?strtol(string,endptr,base):0;
}

unsigned long charstring::toUnsignedLong(const char *string) {
	return (string)?strtoul(string,NULL,10):0;
}

unsigned long charstring::toUnsignedLong(const char *string, char **endptr) {
	return (string)?strtoul(string,endptr,10):0;
}

unsigned long charstring::toUnsignedLong(const char *string, int base) {
	return (string)?strtoul(string,NULL,base):0;
}

unsigned long charstring::toUnsignedLong(const char *string,
						char **endptr, int base) {
	return (string)?strtoul(string,endptr,base):0;
}

long long charstring::toLongLong(const char *string) {
	return (string)?strtoll(string,NULL,10):0;
}

long long charstring::toLongLong(const char *string, char **endptr) {
	return (string)?strtoll(string,endptr,10):0;
}

long long charstring::toLongLong(const char *string, int base) {
	return (string)?strtoll(string,NULL,base):0;
}

long long charstring::toLongLong(const char *string, char **endptr, int base) {
	return (string)?strtoll(string,endptr,base):0;
}

unsigned long long charstring::toUnsignedLongLong(const char *string) {
	return (string)?strtoull(string,NULL,10):0;
}

unsigned long long charstring::toUnsignedLongLong(const char *string,
							char **endptr) {
	return (string)?strtoull(string,endptr,10):0;
}

unsigned long long charstring::toUnsignedLongLong(const char *string,
								int base) {
	return (string)?strtoull(string,NULL,base):0;
}

unsigned long long charstring::toUnsignedLongLong(const char *string,
						char **endptr, int base) {
	return (string)?strtoull(string,endptr,base):0;
}

float charstring::toFloat(const char *string) {
	#ifdef HAVE_STRTOF
	return (string)?strtof(string,NULL):0.0;
	#else
	return (string)?(float)strtod(string,NULL):0.0;
	#endif
}

float charstring::toFloat(const char *string, char **endptr) {
	#ifdef HAVE_STRTOF
	return (string)?strtof(string,endptr):0.0;
	#else
	return (string)?(float)strtod(string,endptr):0.0;
	#endif
}

double charstring::toDouble(const char *string) {
	return (string)?strtod(string,NULL):0.0;
}

double charstring::toDouble(const char *string, char **endptr) {
	return (string)?strtod(string,endptr):0.0;
}

long double charstring::toLongDouble(const char *string) {
	#ifdef HAVE_STRTOLD
	return (string)?strtold(string,NULL):0.0;
	#else
	return (string)?(long double)strtod(string,NULL):0.0;
	#endif
}

long double charstring::toLongDouble(const char *string, char **endptr) {
	#ifdef HAVE_STRTOLD
	return (string)?strtold(string,endptr):0.0;
	#else
	return (string)?(long double)strtod(string,endptr):0.0;
	#endif
}

#ifdef HAVE_STRNDUP
// If you've come here chasing valgrind errors...
// strndup() uses malloc(), not new. No doubt some other code has used delete[]
// rather than free() to free the string.  This should be legal.  Is there
// a platform where it isn't?
char *charstring::duplicate(const char *str, size_t length) {
	return (str)?strndup(str,length):NULL;
}
#else
char *charstring::duplicate(const char *str, size_t length) {
	if (!str) {
		return NULL;
	}
	char	*buffer=new char[length+1];
	copy(buffer,str,length);
	buffer[length]='\0';
	return buffer;
}
#endif

void charstring::safePrint(const char *string, int length) {
	const char	*ch=string;
	for (int i=0; i<length && *ch; i++) {
		character::safePrint(*ch);
		ch++;
	}
}

void charstring::safePrint(const char *string) {
	safePrint(string,charstring::length(string));
}

void charstring::split(const char *string, const char *delimiter,
				char ***list, unsigned long *listlength) {
	split(string,charstring::length(string),
			delimiter,charstring::length(delimiter),
			list,listlength);
}

void charstring::split(const char *string, ssize_t stringlength,
				const char *delimiter,
				char ***list, unsigned long *listlength) {
	split(string,stringlength,
			delimiter,charstring::length(delimiter),
			list,listlength);
}

void charstring::split(const char *string, 
				const char *delimiter, ssize_t delimiterlength,
				char ***list, unsigned long *listlength) {
	split(string,charstring::length(string),
			delimiter,delimiterlength,
			list,listlength);
}

void charstring::split(const char *string, ssize_t stringlength,
				const char *delimiter, ssize_t delimiterlength,
				char ***list, unsigned long *listlength) {

	// handle degenerate cases
	if (!string || !string[0] || !stringlength ||
			!delimiter || !delimiter[0] || !delimiterlength) {
		(*list)=NULL;
		(*listlength)=0;
		return;
	}

	// 2 passes,
	// 1 to count the number of chunks to split the string into,
	// 1 to actually split the string
	for (int pass=0; pass<2; pass++) {

		// set pointers to the beginning and end of the string
		const char	*start=string;
		const char	*end=string+stringlength;

		// initialize the list length
		(*listlength)=0;

		// loop through the string...
		const char	*current=start;
		for (;;) {

			// if there's not enough room left in the string for
			// another delimiter, then move the current position
			// to the end
			if (end-current<delimiterlength) {
				current=end;
			}

			// if we found a delimiter or ran into the end of
			// the string...
			if (current==end ||
				!charstring::compare(current,delimiter,
							delimiterlength)) {

				// handle cases of multiple delimiters in a row
				if (current!=start) {

					// if we're on the second pass...
					if (pass) {

						// make a copy of the string
						// between the last delimiter
						// and here
						(*list)[*listlength]=
							charstring::duplicate(
								start,
								current-start);
					}

					// increment the counter
					(*listlength)++;
				}

				if (current==end) {
					// if we're at the end of the string,
					// then we're done
					break;
				} else {
					// move the current and start pointers
					current=current+delimiterlength;
					start=current;
				}
			} else {
				current++;
			}
		}

		// if we're done with the first pass,
		// create the list and reset the counter
		if (!pass) {
			(*list)=new char *[*listlength];
		}
	}
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
