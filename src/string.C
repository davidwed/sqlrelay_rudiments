// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/string.h>
#ifndef ENABLE_INLINES
	#include <rudiments/private/stringinlines.h>
#endif

#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

void string::upper(char *str) {
	for (int i=0; str[i]; i++) {
		if (str[i]>='a' && str[i]<='z') {
			str[i]=str[i]-32;
		}
	}
}

void string::lower(char *str) {
	for (int i=0; str[i]; i++) {
		if (str[i]>='A' && str[i]<='Z') {
			str[i]=str[i]+32;
		}
	}
}

void string::rightTrim(char *str, char character) {

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

void string::leftTrim(char *str, char character) {

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

void string::strip(char *str, char character) {

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

void string::strip(char *str1, char *str2) {

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

int string::isInteger(const char *str) {

	for (char *ptr=(char *)str; *ptr; ptr++) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-') || 
			(ptr>str && *ptr=='-')) {
			return 0;
		}
	}
	return	1;
}

int string::isNumber(const char *str) {

	int	decimal=0;
	for (char *ptr=(char *)str; *ptr; ptr++) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-' && *ptr!='.') || 
			(ptr>str && *ptr=='-') || (decimal && *ptr=='.')) {
			return 0;
		}
		if (*ptr=='.') {
			decimal=1;
		}
	}
	return	1;
}

char *string::httpEscape(const char *input) {

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

void string::leftJustify(char *str, int length) {

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

void string::rightJustify(char *str, int length) {

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

void string::center(char *str, int length) {

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

int string::countLeadingSpaces(char *str, int length) {
	int	leadingspaces=0;
	for (int index=0; str[index]==' ' && index<length; index++) {
		leadingspaces++;
	}
	return leadingspaces;
}

int string::countTrailingSpaces(char *str, int length) {
	int	trailingspaces=0;
	for (int index=length-1; str[index]==' ' && index>-1; index--) {
		trailingspaces++;
	}
	return trailingspaces;
}

char *string::parseNumber(long number) {
	char	*str=new char[integerLength(number)+1];
	sprintf(str,"%ld",number);
	return str;
}

char *string::parseNumber(double number) {
	char	*str=new char[22];
	sprintf(str,"%f",number);
	return str;
}

char *string::parseNumber(double number,
				unsigned short scale) {
	char	*str=new char[22];
	sprintf(str,"%.*f",scale,number);
	return str;
}

char *string::parseNumber(double number,
				unsigned short precision,
				unsigned short scale) {
	char	*str=new char[precision+3];
	sprintf(str,"%*.*f",precision,scale,number);
	return str;
}
