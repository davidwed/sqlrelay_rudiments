// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/character.h>
#if !defined(RUDIMENTS_HAVE_VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE___VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF)
	#include <rudiments/process.h>
	#include <rudiments/file.h>
#endif

// for strtold and for strchrnul
#ifndef __USE_GNU
	#define __USE_GNU
#endif
#ifndef __USE_ISOC9X
	#define __USE_ISOC9X
#endif

// Redhat 6.2 needs _GNU_SOURCE
#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif

// include this after the above defines or Haiku can't find strchrnul
#include <rudiments/stdio.h>

// for vsnprintf
#include <stdio.h>

#ifdef RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF
int __vsnprintf(char *str, size_t size, const char *format, va_list ap);
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_STRING_H
	#include <string.h>
#endif
#ifdef RUDIMENTS_HAVE_STRINGS_H
	#include <strings.h>
#endif

const char *charstring::findLast(const char *haystack, const char *needle) {

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
		if (!compare(ptr,needle,needlelen)) {
			return ptr;
		}
		ptr--;
	}
	return NULL;
}

void charstring::upper(char *str) {

	if (!str) {
		return;
	}

	for (int32_t i=0; str[i]; i++) {
		if (str[i]>='a' && str[i]<='z') {
			str[i]=str[i]-32;
		}
	}
}

void charstring::lower(char *str) {

	if (!str) {
		return;
	}

	for (int32_t i=0; str[i]; i++) {
		if (str[i]>='A' && str[i]<='Z') {
			str[i]=str[i]+32;
		}
	}
}

void charstring::capitalize(char *str) {

	bool	cap=true;
	for (char *ch=str; *ch; ch++) {
		char	c=*ch;
		if (cap) {
			*ch=character::toUpperCase(c);
			cap=false;
		} else {
			*ch=character::toLowerCase(c);
		}
		if (c==' ' || c=='\'' || c=='"' || c=='-' || c=='(') {
			cap=true;
		}
	}
}

void charstring::rightTrim(char *str, char character) {

	if (str && str[0]) {

		int32_t	i=0;

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

		int32_t	i=0;
		int32_t	j=0;

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

bool charstring::strip(char *str, char character) {

	if (!str) {
		return false;
	}

	int32_t	index=0;
	int32_t	total=0;
	bool	retval=false;

	while (str[index]) {
		if (str[index]==character) {
			total++;
			retval=true;
		} else {
			if (total) {
				str[index-total]=str[index];
			}
		}
		index++;
	}
	str[index-total]='\0';
	return retval;
}

bool charstring::strip(char *str1, const char *str2) {

	if (!str1 || !str2) {
		return false;
	}

	int32_t	str2len=length(str2);
	int32_t	index=0;
	int32_t	total=0;
	bool	retval=false;

	while (str1[index]) {
		if (!strncmp(str1+index,str2,str2len)) {
			total=total+str2len;
			index=index+str2len;
			retval=true;
		} else {
			if (total) {
				str1[index-total]=str1[index];
			}
			index++;
		}
	}
	str1[index-total]='\0';
	return retval;
}

bool charstring::stripSet(char *str, const char *set) {

	if (!str) {
		return false;
	}

	int32_t	index=0;
	int32_t	total=0;
	bool	retval=false;

	while (str[index]) {
		if (character::inSet(str[index],set)) {
			total++;
			retval=true;
		} else {
			if (total) {
				str[index-total]=str[index];
			}
		}
		index++;
	}
	str[index-total]='\0';
	return retval;
}

void charstring::replace(char *str, char oldchar, char newchar) {
	for (char *ptr=str; *ptr; ptr++) {
		if (*ptr==oldchar) {
			*ptr=newchar;
		}
	}
}

void charstring::replace(char *str, const char *oldchars, char newchar) {
	for (char *ptr=str; *ptr; ptr++) {
		if (character::inSet(*ptr,oldchars)) {
			*ptr=newchar;
		}
	}
}

bool charstring::isInteger(const char *str) {

	if (!str) {
		return false;
	}

	const char	*ptr=str;
	for (; *ptr; ptr++) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-') || 
			(ptr>str && *ptr=='-')) {
			return false;
		}
	}

	// if the string ended in a - then it's not a number
	return (*(ptr-1)!='-');
}

bool charstring::isInteger(const char *str, int32_t size) {

	if (!str) {
		return false;
	}

	const char	*ptr=str;
	for (int32_t index=0; index<size; index++) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-') || 
			(ptr>str && *ptr=='-')) {
			return false;
		}
		ptr++;
	}

	// if the string ended in a - then it's not a number
	return (*(ptr-1)!='-');
}

bool charstring::isNumber(const char *str) {

	if (!str) {
		return false;
	}

	const char	*ptr=str;
	int32_t		decimal=0;
	for (; *ptr; ptr++) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-' && *ptr!='.') || 
			(ptr>str && *ptr=='-') || (decimal && *ptr=='.')) {
			return false;
		}
		if (*ptr=='.') {
			decimal=1;
		}
	}

	// if the string ended in a - or . then it's not a number
	return (*(ptr-1)!='-' && *(ptr-1)!='.');
}

bool charstring::isNumber(const char *str, int32_t size) {

	if (!str) {
		return false;
	}

	const char	*ptr=str;
	int32_t		decimal=0;
	for (int32_t index=0; index<size; index++) {
		if (((*ptr>'9' || *ptr<'0') && *ptr!='-' && *ptr!='.') || 
			(ptr>str && *ptr=='-') || (decimal && *ptr=='.')) {
			return false;
		}
		if (*ptr=='.') {
			decimal=1;
		}
		ptr++;
	}

	// if the string ended in a - or . then it's not a number
	return (*(ptr-1)!='-' && *(ptr-1)!='.');
}

