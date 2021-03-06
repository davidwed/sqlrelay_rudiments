// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/character.h>
#if !defined(RUDIMENTS_HAVE_VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE___VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF)
	#include <rudiments/process.h>
	#include <rudiments/file.h>
#endif
#include <rudiments/stringbuffer.h>

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

// for vsnprintf/vsnprintf_s
#include <stdio.h>

#ifdef RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF
extern "C" int __vsnprintf(char *str, size_t size,
				const char *format, va_list ap);
#endif

// MSVC 2010- doesn't define va_copy
#if defined(_MSC_VER) && (_MSC_VER <= 1700)
	#define va_copy(a,b) ((a)=(b))
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

#ifdef RUDIMENTS_HAVE_LOCALE_H
	#include <locale.h>
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
	if (str) {
		for (char *ch=str; *ch; ch++) {
			*ch=character::toUpperCase(*ch);
		}
	}
}

void charstring::lower(char *str) {
	if (str) {
		for (char *ch=str; *ch; ch++) {
			*ch=character::toLowerCase(*ch);
		}
	}
}

void charstring::capitalize(char *str) {
	if (str) {
		bool	cap=true;
		for (char *ch=str; *ch; ch++) {
			if (cap) {
				*ch=character::toUpperCase(*ch);
				cap=false;
			} else {
				*ch=character::toLowerCase(*ch);
			}
			if (character::inSet(*ch," '\"-(")) {
				cap=true;
			}
		}
	}
}

void charstring::rightTrim(char *str, char character) {

	if (!isNullOrEmpty(str)) {

		// advance to the last character in the string
		while (*str) {
			str++;
		}
		str--;

		// back up to the first instance of the character to trim
		while (*str==character) { 
			str--;
		}
		str++;

		// terminate the string there
		*str='\0';
	}
}

