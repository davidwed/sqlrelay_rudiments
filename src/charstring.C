// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

size_t charstring::length(const char *string) {
	return strlen(string);
}

void charstring::zero(char *str, size_t size) {
	memset((void *)str,0,size);
}

char *charstring::append(char *dest, const char *source) {
	return strcat(dest,source);
}

char *charstring::append(char *dest, const char *source, size_t size) {
	return strncat(dest,source,size);
}

char *charstring::copy(char *dest, const char *source) {
	return strcpy(dest,source);
}

char *charstring::copy(char *dest, const char *source, size_t size) {
	return strncpy(dest,source,size);
}

char *charstring::copy(char *dest, size_t location, const char *source) {
	return strcpy(dest+location,source);
}

char *charstring::copy(char *dest, size_t location,
					const char *source, size_t size) {
	return strncpy(dest+location,source,size);
}

int charstring::compare(const char *str1, const char *str2) {
	// FIXME: use strcoll?
	return strcmp(str1,str2);
}

int charstring::compare(const char *str1, const char *str2, size_t size) {
	return strncmp(str1,str2,size);
}

int charstring::compareIgnoringCase(const char *str1, const char *str2) {
	// FIXME: use strcasecmp_l?
	return strcasecmp(str1,str2);
}

int charstring::compareIgnoringCase(const char *str1,
						const char *str2, size_t size) {
	// FIXME: use strncasecmp_l?
	return strncasecmp(str1,str2,size);
}

bool charstring::contains(const char *haystack, const char *needle) {
	return (findFirst(haystack,needle)!=NULL);
}

bool charstring::contains(const char *haystack, const char needle) {
	return (findFirst(haystack,needle)!=NULL);
}

char *charstring::findFirst(const char *haystack, const char *needle) {
	return strstr(haystack,needle);
}

char *charstring::findFirst(const char *haystack, const char needle) {
	return strchr(haystack,needle);
}

char *charstring::findLast(const char *haystack, const char *needle) {

	size_t	haystacklen=length(haystack);
	size_t	needlelen=length(needle);
	if (needlelen>haystacklen) {
		return NULL;
	}
	
	char	*ptr=(char *)haystack+haystacklen-needlelen;
	while (ptr>haystack) {
		if (!compare(ptr,needle)) {
			return ptr;
		}
		ptr--;
	}
	return NULL;
}

char *charstring::findLast(const char *haystack, const char needle) {
	return strrchr(haystack,needle);
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

long charstring::toLong(const char *string) {
	return strtol(string,NULL,0);
}

long charstring::toLong(const char *string, char **endptr) {
	return strtol(string,endptr,0);
}

long charstring::toLong(const char *string, int base) {
	return strtol(string,NULL,base);
}

long charstring::toLong(const char *string, char **endptr, int base) {
	return strtol(string,endptr,base);
}

double charstring::toDouble(const char *string) {
	return strtod(string,NULL);
}

double charstring::toDouble(const char *string, char **endptr) {
	return strtod(string,endptr);
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