int64_t charstring::convertAmount(const char *amount) {
	if (!amount) {
		return 0;
	}
	const char	*dollarsstr=findFirst(amount,'$');
	dollarsstr=(dollarsstr)?dollarsstr+1:amount;
	uint64_t	dollars=toUnsignedInteger(dollarsstr);
	const char	*centsstr=findFirst(amount,'.');
	uint64_t	cents=(centsstr)?toUnsignedInteger(centsstr+1):0;
	return (dollars*100+cents);
}

char *charstring::convertAmount(int64_t amount) {
	uint16_t	length=integerLength(amount)+4;
	if (length<6) {
		length=6;
	}
	char	negative[2];
	if (amount<0) {
		negative[0]='-';
	} else {
		negative[0]='\0';
	}
	negative[1]='\0';
	char	*amountstr=new char[length];
	int64_t	amt=abs(amount);
	printf(amountstr,length,
			"$%s%lld.%02lld",negative,
			(long long)(amt/100),
			(long long)(amt-(amt/100*100)));
	return amountstr;
}

char *charstring::convertAmount(int64_t amount, uint16_t spaces) {
	char	*amt=convertAmount(amount);
	ssize_t	amtlen=length(amt+1);
	uint16_t	realspaces=(amtlen+1>spaces)?amtlen+1:spaces;
	char	*buffer=new char[realspaces+1];
	buffer[realspaces]='\0';
	rawbuffer::set(buffer,' ',realspaces);
	rawbuffer::copy(buffer+realspaces-amtlen,amt+1,amtlen);
	if (buffer[0]==' ') {
		buffer[0]='$';
	}
	delete[] amt;
	return buffer;
}

char *charstring::httpEscape(const char *input) {

	if (!input) {
		return NULL;
	}

	size_t		outputlen=length(input)*3+1;
	char		*output=new char[outputlen];
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
			(*outptr)='%';
			outptr++;
			outputlen--;
			char	upper=(*ptr)>>4;
			if (upper<9) {
				*outptr=upper+'0';
			} else {
				*outptr=upper-10+'A';
			}
			outptr++;
			outputlen--;
			char	lower=(*ptr)&0x0F;
			if (lower<9) {
				*outptr=lower+'0';
			} else {
				*outptr=lower-10+'A';
			}
		}
		outptr++;
		outputlen--;
		ptr++;
	}
	(*outptr)='\0';

	return output;
}

char *charstring::httpUnescape(const char *input) {

	if (!input) {
		return NULL;
	}

	char		*output=new char[length(input)+1];
	char		*outptr=output;
	const char	*ptr=input;
	
	while (*ptr) {
		if (*ptr=='+') {
			(*outptr)=' ';
		} else if (*ptr=='%') {
			ptr++;
			char	hex[5];
			hex[0]='0';
			hex[2]='x';
			if (*ptr) {
				hex[0]=*ptr;
				ptr++;
				if (*ptr) {
					hex[1]=*ptr;
				} else {
					(*outptr)='%';
					outptr++;
					(*outptr)=*ptr;
					outptr++;
					break;
				}
				hex[2]='\0';
				char	ch=charstring::toInteger(hex,16);
				(*outptr)=ch;
			} else {
				(*outptr)='%';
				outptr++;
				break;
			}
		} else {
			(*outptr)=*ptr;
		}
		outptr++;
		ptr++;
	}
	(*outptr)='\0';

	return output;
}

char *charstring::escape(const char *input, const char *characters) {
	char		*output;
	uint64_t	outputsize;
	escape(input,length(input),&output,&outputsize,characters);
	return output;
}

void charstring::escape(const char *input, uint64_t inputsize,
				char **output, uint64_t *outputsize,
						const char *characters) {

	(*output)=NULL;
	(*outputsize)=0;

	if (!input) {
		return;
	}

	for (uint16_t pass=0; pass<2; pass++) {

		uint64_t	outputindex=0;
		for (uint64_t inputindex=0;
				inputindex<inputsize;
				inputindex++) {
			if (charstring::contains(characters,
						input[inputindex]) ||
					input[inputindex]=='\\') {
				if (pass==0) {
					(*outputsize)++;
				} else {
					(*output)[outputindex]='\\';
					outputindex++;
				}
			}
			if (pass==0) {
				(*outputsize)++;
			} else {
				(*output)[outputindex]=input[inputindex];
			}
			outputindex++;
		}
		if (pass==0) {
			(*output)=new char[(*outputsize)+1];
			(*output)[(*outputsize)]='\0';
		}
	}
}

char *charstring::unescape(const char *input) {
	char		*output;
	uint64_t	outputsize;
	unescape(input,length(input),&output,&outputsize);
	return output;
}

void charstring::unescape(const char *input, uint64_t inputsize,
				char **output, uint64_t *outputsize) {

	(*output)=NULL;
	(*outputsize)=0;

	if (!input) {
		return;
	}

	for (uint16_t pass=0; pass<2; pass++) {

		bool		escaped=false;
		uint64_t	outputindex=0;
		for (uint64_t inputindex=0;
				inputindex<inputsize;
				inputindex++) {
			if (!escaped && input[inputindex]=='\\') {
				escaped=true;
				continue;
			}
			if (pass==0) {
				(*outputsize)++;
			} else {
				(*output)[outputindex]=input[inputindex];
			}
			outputindex++;
			escaped=false;
		}
		if (pass==0) {
			(*output)=new char[(*outputsize)+1];
			(*output)[(*outputsize)]='\0';
		}
	}
}

