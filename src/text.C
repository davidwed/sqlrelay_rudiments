// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/text.h>
#ifndef ENABLE_INLINES
	#include <rudiments/private/textinlines.h>
#endif

#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

void text::upper(char *string) {

	int	i=0;
	while (string[i]) {
		if (string[i]>='a' && string[i]<='z') {
			string[i]=string[i]-32;
		}
		i++;
	}
}

void text::lower(char *string) {

	int	i=0;
	while (string[i]) {
		if (string[i]>='A' && string[i]<='Z') {
			string[i]=string[i]+32;
		}
		i++;
	}
}

void text::rightTrim(char *string,char character) {

	if (string && string[0]) {

		int	i=0;

		// advance to the last character in the string
		while (string[i]) {
			i++;
		}
		i--;

		// back up to the first instance of the character to trim
		while (string[i]==character) { 
			i--;
		}
		i++;

		// terminate the string there
		string[i]=(char)NULL;
	}
}

void text::leftTrim(char *string, char character) {

	if (string && string[0]) {

		int	i=0;
		int	j=0;

		// advance past all of the characters we want to replace
		while (string[i]==character) {
			i++;
		}
	
		// swap chars to front of string
		while (string[i]) {
			string[j]=string[i];
			j++;
			i++;
		}
		// store a null to the new end of string
		string[j]=(char)NULL;
	}
}

void text::strip(char *string, char character) {

	int	index=0;
	int	total=0;

	while (string[index]) {
		if (string[index]==character) {
			total++;
		} else {
			if (total) {
				string[index-total]=string[index];
			}
		}
		index++;
	}
	string[index-total]=(char)NULL;
}

void text::strip(char *string1, char *string2) {

	int	str2len=strlen(string2);
	int	index=0;
	int	total=0;

	while (string1[index]) {
		if (!strncmp(string1+index,string2,str2len)) {
			total=total+str2len;
			index=index+str2len;
		} else {
			if (total) {
				string1[index-total]=string1[index];
			}
			index++;
		}
	}
	string1[index-total]=(char)NULL;
}

int text::isInteger(const char *string) {

	char	*ptr=(char *)string;
	while (*ptr) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-') || 
			(ptr>string && *ptr=='-')) {
			return 0;
		}
		ptr++;
	}
	return	1;
}

int text::isNumber(const char *string) {

	int	decimal=0;
	char	*ptr=(char *)string;
	while (*ptr) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-' && *ptr!='.') || 
			(ptr>string && *ptr=='-') || (decimal && *ptr=='.')) {
			return 0;
		}
		if (*ptr=='.') {
			decimal=1;
		}
		ptr++;
	}
	return	1;
}

char *text::httpEscape(const char *input) {

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

void text::leftJustify(char *string, int length) {

	// count leading spaces
	int	spaces=countLeadingSpaces(string,length);

	// replace characters
	int	index;
	int	stop=length-spaces;
	for (index=0; index<stop; index++) {
		string[index]=string[index+spaces];
	}

	// right-pad with spaces
	for (; index<length; index++) {
		string[index]=' ';
	}
}

void text::rightJustify(char *string, int length) {

	// count trailing spaces
	int	spaces=countTrailingSpaces(string,length);

	// replace characters
	int	index;
	int	stop=spaces-1;
	for (index=length-1; index>stop; index--) {
		string[index]=string[index-spaces];
	}

	// right-pad with spaces
	for (; index>-1; index--) {
		string[index]=' ';
	}
}

void text::center(char *string, int length) {

	int	leadingspaces=countLeadingSpaces(string,length);
	int	trailingspaces=countTrailingSpaces(string,length);

	int	leftpad=(leadingspaces+trailingspaces)/2;

	if (leftpad>leadingspaces) {
		// shift everything right
		int	difference=leftpad-leadingspaces;
		int	index;
		for (index=length-1; index>difference-1; index--) {
			string[index]=string[index-difference];
		}
		for (; index>-1; index--) {
			string[index]=' ';
		}
	} else if (leftpad<leadingspaces) {
		// shift everything left
		int	difference=leadingspaces-leftpad;
		int	index;
		for (index=0; index<length-difference; index++) {
			string[index]=string[index+difference];
		}
		for (; index<length; index++) {
			string[index]=' ';
		}
	}
}

int text::countLeadingSpaces(char *string, int length) {
	int	leadingspaces=0;
	for (int index=0; string[index]==' ' && index<length; index++) {
		leadingspaces++;
	}
	return leadingspaces;
}

int text::countTrailingSpaces(char *string, int length) {
	int	trailingspaces=0;
	for (int index=length-1; string[index]==' ' && index>-1; index--) {
		trailingspaces++;
	}
	return trailingspaces;
}

char *text::parseNumber(long number) {
	char	*string=new char[integerLength(number)+1];
	sprintf(string,"%d",number);
	return string;
}

char *text::parseNumber(double number) {
	char	*string=new char[22];
	sprintf(string,"%f",number);
	return string;
}

char *text::parseNumber(double number,
				unsigned short scale) {
	char	*string=new char[22];
	sprintf(string,"%.*f",scale,number);
	return string;
}

char *text::parseNumber(double number,
				unsigned short precision,
				unsigned short scale) {
	char	*string=new char[precision+3];
	sprintf(string,"%*.*f",precision,scale,number);
	return string;
}
