// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/rawbuffer.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

RUDIMENTS_INLINE
size_t charstring::length(const char *string) {
	return strlen(string);
}

RUDIMENTS_INLINE
void charstring::zero(char *str, size_t size) {
	rawbuffer::set((void *)str,0,size);
}

RUDIMENTS_INLINE
char *charstring::append(char *dest, const char *source) {
	return strcat(dest,source);
}

RUDIMENTS_INLINE
char *charstring::append(char *dest, const char *source, size_t size) {
	return strncat(dest,source,size);
}

RUDIMENTS_INLINE
char *charstring::copy(char *dest, const char *source) {
	return strcpy(dest,source);
}

RUDIMENTS_INLINE
char *charstring::copy(char *dest, const char *source, size_t size) {
	return strncpy(dest,source,size);
}

RUDIMENTS_INLINE
char *charstring::copy(char *dest, size_t location, const char *source) {
	return strcpy(dest+location,source);
}

RUDIMENTS_INLINE
char *charstring::copy(char *dest, size_t location,
					const char *source, size_t size) {
	return strncpy(dest+location,source,size);
}

RUDIMENTS_INLINE
int charstring::compare(const char *str1, const char *str2) {
	// FIXME: use strcoll?
	return strcmp(str1,str2);
}

RUDIMENTS_INLINE
int charstring::compare(const char *str1, const char *str2, size_t size) {
	return strncmp(str1,str2,size);
}

RUDIMENTS_INLINE
int charstring::compareIgnoringCase(const char *str1, const char *str2) {
	return strcasecmp(str1,str2);
}

RUDIMENTS_INLINE
int charstring::compareIgnoringCase(const char *str1,
						const char *str2, size_t size) {
	return strncasecmp(str1,str2,size);
}

RUDIMENTS_INLINE
bool charstring::contains(const char *haystack, const char *needle) {
	return (findFirst(haystack,needle)!=NULL);
}

RUDIMENTS_INLINE
bool charstring::contains(const char *haystack, const char needle) {
	return (findFirst(haystack,needle)!=NULL);
}

RUDIMENTS_INLINE
char *charstring::findFirst(const char *haystack, const char *needle) {
	return strstr(haystack,needle);
}

RUDIMENTS_INLINE
char *charstring::findFirst(const char *haystack, const char needle) {
	return strchr(haystack,needle);
}

RUDIMENTS_INLINE
char *charstring::findLast(const char *haystack, const char needle) {
	return strrchr(haystack,needle);
}

RUDIMENTS_INLINE
char *charstring::duplicate(const char *str) {
	return strdup(str);
}

RUDIMENTS_INLINE
void charstring::rightTrim(char *string) {
	rightTrim(string,' ');
}

RUDIMENTS_INLINE
void charstring::leftTrim(char *string) {
	leftTrim(string,' ');
}

RUDIMENTS_INLINE
void charstring::bothTrim(char *string) {
	bothTrim(string,' ');
}

RUDIMENTS_INLINE
long charstring::toLong(const char *string) {
	return strtol(string,NULL,0);
}

RUDIMENTS_INLINE
long charstring::toLong(const char *string, char **endptr) {
	return strtol(string,endptr,0);
}

RUDIMENTS_INLINE
long charstring::toLong(const char *string, int base) {
	return strtol(string,NULL,base);
}

RUDIMENTS_INLINE
long charstring::toLong(const char *string, char **endptr, int base) {
	return strtol(string,endptr,base);
}

RUDIMENTS_INLINE
long long charstring::toLongLong(const char *string) {
	return strtoll(string,NULL,0);
}

RUDIMENTS_INLINE
long long charstring::toLongLong(const char *string, char **endptr) {
	return strtoll(string,endptr,0);
}

RUDIMENTS_INLINE
long long charstring::toLongLong(const char *string, int base) {
	return strtoll(string,NULL,base);
}

RUDIMENTS_INLINE
long long charstring::toLongLong(const char *string, char **endptr, int base) {
	return strtoll(string,endptr,base);
}

RUDIMENTS_INLINE
float charstring::toFloat(const char *string) {
	return strtof(string,NULL);
}

RUDIMENTS_INLINE
float charstring::toFloat(const char *string, char **endptr) {
	return strtof(string,endptr);
}

RUDIMENTS_INLINE
double charstring::toDouble(const char *string) {
	return strtod(string,NULL);
}

RUDIMENTS_INLINE
double charstring::toDouble(const char *string, char **endptr) {
	return strtod(string,endptr);
}

RUDIMENTS_INLINE
long double charstring::toLongDouble(const char *string) {
	return strtold(string,NULL);
}

RUDIMENTS_INLINE
long double charstring::toLongDouble(const char *string, char **endptr) {
	return strtold(string,endptr);
}