void charstring::leftJustify(char *str, int32_t length) {

	if (!str) {
		return;
	}

	// count leading spaces
	int32_t	spaces=countLeadingSpaces(str,length);

	// replace characters
	int32_t	index;
	int32_t	stop=length-spaces;
	for (index=0; index<stop; index++) {
		str[index]=str[index+spaces];
	}

	// right-pad with spaces
	for (; index<length; index++) {
		str[index]=' ';
	}
}


void charstring::rightPad(char *str, int32_t lngth, char padchar, bool fill) {

	if (!str) {
		return;
	}
	
	if (fill) {
		int32_t valuelength=length(str);
		for(; valuelength<lngth; valuelength++){
			str[valuelength]=' ';
		}
		str[valuelength]='\0';
	}

	// count trailing spaces
	int32_t	spaces=countTrailingSpaces(str,lngth);
	if (spaces==0){
		return;
	}

	// replace characters
	int32_t	index;
	int32_t	stop=spaces-1;
	for (index=lngth-1; index>stop; index--) {
		str[index]=str[index-spaces];
	}

	// right-pad with spaces
	for (; index>-1; index--) {
		str[index]=padchar;
	}
}

void charstring::rightJustify(char *str, int32_t length) {

	rightPad(str,length,' ',false);
}

void charstring::center(char *str, int32_t length) {

	if (!str) {
		return;
	}

	int32_t	leadingspaces=countLeadingSpaces(str,length);
	int32_t	trailingspaces=countTrailingSpaces(str,length);

	int32_t	leftpad=(leadingspaces+trailingspaces)/2;

	if (leftpad>leadingspaces) {
		// shift everything right
		int32_t	difference=leftpad-leadingspaces;
		int32_t	index;
		for (index=length-1; index>difference-1; index--) {
			str[index]=str[index-difference];
		}
		for (; index>-1; index--) {
			str[index]=' ';
		}
	} else if (leftpad<leadingspaces) {
		// shift everything left
		int32_t	difference=leadingspaces-leftpad;
		int32_t	index;
		for (index=0; index<length-difference; index++) {
			str[index]=str[index+difference];
		}
		for (; index<length; index++) {
			str[index]=' ';
		}
	}
}

int32_t charstring::countLeadingSpaces(const char *str, int32_t length) {

	if (!str) {
		return 0;
	}

	int32_t	leadingspaces=0;
	for (int32_t index=0; str[index]==' ' && index<length; index++) {
		leadingspaces++;
	}
	return leadingspaces;
}

int32_t charstring::countTrailingSpaces(const char *str, int32_t length) {

	if (!str) {
		return 0;
	}

	int32_t	trailingspaces=0;
	for (int32_t index=length-1; str[index]==' ' && index>-1; index--) {
		trailingspaces++;
	}
	return trailingspaces;
}

char *charstring::parseNumber(int16_t number) {
	return parseNumber((int64_t)number,1);
}

char *charstring::parseNumber(int16_t number,
				uint16_t zeropadding) {
	return parseNumber((int64_t)number,zeropadding);
}

char *charstring::parseNumber(uint16_t number) {
	return parseNumber((uint64_t)number,1);
}

char *charstring::parseNumber(uint16_t number, uint16_t zeropadding) {
	return parseNumber((uint64_t)number,zeropadding);
}

char *charstring::parseNumber(int32_t number) {
	return parseNumber((int64_t)number,1);
}

char *charstring::parseNumber(int32_t number, uint16_t zeropadding) {
	return parseNumber((int64_t)number,zeropadding);
}

char *charstring::parseNumber(uint32_t number) {
	return parseNumber((uint64_t)number,1);
}

char *charstring::parseNumber(uint32_t number, uint16_t zeropadding) {
	return parseNumber((uint64_t)number,zeropadding);
}

char *charstring::parseNumber(int64_t number) {
	return parseNumber(number,1);
}

char *charstring::parseNumber(int64_t number, uint16_t zeropadding) {
	uint16_t	len=integerLength(number);
	uint16_t	strlength=((zeropadding>len)?zeropadding:len)+1;
	char	*str=new char[strlength];
	int32_t	strindex=strlength-1;
	str[strindex--]='\0';
	int64_t	posnumber=(number>=0)?number:-1*number;
	while (posnumber) {
		str[strindex--]='0'+posnumber%10;
		posnumber/=10;
	}
	int32_t	finish=(number>=0)?-1:0;
	while (strindex>finish) {
		str[strindex--]='0';
	}
	if (number<0) {
		str[strindex]='-';
	}
	return str;
}

char *charstring::parseNumber(uint64_t number) {
	return parseNumber(number,1);
}

char *charstring::parseNumber(uint64_t number, uint16_t zeropadding) {
	uint16_t	len=integerLength(number);
	uint16_t	strlength=((zeropadding>len)?zeropadding:len)+1;
	char	*str=new char[strlength];
	int32_t	strindex=strlength-1;
	str[strindex--]='\0';
	while (number) {
		str[strindex--]='0'+number%10;
		number/=10;
	}
	while (strindex>-1) {
		str[strindex--]='0';
	}
	return str;
}

char *charstring::parseNumber(float number) {
	return parseNumber((long double)number);
}

char *charstring::parseNumber(float number, uint16_t scale) {
	return parseNumber((long double)number,scale);
}

char *charstring::parseNumber(float number,
				uint16_t precision, uint16_t scale) {
	return parseNumber((long double)number,precision,scale);
}

char *charstring::parseNumber(double number) {
	return parseNumber((long double)number);
}