void charstring::leftTrim(char *str, char character) {

	if (!isNullOrEmpty(str)) {

		int32_t	i=0;
		int32_t	j=0;

		// advance past all of the characters we want to trim
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
		if (!compare(str1+index,str2,str2len)) {
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

char *charstring::replace(const char *str, const char *oldstr,
						const char *newstr) {
	stringbuffer	newstring;
	ssize_t		oldstrlen=charstring::length(oldstr);
	const char	*ptr=str;
	const char	*start=ptr;
	while (*ptr) {
		if (!charstring::compare(ptr,oldstr,oldstrlen)) {
			newstring.append(start,ptr-start);
			newstring.append(newstr);
			ptr+=oldstrlen;
			start=ptr;
		} else {
			ptr++;
		}
	}
	newstring.append(start,ptr-start);
	return newstring.detachString();
}

char *charstring::replace(const char *str, const char * const *oldstrset,
						ssize_t *oldstrlen,
						const char * const *newstrset) {

	// search and replace
	stringbuffer	newstring;
	const char	*ptr=str;
	const char	*start=ptr;
	while (*ptr) {
		bool	found=false;
		uint64_t i=0;
		for (const char * const *oldptr=oldstrset; *oldptr; oldptr++) {
			if (!charstring::compare(
					ptr,oldstrset[i],oldstrlen[i])) {
				newstring.append(start,ptr-start);
				newstring.append(newstrset[i]);
				ptr+=oldstrlen[i];
				start=ptr;
				found=true;
				break;
			}
			i++;
		}
		if (!found) {
			ptr++;
		}
	}
	newstring.append(start,ptr-start);

	return newstring.detachString();
}

char *charstring::replace(const char *str,
				regularexpression *from,
				const char *to,
				bool replaceglobal) {
	
	// declare buffer for new string
	stringbuffer	newstring;

	const char	*start=str;
	const char	*ptr=start;
	for (;;) {

		// look for a matching part
		if (!*ptr || !from->match(ptr) || !from->getSubstringCount()) {

			// bail if no match is found
			break;
		}

		// get the bounds of the matching chunk
		int32_t		fi=from->getSubstringCount()-1;
		const char	*fromstart=from->getSubstringStart(fi);
		const char	*fromend=from->getSubstringEnd(fi);

		// move on if they're the same
		if (fromend==fromstart) {
			ptr++;
			continue;
		}

		// append the previous, non-matching part of the chunk
		newstring.append(start,fromstart-start);

		// append the replacement part
		newstring.append(to);

		// move the start forward in the matching chunk
		start=fromend;
		ptr=start;

		// bail if we're not replacing globally
		if (!replaceglobal) {
			break;
		}
	}

	// append the rest of the chunk
	newstring.append(start);

	// return the string that contains the replacements
	return newstring.detachString();
}

char *charstring::replace(const char *str,
				regularexpression *match,
				bool matchglobal,
				regularexpression *from,
				const char *to,
				bool replaceglobal) {

	// declare buffer for new string
	stringbuffer	newstring;

	const char	*start=str;
	const char	*ptr=start;
	for (;;) {

		// look for a matching part
		if (!*ptr || !match->match(ptr) ||
				!match->getSubstringCount()) {

			// bail if no match is found
			break;
		}

		// get the bounds of the matching chunk
		int32_t		mi=match->getSubstringCount()-1;
		const char	*matchstart=match->getSubstringStart(mi);
		const char	*matchend=match->getSubstringEnd(mi);

		// move on if they're the same
		if (matchend==matchstart) {
			ptr++;
			continue;
		}

		// get a copy of the matching chunk
		char		*matchchunk=charstring::duplicate(matchstart,
							matchend-matchstart);

		// append the previous, non-matching part of the main string
		newstring.append(start,matchstart-start);

		// transform the chunk...

		start=matchchunk;
		ptr=start;
		for (;;) {

			// look for a matching part
			if (!*ptr || !from->match(ptr) ||
					!from->getSubstringCount()) {

				// bail if no match is found
				break;
			}

			// get the bounds of the matching chunk
			int32_t		fi=from->getSubstringCount()-1;
			const char	*fromstart=
					from->getSubstringStart(fi);
			const char	*fromend=
					from->getSubstringEnd(fi);

			// move on if they're the same
			if (fromend==fromstart) {
				ptr++;
				continue;
			}

			// append the previous, non-matching part of the chunk
			newstring.append(start,fromstart-start);

			// append the replacement part
			newstring.append(to);

			// move the start forward in the matching chunk
			start=fromend;
			ptr=start;

			// bail if we're not replacing globally
			if (!replaceglobal) {
				break;
			}
		}

		// append the rest of the chunk
		newstring.append(start);

		// move the start forward in the main string
		start=matchend;
		ptr=start;

		// bail if we're not matching globally
		if (!matchglobal) {
			break;
		}
	}

	// append the rest of the main string
	newstring.append(start);

	// return the string that contains the replacements
	return newstring.detachString();
}

bool charstring::isInteger(const char *str) {

	if (isNullOrEmpty(str)) {
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

	if (!str || !size) {
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

	if (isNullOrEmpty(str)) {
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

	if (!str || !size) {
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
	int64_t	amt;
	#ifdef RUDIMENTS_HAVE_LLABS
		amt=llabs((long long)amount);
	#else
		amt=abs((long)amount);
	#endif
	printf(amountstr,length,
			"$%s%lld.%02lld",negative,
			amt/100,amt-(amt/100*100));
	return amountstr;
}

char *charstring::convertAmount(int64_t amount, uint16_t spaces) {
	char	*amt=convertAmount(amount);
	ssize_t	amtlen=length(amt+1);
	uint16_t	realspaces=(amtlen+1>spaces)?amtlen+1:spaces;
	char	*buffer=new char[realspaces+1];
	buffer[realspaces]='\0';
	bytestring::set(buffer,' ',realspaces);
	bytestring::copy(buffer+realspaces-amtlen,amt+1,amtlen);
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
				(*ptr>='0' && *ptr<='9') ||
				character::inSet(*ptr,
					"-._~:/?#[]@!$&'()*,;=")) {
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
	if (spaces==0){
		return;
	}

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

void charstring::rightJustify(char *str, int32_t length) {

	if (!str) {
		return;
	}
	
	// count trailing spaces
	int32_t	spaces=countTrailingSpaces(str,length);
	if (spaces==0){
		return;
	}

	// replace characters
	int32_t	index;
	int32_t	stop=spaces-1;
	for (index=length-1; index>stop; index--) {
		str[index]=str[index-spaces];
	}

	// right-pad with spaces
	for (; index>-1; index--) {
		str[index]=' ';
	}
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
	return length((const char *)(string));
}

bool charstring::isNullOrEmpty(const char *string) {
	return !(string && string[0]);
}

bool charstring::isNullOrEmpty(const unsigned char *string) {
	return !(string && string[0]);
}

bool charstring::isYes(const char *string) {
	char	next;
	if (!string) {
		return false;
	} else if (!charstring::compareIgnoringCase(string,"yes",3)) {
		next=string[3];
	} else if (!charstring::compareIgnoringCase(string,"true",4)) {
		next=string[4];
	} else if (string[0]=='y' || string[0]=='Y' ||
			string[0]=='t' || string[0]=='T' ||
			string[0]=='1') {
		next=string[1];
	} else {
		return false;
	}
	return (!next || (!character::isAlphanumeric(next) && 
				!character::isAlphabeticalExtended(next)));
}

bool charstring::isNo(const char *string) {
	char	next;
	if (!string) {
		return false;
	} else if (!charstring::compareIgnoringCase(string,"no",2)) {
		next=string[2];
	} else if (!charstring::compareIgnoringCase(string,"false",5)) {
		next=string[5];
	} else if (string[0]=='n' || string[0]=='N' ||
			string[0]=='f' || string[0]=='F' ||
			string[0]=='0') {
		next=string[1];
	} else {
		return false;
	}
	return (!next || (!character::isAlphanumeric(next) && 
				!character::isAlphabeticalExtended(next)));
}

void charstring::zero(char *str, size_t size) {
	bytestring::set(str,0,size);
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
		if (!str1 || !str2) {
			return (str1!=str2);
		}
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
		if (*ptr1) {
			return *ptr1;
		} else if (*ptr2) {
			return -(*ptr2);
		}
		return 0;
	#endif
}

int32_t charstring::compareIgnoringCase(const char *str1,
						const char *str2, size_t size) {
	#ifdef RUDIMENTS_HAVE_STRNCASECMP
		return (str1 && str2)?strncasecmp(str1,str2,size):(str1!=str2);
	#else
		if (!str1 || !str2) {
			return (str1!=str2);
		}
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
		if (count<size) {
			if (*ptr1 && !*ptr2) {
				return *ptr1;
			} else if (*ptr2 && !*ptr1) {
				return -(*ptr2);
			}
		}
		return 0;
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
	return (char *)(findFirstOrEnd((const char *)(haystack),needle));
}

const char *charstring::findLast(const char *haystack, char needle) {
	return (haystack)?strrchr(haystack,needle):NULL;
}

char *charstring::findFirst(char *haystack, const char *needle) {
	return (char *)(findFirst((const char *)(haystack),needle));
}

char *charstring::findFirst(char *haystack, char needle) {
	return (char *)(findFirst((const char *)(haystack),needle));
}

char *charstring::findLast(char *haystack, const char *needle) {
	return (char *)(findLast((const char *)(haystack),needle));
}

char *charstring::findLast(char *haystack, char needle) {
	return (char *)(findLast((const char *)(haystack),needle));
}

const char *charstring::findFirstOfSet(const char *haystack, const char *set) {
	return (char *)(findFirstOfSet((char *)(haystack),set));
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
	return (char *)(findFirstOfSetOrEnd((char *)(haystack),set));
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

const char *charstring::findLastOfSet(const char *haystack, const char *set) {
	return (char *)(findLastOfSet((char *)(haystack),set));
}

char *charstring::findLastOfSet(char *haystack, const char *set) {
	if (!haystack || !set) {
		return NULL;
	}
	char	*retval=haystack+charstring::length(haystack);
	do {
		retval--;
		if (contains(set,*retval)) {
			return retval;
		}
	} while (retval>haystack);
	return NULL;
}

const char *charstring::findLastOfSetOrEnd(const char *haystack,
							const char *set) {
	return (char *)(findLastOfSetOrEnd((char *)(haystack),set));
}

char *charstring::findLastOfSetOrEnd(char *haystack, const char *set) {
	if (!haystack || !set) {
		return NULL;
	}
	char	*retval=haystack+charstring::length(haystack);
	do {
		retval--;
		if (contains(set,*retval)) {
			return retval;
		}
	} while (retval>haystack);
	return haystack+charstring::length(haystack);
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

void charstring::rightTrim(char *str) {

	if (!isNullOrEmpty(str)) {

		// advance to the last character in the string
		while (*str) {
			str++;
		}
		str--;

		// back up to the first instance of the character to trim
		while (character::isWhitespace(*str)) { 
			str--;
		}
		str++;

		// terminate the string there
		*str='\0';
	}
}

void charstring::leftTrim(char *str) {

	if (!isNullOrEmpty(str)) {

		int32_t	i=0;
		int32_t	j=0;

		// advance past all of the characters we want to trim
		while (character::isWhitespace(str[i])) {
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

void charstring::bothTrim(char *string) {
	leftTrim(string);
	rightTrim(string);
}

int64_t charstring::toInteger(const char *string) {
	return toInteger(string,NULL,10);
}

int64_t charstring::toInteger(const char *string, const char **endptr) {
	return toInteger(string,endptr,10);
}

int64_t charstring::toInteger(const char *string, int32_t base) {
	return toInteger(string,NULL,base);
}

int64_t charstring::toInteger(const char *string,
				const char **endptr, int32_t base) {
	#ifdef RUDIMENTS_HAVE_STRTOLL
	return (string)?strtoll(string,(char **)endptr,base):0;
	#else
	return (string)?strtol(string,(char **)endptr,base):0;
	#endif
}

uint64_t charstring::toUnsignedInteger(const char *string) {
	return toUnsignedInteger(string,NULL,10);
}

uint64_t charstring::toUnsignedInteger(const char *string,
					const char **endptr) {
	return toUnsignedInteger(string,endptr,10);
}

uint64_t charstring::toUnsignedInteger(const char *string, int32_t base) {
	return toUnsignedInteger(string,NULL,base);
}

uint64_t charstring::toUnsignedInteger(const char *string,
					const char **endptr, int32_t base) {
	#ifdef RUDIMENTS_HAVE_STRTOULL
	return (string)?strtoull(string,(char **)endptr,base):0;
	#else
	return (string)?strtoul(string,(char **)endptr,base):0;
	#endif
}

long double charstring::toFloat(const char *string) {
	return toFloat(string,NULL);
}

long double charstring::toFloatC(const char *string) {

	/* This method is needed when the locale of the client is different
	 * from the C/POSIX locale, but we still need to convert a string
	 * formatted in C/POSIX locale format to a float. Perhaps the string
	 * was supplied to us by a host which is using the C/POSIX locale).
	 * Because there are no standard, let alone portable api, for string
	 * conversion functions that take a locale as argument, this code
	 * implements a workaround of converting the string from C locale
	 * representation to one appropriate for the current locale. */

#ifdef RUDIMENTS_HAVE_LOCALE_H
	size_t		len=length(string);
	char		stringinlocale[256];
	const char	*decimalpointlocation;
	struct	lconv	*currentlconv=localeconv();
	if ((currentlconv!=NULL) &&
		(currentlconv->decimal_point!=NULL) &&
		(currentlconv->decimal_point[0]!=0) &&
		(currentlconv->decimal_point[0]!='.') &&
		(currentlconv->decimal_point[1]==0) &&
		((decimalpointlocation=findFirst(string,'.'))!=NULL) &&
		(len<sizeof(stringinlocale))) {

		bytestring::copy(stringinlocale,string,len+1);

		stringinlocale[decimalpointlocation-string]=
					currentlconv->decimal_point[0];

		return toFloat(stringinlocale,NULL);
	}
#endif
	return toFloat(string,NULL);
}

long double charstring::toFloat(const char *string, const char **endptr) {
	#ifdef RUDIMENTS_HAVE_STRTOLD
	return (string)?strtold(string,(char **)endptr):0.0;
	#else
	return (string)?(long double)(strtod(string,(char **)endptr)):0.0;
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
	if (!list && !listlength) {
		return;
	}
	if (isNullOrEmpty(string) || !stringlength ||
		isNullOrEmpty(delimiter) || !delimiterlength) {
		if (list) {
			(*list)=NULL;
		}
		if (listlength) {
			(*listlength)=0;
		}
		return;
	}

	// declare local list length
	uint64_t	ll=0;

	// 2 passes,
	// 1 to count the number of chunks to split the string into,
	// 1 to actually split the string
	for (int32_t pass=0; pass<2; pass++) {

		// set pointers to the beginning and end of the string
		const char	*start=string;
		const char	*end=string+stringlength;

		// initialize the list length
		ll=0;

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
					if (pass && list) {

						// make a copy of the string
						// between the last delimiter
						// and here
						(*list)[ll]=
							charstring::duplicate(
								start,
								current-start);
					}

					// increment the counter
					ll++;
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
		if (!pass && list) {
			(*list)=new char *[ll];
		}
	}

	// copy out the listlength
	if (listlength) {
		(*listlength)=ll;
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
			bytestring::copy(data,input+inputindex,3);
		} else {
			bytestring::copy(data,input+inputindex,bytesremaining);
			bytestring::zero(data+bytesremaining,3-(bytesremaining));
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

char *charstring::pad(const char *str, char padchar,
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

	bytestring::set(newstring,padchar,totallength);
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
	va_list	args;
	va_start(args,format);
	size_t	result=printf(buffer,length,format,&args);
	va_end(args);
	return result;
}

#if !defined(RUDIMENTS_HAVE_VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE__VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE___VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF) && \
	!defined(RUDIMENTS_HAVE_UNDEFINED_VSNPRINTF_S)
// This is quite a hack...
//
// Old enough systems (like linux libc4) don't provide vsnprintf but do provide
// vsprintf.  There's no safe way to use vsprintf though, especially the way
// the bytebuffer class would like to use it.
//
// I could grab a vsnprintf implementation from any of the other
// LGPL-compatible libraries.  Tried it.  Too much work.
//
// vsnprintf could be implemented using vsprintf but a large enough buffer to
// vsprintf to safely must be created.  What is "large enough" though?
//
// I could implement a format string parser that calculates the buffer
// size.  Tried it.  Also too much work.
// 
// Short of that, the only safe thing to do is vfprintf to a file, find out how
// many characters were written and then do the same to a string.
// 
// That's taking the long way around for sure.
//
// The null device is the obvious choice, so we'll try that first.  But, some
// implementations of vsprintf return 0 when writing to the null device.  Or,
// maybe some implementations of the null device return 0 when anything is
// written to them.  Either way, if using the null device fails, we fall back
// to a scratch file.
//
// I'm not even going to benchmark to find out how poorly this performs.
// Hopefully disk-caching will help it out.  Also, if you happen to be using
// a ram-based temporary directory then that will help too.  Systems old enough
// to need this probably aren't though.
//
// There are, of course, security concerns with the scratch file.  Anyone with
// the right permissions can read the scratch file.  Someone could delete it,
// and on some systems that could cause problems.
//
// The scratch file uses the PID of the current process for uniqueness.  This
// could cause race conditions in multi-threaded programs, but chances are if
// your system doesn't have vsnprintf, then it doesn't have thread support
// either.
//
// While terribly inefficient, the scratch file process should be clean.  It
// only creates one scratch file per process and cleans it up at exit, unless
// the program crashes or is killed with -9.

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

		// first try the null device
		scratchfile=charstring::duplicate(
					#if defined(_WIN32)
						"\Device\Null"
					#elif defined(__VMS)
						"NLA0:"
					#else
						"/dev/null"
					#endif
						);
		scratch=fopen(scratchfile,"w");
		if (scratch) {
			// writing to the null device returns
			// 0 or -1 on some platforms
			if (fprintf(scratch,"test")!=4) {
				fclose(scratch);
				scratch=NULL;
				delete[] scratchfile;
			}
		} else {
			delete[] scratchfile;
		}

		// if that fails then try /tmp/scratch.pid
		if (!scratch) {
			scratchfile=new char[20];
			charstring::copy(scratchfile,"/tmp/scratch.");
			charstring::append(scratchfile,
					(uint64_t)process::getProcessId());
			scratch=fopen(scratchfile,"w+");
			if (scratch) {
				process::atExit((void (*)(void))removeScratch);
				rewind(scratch);
			} else {
				delete[] scratchfile;
				return -1;
			}
		}
	}


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

// now we can say that we have vsnprintf
#define RUDIMENTS_HAVE_VSNPRINTF 1

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
	#if defined(RUDIMENTS_HAVE_VSNPRINTF_S)
		ssize_t	size=vsnprintf_s(buf,buflen,_TRUNCATE,format,*argp);
	#elif defined(RUDIMENTS_HAVE___VSNPRINTF)
		ssize_t	size=__vsnprintf(buf,buflen,format,*argp);
	#elif defined(RUDIMENTS_HAVE__VSNPRINTF)
		ssize_t	size=_vsnprintf(buf,buflen,format,*argp);
	#elif defined(RUDIMENTS_HAVE_VSNPRINTF)
		ssize_t	size=vsnprintf(buf,buflen,format,*argp);
	#elif defined(RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF)
		ssize_t	size=__vsnprintf(buf,buflen,format,*argp);
		// Solaris 2.5.1 (and maybe others) return buflen if
		// truncation occurs.  In that case, simulate systems
		// that return -1 if truncation occurs.
		if (size==(ssize_t)buflen-1) {
			size=-1;
		}
	#else
		#error no vsnprintf or anything like it
	#endif

	// Some implementations (SCO OSR6, Redhat 5.2, probably others) return
	// -1 if truncation occurred though and don't write anything to
	// "buffer".
	//
	// For systems like those, we'll simulate the expected behavior...
	buflen=length;
	size_t	inc=16;
	while (size==-1) {

		buflen=buflen+inc;
		buf=new char[buflen];

		#if defined(RUDIMENTS_HAVE_VSNPRINTF_S)
			size=vsnprintf_s(buf,buflen,_TRUNCATE,format,*argp);
		#elif defined(RUDIMENTS_HAVE___VSNPRINTF)
			size=__vsnprintf(buf,buflen,format,*argp);
		#elif defined(RUDIMENTS_HAVE__VSNPRINTF)
			size=_vsnprintf(buf,buflen,format,*argp);
		#elif defined(RUDIMENTS_HAVE_VSNPRINTF)
			size=vsnprintf(buf,buflen,format,*argp);
		#elif defined(RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF)
			size=__vsnprintf(buf,buflen,format,*argp);
			// Solaris 2.5.1 (and maybe others) return buflen if
			// truncation occurs.  In that case, simulate systems
			// that return -1 if truncation occurs.
			if (size==(ssize_t)buflen-1) {
				size=-1;
			}
		#else
			#error no vsnprintf or anything like it
		#endif
		if (size>-1) {
			charstring::copy(buffer,buf,length);
		}
		delete[] buf;

		// adjust how quickly the buffer grows
		// (this can certainly be optimized further)
		inc=inc*2;
		if (inc>1024) {
			inc=1024;
		}
	}
	return size;
}

ssize_t charstring::printf(char **buffer, const char *format, ...) {
	va_list	args;
	va_start(args,format);
	// the printf() below will call va_end(args)
	return printf(buffer,format,&args);
}

ssize_t charstring::printf(char **buffer, const char *format, va_list *argp) {

	// sanity check
	if (!buffer) {
		return 0;
	}

	// initialize *buffer
	*buffer=NULL;

	// Ideally we'd use vasprintf() if it's available, but we want to be
	// able to delete[] buffer, and if we use vasprintf() to create it then
	// we'd have to free(buffer).

	// Some compilers throw a warning if they see "printf(NULL..." at all,
	// whether it's the global function printf() or one that you've defined
	// yourself.  Using *buffer here works around that.
	va_list	argp1;
	va_copy(argp1,*argp);
	ssize_t	size=charstring::printf(*buffer,0,format,argp);
	va_end(*argp);
	*buffer=new char[size+1];
	size=charstring::printf(*buffer,size+1,format,&argp1);
	va_end(argp1);
	return size;
}