char *charstring::parseNumber(double number,
				uint16_t scale) {
	return parseNumber((long double)number,scale);
}

char *charstring::parseNumber(double number,
				uint16_t precision, uint16_t scale) {
	return parseNumber((long double)number,precision,scale);
}

char *charstring::parseNumber(long double number) {
	char	*str=new char[22];
	printf(str,22,"%Lf",number);
	return str;
}

char *charstring::parseNumber(long double number, uint16_t scale) {
	char	*str=new char[22];
	printf(str,22,"%.*Lf",scale,number);
	return str;
}

char *charstring::parseNumber(long double number,
				uint16_t precision, uint16_t scale) {
	size_t	strlength=precision+3;
	char	*str=new char[strlength];
	printf(str,strlength,"%*.*Lf",precision,scale,number);
	return str;
}

void charstring::bothTrim(char *string, char character) {
	leftTrim(string,character);
	rightTrim(string,character);
}

uint16_t charstring::integerLength(int16_t number) {
	uint16_t	length=(number>0)?0:1;
	for (int16_t num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		length++;
	}
	return length;
}

uint16_t charstring::integerLength(int32_t number) {
	uint16_t	length=(number>0)?0:1;
	for (int32_t num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		length++;
	}
	return length;
}

uint16_t charstring::integerLength(int64_t number) {
	uint16_t	length=(number>0)?0:1;
	for (int64_t num=((number>0)?number:(-1*number)); num>0; num=num/10) {
		length++;
	}
	return length;
}

uint16_t charstring::integerLength(uint16_t number) {
	uint16_t	length=(number>0)?0:1;
	for (uint16_t num=number; num>0; num=num/10) {
		length++;
	}
	return length;
}

uint16_t charstring::integerLength(uint32_t number) {
	uint16_t	length=(number>0)?0:1;
	for (uint32_t num=number; num>0; num=num/10) {
		length++;
	}
	return length;
}

uint16_t charstring::integerLength(uint64_t number) {
	uint16_t	length=(number>0)?0:1;
	for (uint64_t num=number; num>0; num=num/10) {
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
	rawbuffer::set(str,0,size);
}

char *charstring::append(char *dest, const char *source) {
	return append(dest,source,length(source)+1);
}

char *charstring::append(char *dest, const char *source, size_t size) {
	return copy(dest+length(dest),source,size);
}

char *charstring::append(char *dest, int64_t number) {
	char	*str=charstring::parseNumber(number);
	char	*retval=append(dest,str);
	delete[] str;
	return retval;
}

char *charstring::append(char *dest, uint64_t number) {
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

char *charstring::append(char *dest, double number, uint16_t scale) {
	char	*str=charstring::parseNumber(number,scale);
	char	*retval=append(dest,str);
	delete[] str;
	return retval;
}

char *charstring::append(char *dest, double number, uint16_t precision,
							uint16_t scale) {
	char	*str=charstring::parseNumber(number,precision,scale);
	char	*retval=append(dest,str);
	delete[] str;
	return retval;
}

char *charstring::copy(char *dest, const char *source) {
	return copy(dest,source,length(source)+1);
}

char *charstring::copy(char *dest, const char *source, size_t size) {
	if (!dest || !source) {
		return dest;
	}
	return strncpy(dest,source,size);
}

char *charstring::copy(char *dest, size_t location, const char *source) {
	return copy(dest+location,source);
}

char *charstring::copy(char *dest, size_t location,
					const char *source, size_t size) {
	return copy(dest+location,source,size);
}

char *charstring::safeCopy(char *dest, size_t destsize, const char *source) {
	return safeCopy(dest,destsize,source,length(source)+1);
}

char *charstring::safeCopy(char *dest, size_t destsize,
				const char *source, size_t sourcesize) {
	return copy(dest,source,(sourcesize>destsize)?destsize:sourcesize);
}

int32_t charstring::compare(const char *str1, const char *str2) {
	// FIXME: use strcoll?
	return (str1 && str2)?strcmp(str1,str2):(str1!=str2);
}

int32_t charstring::compare(const char *str1, const char *str2, size_t size) {
	return (str1 && str2)?strncmp(str1,str2,size):(str1!=str2);
}

int32_t charstring::compareIgnoringCase(const char *str1, const char *str2) {
	#ifdef RUDIMENTS_HAVE_STRCASECMP
		return (str1 && str2)?strcasecmp(str1,str2):(str1!=str2);
	#else
		const char	*ptr1=str1;
		const char	*ptr2=str2;
		int32_t		diff=0;
		while (*ptr1 && *ptr2) {
			diff=character::toUpperCase(*ptr1)-
				character::toUpperCase(*ptr2);
			if (diff) {
				return diff;
			}
			ptr1++;
			ptr2++;
		}
		return diff;
	#endif
}

int32_t charstring::compareIgnoringCase(const char *str1,
						const char *str2, size_t size) {
	#ifdef RUDIMENTS_HAVE_STRNCASECMP
		return (str1 && str2)?strncasecmp(str1,str2,size):(str1!=str2);
	#else
		size_t		count=0;
		const char	*ptr1=str1;
		const char	*ptr2=str2;
		int32_t		diff=0;
		while (*ptr1 && *ptr2 && count<size) {
			diff=character::toUpperCase(*ptr1)-
				character::toUpperCase(*ptr2);
			if (diff) {
				return diff;
			}
			ptr1++;
			ptr2++;
			count++;
		}
		return diff;
	#endif
}

bool charstring::inSet(const char *str, const char * const *set) {
	if (!set || !set[0]) {
		return !str;
	}
	for (const char * const *s=set; *s; s++) {
		if (!compare(str,*s)) {
			return true;
		}
	}
	return false;
}

bool charstring::inSetIgnoringCase(const char *str, const char * const *set) {
	if (!set || !set[0]) {
		return !str;
	}
	for (const char * const *s=set; *s; s++) {
		if (!compareIgnoringCase(str,*s)) {
			return true;
		}
	}
	return false;
}

bool charstring::contains(const char *haystack, const char *needle) {
	return (findFirst(haystack,needle)!=NULL);
}

bool charstring::contains(const char *haystack, char needle) {
	return (findFirst(haystack,needle)!=NULL);
}

const char *charstring::findFirst(const char *haystack, const char *needle) {
	return (haystack && needle)?strstr(haystack,needle):NULL;
}

const char *charstring::findFirst(const char *haystack, char needle) {
	return (haystack)?strchr(haystack,needle):NULL;
}

const char *charstring::findFirstOrEnd(const char *haystack, char needle) {
	#ifdef RUDIMENTS_HAVE_STRCHRNUL
	return (haystack)?strchrnul(haystack,needle):NULL;
	#else
	if (!haystack) {
		return NULL;
	}
	const char	*retval=haystack;
	while (*retval) {
		if (*retval==needle) {
			return retval;
		}
		retval++;
	}
	return retval;
	#endif
}

const char *charstring::findFirstOrEnd(const char *haystack,
						const char *needle) {
	if (!haystack || !needle) {
		return NULL;
	}
	const char	*retval=haystack;
	size_t	needlelen=length(needle);
	while (*retval) {
		if (!compare(retval,needle,needlelen)) {
			return retval;
		}
		retval++;
	}
	return retval;
}

char *charstring::findFirstOrEnd(char *haystack, const char *needle) {
	return const_cast<char *>(findFirstOrEnd(
					const_cast<const char *>(haystack),
					needle));
}

const char *charstring::findLast(const char *haystack, char needle) {
	return (haystack)?strrchr(haystack,needle):NULL;
}

char *charstring::findFirst(char *haystack, const char *needle) {
	return const_cast<char *>(findFirst(
					const_cast<const char *>(haystack),
					needle));
}

char *charstring::findFirst(char *haystack, char needle) {
	return const_cast<char *>(findFirst(
					const_cast<const char *>(haystack),
					needle));
}

char *charstring::findLast(char *haystack, const char *needle) {
	return const_cast<char *>(findLast(
					const_cast<const char *>(haystack),
					needle));
}

char *charstring::findLast(char *haystack, char needle) {
	return const_cast<char *>(findLast(
					const_cast<const char *>(haystack),
					needle));
}

const char *charstring::findFirstOfSet(const char *haystack, const char *set) {
	return const_cast<char *>(findFirstOfSet(
					const_cast<char *>(haystack),set));
}

char *charstring::findFirstOfSet(char *haystack, const char *set) {
	#ifdef RUDIMENTS_HAVE_STRPBRK
	return (haystack && set)?strpbrk(haystack,set):NULL;
	#else
	if (!haystack || !set) {
		return NULL;
	}
	char	*retval=haystack;
	while (*retval) {
		if (contains(set,*retval)) {
			return retval;
		}
		retval++;
	}
	return NULL;
	#endif
}

const char *charstring::findFirstOfSetOrEnd(const char *haystack,
							const char *set) {
	return const_cast<char *>(findFirstOfSetOrEnd(
					const_cast<char *>(haystack),set));
}

char *charstring::findFirstOfSetOrEnd(char *haystack, const char *set) {
	if (!haystack || !set) {
		return NULL;
	}
	char	*retval=haystack;
	while (*retval) {
		if (contains(set,*retval)) {
			return retval;
		}
		retval++;
	}
	return retval;
}

size_t charstring::lengthContainingSet(const char *haystack, const char *set) {
	#ifdef RUDIMENTS_HAVE_STRSPN
	return (haystack && set)?strspn(haystack,set):0;
	#else
	if (!haystack || !set) {
		return 0;
	}
	size_t	index=0;
	while (contains(set,haystack[index])) {
		index++;
	}
	return index;
	#endif
}

size_t charstring::lengthNotContainingSet(const char *haystack,
						const char *set) {
	#ifdef RUDIMENTS_HAVE_STRCSPN
	return (haystack && set)?strcspn(haystack,set):0;
	#else
	if (!haystack || !set) {
		return 0;
	}
	size_t	index=0;
	while (!contains(set,haystack[index])) {
		index++;
	}
	return index;
	#endif
}

char *charstring::duplicate(const char *str) {
	if (!str) {
		return NULL;
	}
	return duplicate(str,length(str));
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

int64_t charstring::toInteger(const char *string) {
	return toInteger(string,NULL,10);
}

int64_t charstring::toInteger(const char *string, char **endptr) {
	return toInteger(string,endptr,10);
}

int64_t charstring::toInteger(const char *string, int32_t base) {
	return toInteger(string,NULL,base);
}

int64_t charstring::toInteger(const char *string, char **endptr, int32_t base) {
	#ifdef RUDIMENTS_HAVE_STRTOLL
	return (string)?strtoll(string,endptr,base):0;
	#else
	return (string)?strtol(string,endptr,base):0;
	#endif
}

uint64_t charstring::toUnsignedInteger(const char *string) {
	return toUnsignedInteger(string,NULL,10);
}

uint64_t charstring::toUnsignedInteger(const char *string, char **endptr) {
	return toUnsignedInteger(string,endptr,10);
}

uint64_t charstring::toUnsignedInteger(const char *string, int32_t base) {
	return toUnsignedInteger(string,NULL,base);
}

uint64_t charstring::toUnsignedInteger(const char *string,
						char **endptr, int32_t base) {
	#ifdef RUDIMENTS_HAVE_STRTOULL
	return (string)?strtoull(string,endptr,base):0;
	#else
	return (string)?strtoul(string,endptr,base):0;
	#endif
}

long double charstring::toFloat(const char *string) {
	return toFloat(string,NULL);
}

long double charstring::toFloat(const char *string, char **endptr) {
	#ifdef RUDIMENTS_HAVE_STRTOLD
	return (string)?strtold(string,endptr):0.0;
	#else
	return (string)?static_cast<long double>(strtod(string,endptr)):0.0;
	#endif
}

char *charstring::duplicate(const char *str, size_t length) {
	if (!str) {
		return NULL;
	}
	char	*buffer=new char[length+1];
	copy(buffer,str,length);
	buffer[length]='\0';
	return buffer;
}

void charstring::split(const char *string, const char *delimiter,
				bool collapse,
				char ***list, uint64_t *listlength) {
	split(string,length(string),
			delimiter,length(delimiter),
			collapse,list,listlength);
}

void charstring::split(const char *string, ssize_t stringlength,
				const char *delimiter, bool collapse,
				char ***list, uint64_t *listlength) {
	split(string,stringlength,
			delimiter,length(delimiter),
			collapse,list,listlength);
}

void charstring::split(const char *string, 
				const char *delimiter, ssize_t delimiterlength,
				bool collapse,
				char ***list, uint64_t *listlength) {
	split(string,length(string),
			delimiter,delimiterlength,
			collapse,list,listlength);
}

void charstring::split(const char *string, ssize_t stringlength,
				const char *delimiter, ssize_t delimiterlength,
				bool collapse,
				char ***list, uint64_t *listlength) {

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
	for (int32_t pass=0; pass<2; pass++) {

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
				if (current!=start || !collapse) {

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

char *charstring::subString(const char *str, size_t start, size_t end) {

	// handle end<start
	if (end<start) {
		size_t	temp=end;
		end=start;
		start=temp;
	}
	return duplicate(str+start,end-start+1);
}

char *charstring::subString(const char *str, size_t start) {
	return subString(str,start,length(str)-1);
}

char *charstring::base64Encode(const unsigned char *input) {
	return base64Encode(input,length(input));
}

char *charstring::base64Encode(const unsigned char *input, uint64_t inputsize) {
	char		*retval=NULL;
	uint64_t	retvalsize=0;
	base64Encode(input,inputsize,&retval,&retvalsize);
	return retval;
}

static char	b64code[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz0123456789+/";

// Since we're setting a lot of the elements of this array to -1, it must
// explicitly be set to type signed char for platforms like ARM where char
// (without the signed qualifier) are actually unsigned.
static signed char	b64dcode[]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	// 0-9
					-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	// 10-19
					-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,	// etc.
					-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
					-1,-1,-1,62,-1,-1,-1,63,52,53,
					54,55,56,57,58,59,60,61,-1,-1,
					-1,0,-1,-1,-1,0,1,2,3,4,
					5,6,7,8,9,10,11,12,13,14,
					15,16,17,18,19,20,21,22,23,24,
					25,-1,-1,-1,-1,-1,-1,26,27,28,
					29,30,31,32,33,34,35,36,37,38,
					39,40,41,42,43,44,45,46,47,48,
					49,50,51,-1,-1,-1,-1,-1};

void charstring::base64Encode(const unsigned char *input, uint64_t inputsize,
					char **output, uint64_t *outputsize) {

	// handle null input
	if (!input) {
		*output=NULL;
		*outputsize=0;
		return;
	}

	// handle 0-length input
	if (!inputsize) {
		*output=charstring::duplicate("");
		*outputsize=0;
		return;
	}

	// handle real input...

	// figure out the output size:
	//
	// let x = inputsize
	// let y = outputsize
	//
	// (I know this sequence is true by
	//  observation of input vs output sizes)
	// x   y
	// -----
	// 9   12 = (x*4/3)+0
	// 8   12 = (x*4/3)+2
	// 7   12 = (x*4/3)+3
	// 6   8  = (x*4/3)+0
	// 5   8  = (x*4/3)+2
	// 4   8  = (x*4/3)+3
	// 3   4  = (x*4/3)+0
	// 2   4  = (x*4/3)+2
	// 1   4  = (x*4/3)+3
	//
	// y=((x*4/3)+z)
	//
	// x   z
	// -----
	// 9   0 = 9-x+0
	// 8   2 = 9-x+1
	// 7   3 = 9-x+1
	// 6   0 = 6-x+0
	// 5   2 = 6-x+1
	// 4   3 = 6-x+1
	// 3   0 = 3-x+0
	// 2   2 = 3-x+1
	// 1   3 = 3-x+1
	//
	// z=(a-x+b)
	//
	// x   a
	// -----
	// 9   9 = ((x+2)/3)*3
	// 8   9 = ((x+2)/3)*3
	// 7   9 = ((x+2)/3)*3
	// 6   6 = ((x+2)/3)*3
	// 5   6 = ((x+2)/3)*3
	// 4   6 = ((x+2)/3)*3
	// 3   3 = ((x+2)/3)*3
	// 2   3 = ((x+2)/3)*3
	// 1   3 = ((x+2)/3)*3
	//
	// a=(((x+2)/3)*3)
	//
	// x   b
	// -----
	// 9   0 = 1-((x+2)%3/2)
	// 8   1 = 1-((x+2)%3/2)
	// 7   1 = 1-((x+2)%3/2)
	// 6   0 = 1-((x+2)%3/2)
	// 5   1 = 1-((x+2)%3/2)
	// 4   1 = 1-((x+2)%3/2)
	// 3   0 = 1-((x+2)%3/2)
	// 2   1 = 1-((x+2)%3/2)
	// 1   1 = 1-((x+2)%3/2)
	//
	// b=(1-((x+2)%3/2))
	//
	// z=(a-x+b)
	// z=(a-x+(1-((x+2)%3/2)))
	// a=(((x+2)/3)*3)
	// z=((((x+2)/3)*3)-x+(1-((x+2)%3/2)))
	// y=((x*4/3)+z)
	// y=((x*4/3)+((((x+2)/3)*3)-x+(1-((x+2)%3/2))))
	//
	// yay!
	*outputsize=((inputsize*4/3)+((((inputsize+2)/3)*3)-
				inputsize+(1-((inputsize+2)%3/2))));
	*output=new char [(*outputsize)+1];

	uint64_t	outputindex=0;
	unsigned char	data[3];
	uint64_t	bytesremaining=0;
	for (uint64_t inputindex=0;
			inputindex<inputsize;
			inputindex=inputindex+3) {

		bytesremaining=inputsize-inputindex;
		if (bytesremaining>=3) {
			rawbuffer::copy(data,input+inputindex,3);
		} else {
			rawbuffer::copy(data,input+inputindex,bytesremaining);
			rawbuffer::zero(data+bytesremaining,3-(bytesremaining));
		}

		(*output)[outputindex++]=b64code[data[0]>>2];
		(*output)[outputindex++]=b64code[(data[0]<<4|data[1]>>4)&0x3F];
		(*output)[outputindex++]=b64code[(data[1]<<2|data[2]>>6)&0x3F];
		(*output)[outputindex++]=b64code[data[2]&0x3F];
	}
	(*output)[outputindex]='\0';

	// convert any excess A's to ='s
	uint64_t	excess=3-bytesremaining;
	for (uint64_t i=0; i<excess; i++) {
		if ((*output)[--outputindex]=='A') {
			(*output)[outputindex]='=';
		}
	}
}

unsigned char *charstring::base64Decode(const char *input) {
	return base64Decode(input,length(input));
}

unsigned char *charstring::base64Decode(const char *input,
						uint64_t inputsize) {
	unsigned char	*retval=NULL;
	uint64_t	retvalsize=0;
	base64Decode(input,inputsize,&retval,&retvalsize);
	return retval;
}

void charstring::base64Decode(const char *input, uint64_t inputsize,
				unsigned char **output, uint64_t *outputsize) {

	// handle null input
	if (!input) {
		*output=NULL;
		*outputsize=0;
		return;
	}

	// handle 0-length input
	if (!inputsize) {
		*output=(unsigned char *)charstring::duplicate("");
		*outputsize=0;
		return;
	}

	// handle real input...
	*outputsize=inputsize*3/4;
	*output=new unsigned char [(*outputsize)+1];
	uint64_t	outputindex=0;
	unsigned char	data[4];
	uint64_t	inputindex=0;
	while (inputindex<inputsize) {

		data[0]=(unsigned char)b64dcode[(int32_t)input[inputindex++]];
		data[1]=(unsigned char)b64dcode[(int32_t)input[inputindex++]];
		data[2]=(unsigned char)b64dcode[(int32_t)input[inputindex++]];
		data[3]=(unsigned char)b64dcode[(int32_t)input[inputindex++]];

		(*output)[outputindex++]=data[0]<<2|data[1]>>4;
		(*output)[outputindex++]=(data[1]&0x0F)<<4|data[2]>>2;
		(*output)[outputindex++]=(data[2]&0x03)<<6|data[3];
	}
	(*output)[outputindex]='\0';

	// the output could contain some trailing \0's that are artifacts of
	// the ='s in the encoded data, no big deal if the decoded data is
	// supposed to be a string, but not ok for binary data.  Reduce
	// outputsize accordingly.

	// there can be at most 2 trailing ='s, each equal represents an
	// additional trailing NULL, reduce outputsize accordingly
	(*outputsize)-=(input[inputsize-1]=='=')+(input[inputsize-2]=='=');
}

char *charstring::insertString(const char *dest,
				const char *src, uint64_t index) {

	uint64_t	srcsize=length(src);
	uint64_t	size=length(dest)+srcsize+1;
	char		*retval=new char[size];
	for (uint64_t i=0,j=0; i<size;) {
		if (i==index) {
			for (uint64_t k=0; k<srcsize; k++) {
				retval[i++]=src[k];
			}
		} else {
			retval[i++]=dest[j++];
		}
	}
	retval[size-1]='\0';
	return retval;
}

void charstring::obfuscate(char *str) {
	for (char *ch=str; *ch; ch++) {
		*ch=(*ch)+128;
	}
}

void charstring::deobfuscate(char *str) {
	for (char *ch=str; *ch; ch++) {
		*ch=(*ch)-128;
	}
}

char *charstring::padString(const char *str, char padchar,
				int16_t direction, uint64_t totallength) {

	if (totallength==0) {
		return NULL;
	}

	uint64_t	strlen=((str==NULL)?0:length(str));
	char		*newstring=NULL;

	newstring=new char[totallength+1];
	if (strlen>=totallength) {
		charstring::copy(newstring,str,totallength);
		newstring[totallength]=0;
		return newstring;
	}

	rawbuffer::set(newstring,padchar,totallength);
	newstring[totallength]=0;

	if (direction<0) {
		// pad left
		charstring::copy(&newstring[totallength-strlen],str,strlen);
	} else if (direction>0) {
		// pad right
		charstring::copy(newstring,str,strlen);
	} else {
		// pad center
		charstring::copy(&newstring[(totallength-strlen)/2],str,strlen);
	}

	return newstring;
}

ssize_t charstring::printf(char *buffer, size_t length,
					const char *format, ...) {
	va_list	argp;
	va_start(argp,format);
	size_t	result=printf(buffer,length,format,&argp);
	va_end(argp);
	return result;
}

#if !defined(RUDIMENTS_HAVE_VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE___VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF)
// This is quite a hack...
//
// Old enough systems (like linux libc4) don't provide vsnprintf but do provide
// vsprintf.  There's no safe way to use vsprintf though, especially the way
// the variablebuffer class would like to use it.
//
// I could grab a vsnprintf implementation from any of the other
// LGPL-compatible libraries.  Tried it.  Too much work.
//
// vsnprintf could be implemented using vsprintf but a large enough buffer to
// vsprintf to safely must be created.
//
// I could implement a format string parser that calculates the buffer
// size.  Tried it.  Also too much work.
// 
// Short of that, the only safe thing to do is vfprintf to a scratch file,
// find out how many characters were written and then do the same to a string.
// 
// That's taking the long way around for sure.
//
// I tried writing to /dev/null but vsprintf to /dev/null returns 0.  I
// should have expected that actually.  Unless there's some sort of personal-
// ramdisk that I don't know about, it appears we have to use an actual file.
//
// I'm not even going to benchmark to find out how poorly this performs.
// Hopefully disk-caching will help it out.  Also, if you happen to be using
// a ram-based /tmp then that will help too.  Systems old enough to need this
// probably aren't though.
//
// There are, of course, security concerns.  Anyone with the right permissions
// can read the scratch file.  Someone could delete it, and on some systems
// that could cause problems.
//
// The scratch file uses the PID of the current process for uniqueness.  This
// could cause race conditions in multi-threaded programs, but chances are if
// your system doesn't have vsnprintf, then it doesn't have thread support
// either.
//
// The process, while terribly inefficient, should be clean.  It only creates
// one scratch file per process and cleans it up at exit, unless the program
// crashes or is killed with -9.

static char	*scratchfile=NULL;
static FILE	*scratch=NULL;

static void removeScratch() {
	file::remove(scratchfile);
	if (scratch) {
		fclose(scratch);
		delete[] scratchfile;
	}
}

static ssize_t vsnprintf(char *buffer, size_t length,
				const char *format, va_list argp) {

	// open a scratch file if it's not already open
	if (!scratch) {
		scratchfile=new char[20];
		charstring::copy(scratchfile,"/tmp/scratch.");
		charstring::append(scratchfile,
					(uint64_t)process::getProcessId());
		scratch=fopen(scratchfile,"w+");
		if (!scratch) {
			delete[] scratchfile;
			scratchfile=NULL;
			return -1;
		}
		atexit((void (*)(void))removeScratch);
	}

	// rewind the scratch file
	rewind(scratch);

	// write to the scratch file so we can
	// figure out how much space we need
	ssize_t	safebuffersize=vfprintf(scratch,format,argp);

	// create a big enough buffer for that
	char	*safebuffer=new char[safebuffersize+1];

	// vsprintf to safebuffer
	ssize_t byteswritten=vsprintf(safebuffer,format,argp);

	// bail on error
	if (byteswritten==-1) {
		delete[] safebuffer;
		return -1;
	}

	// figure out how many bytes we can copy back to "buffer"
	// (the +1 is because vsprintf returns the number of bytes written
	// minus the NULL terminator)
	size_t	bytestocopy=((size_t)byteswritten+1<length)?
					byteswritten+1:length;

	// copy what we can back to "buffer"
	charstring::copy(buffer,safebuffer,bytestocopy);

	// clean up
	delete[] safebuffer;

	// return the number of bytes we would like to have copied
	// (except for the NULL terminator)
	return safebuffersize;
}
#endif

ssize_t charstring::printf(char *buffer, size_t length,
					const char *format, va_list *argp) {

	// vsnprintf should write whatever will fit into "buffer" and
	// either return the number of bytes that were written or the
	// number of bytes that would have been written if truncation
	// hadn't occurred.

	// But, implementations vary widely.

	// Some implementations (like linux libc5) crash if "buffer" is NULL
	// and corrupt memory if "buffer" is only 1 character.  Use a buffer
	// of at least two characters in either of those cases.
	char	*buf=buffer;
	size_t	buflen=length;
	char	b[2];
	if (!buf || buflen<2) {
		buf=b;
		buflen=sizeof(b);
	}
	#if defined(RUDIMENTS_HAVE___VSNPRINTF) || \
		defined(RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF)
		ssize_t	size=__vsnprintf(buf,buflen,format,*argp);
	#else
		ssize_t	size=vsnprintf(buf,buflen,format,*argp);
	#endif

	// Some implementations (SCO OSR6, Redhat 5.2, probably others) return
	// -1 if truncation occurred though and don't write anything to
	// "buffer".
	//
	// For systems like those, we'll simulate the expected behavior...
	buflen=length;
	while (size==-1) {
		// FIXME: for large strings this can be incredibly slow
		buflen=buflen+16;
		buf=new char[buflen];
		size=vsnprintf(buf,buflen,format,*argp);
		if (size>-1) {
			charstring::copy(buffer,buf,length);
		}
		delete[] buf;
	}
	return size;
}
