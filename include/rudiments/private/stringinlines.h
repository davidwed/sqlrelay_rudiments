// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE string::string(const char *str) {
	buffer=strdup(str);
	this->size=strlen(str);
}

RUDIMENTS_INLINE string::string(const char *str, size_t size) {
	memcpy((void *)buffer,(void *)str,size);
	this->size=size;
}

RUDIMENTS_INLINE string::string(size_t size) {
	buffer=new char[size];
	this->size=size;
}

RUDIMENTS_INLINE string::~string() {
	delete[] buffer;
}

RUDIMENTS_INLINE char *string::getString() const {
	return buffer;
}

RUDIMENTS_INLINE size_t string::getLength() const {
	return strlen(buffer);
}

RUDIMENTS_INLINE size_t string::getSize() const {
	return size;
}

RUDIMENTS_INLINE void string::zero() {
	memset((void *)buffer,0,size);
}

RUDIMENTS_INLINE void string::zero(char *str, size_t size) {
	memset((void *)str,0,size);
}

RUDIMENTS_INLINE char *string::append(const string *str) {
	return strcat(buffer,str->getString());
}

RUDIMENTS_INLINE char *string::append(const char *str) {
	return strcat(buffer,str);
}

RUDIMENTS_INLINE char *string::append(const string *str, size_t size) {
	return strncat(buffer,str->getString(),size);
}

RUDIMENTS_INLINE char *string::append(const char *str, size_t size) {
	return strncat(buffer,str,size);
}

RUDIMENTS_INLINE char *string::append(char *dest, const string *source) {
	return strcat(dest,source->getString());
}

RUDIMENTS_INLINE char *string::append(char *dest, const char *source) {
	return strcat(dest,source);
}

RUDIMENTS_INLINE char *string::append(char *dest, const string *source,
								size_t size) {
	return strncat(dest,source->getString(),size);
}

RUDIMENTS_INLINE char *string::append(char *dest, const char *source,
								size_t size) {
	return strncat(dest,source,size);
}

RUDIMENTS_INLINE char *string::copyInto(const string *str) {
	return strcpy(buffer,str->getString());
}

RUDIMENTS_INLINE char *string::copyInto(const char *str) {
	return strcpy(buffer,str);
}

RUDIMENTS_INLINE char *string::copyInto(const char *str, size_t size) {
	return strncpy(buffer,str,size);
}

RUDIMENTS_INLINE char *string::copyInto(size_t location, const string *str) {
	return strcpy(buffer+location,str->getString());
}

RUDIMENTS_INLINE char *string::copyInto(size_t location, const char *str) {
	return strcpy(buffer+location,str);
}

RUDIMENTS_INLINE char *string::copyInto(size_t location, const char *str,
								size_t size) {
	return strncpy(buffer+location,str,size);
}

RUDIMENTS_INLINE char *string::copyInto(char *dest, const string *source) {
	return strcpy(dest,source->getString());
}

RUDIMENTS_INLINE char *string::copyInto(char *dest, const char *source) {
	return strcpy(dest,source);
}

RUDIMENTS_INLINE char *string::copyInto(char *dest, const char *source,
								size_t size) {
	return strncpy(dest,source,size);
}

RUDIMENTS_INLINE char *string::copyInto(char *dest, size_t location,
						const string *source) {
	return strcpy(dest+location,source->getString());
}

RUDIMENTS_INLINE char *string::copyInto(char *dest, size_t location,
						const char *source) {
	return strcpy(dest+location,source);
}

RUDIMENTS_INLINE char *string::copyInto(char *dest, size_t location,
					const char *source, size_t size) {
	return strncpy(dest+location,source,size);
}

RUDIMENTS_INLINE int string::compare(const string *str) const {
	return strcmp(buffer,str->getString());
}

RUDIMENTS_INLINE int string::compare(const char *str) const {
	return strcmp(buffer,str);
}

RUDIMENTS_INLINE int string::compare(const string *str, size_t size) const {
	return strncmp(buffer,str->getString(),size);
}

RUDIMENTS_INLINE int string::compare(const char *str, size_t size) const {
	return strncmp(buffer,str,size);
}

RUDIMENTS_INLINE int string::compareIgnoringCase(const string *str) const {
	return strcasecmp(buffer,str->getString());
}

RUDIMENTS_INLINE int string::compareIgnoringCase(const char *str) const {
	return strcasecmp(buffer,str);
}

RUDIMENTS_INLINE int string::compareIgnoringCase(const string *str,
							size_t size) const {
	return strncasecmp(buffer,str->getString(),size);
}

RUDIMENTS_INLINE int string::compareIgnoringCase(const char *str,
							size_t size) const {
	return strncasecmp(buffer,str,size);
}

RUDIMENTS_INLINE int string::compare(const char *str1, const string *str2) {
	return strcmp(str1,str2->getString());
}

RUDIMENTS_INLINE int string::compare(const char *str1, const char *str2) {
	return strcmp(str1,str2);
}

RUDIMENTS_INLINE int string::compare(const char *str1, const string *str2,
								size_t size) {
	return strncmp(str1,str2->getString(),size);
}

RUDIMENTS_INLINE int string::compare(const char *str1, const char *str2,
								size_t size) {
	return strncmp(str1,str2,size);
}

RUDIMENTS_INLINE int string::compareIgnoringCase(const char *str1,
							const string *str2) {
	return strcasecmp(str1,str2->getString());
}

RUDIMENTS_INLINE int string::compareIgnoringCase(const char *str1,
							const char *str2) {
	return strcasecmp(str1,str2);
}

RUDIMENTS_INLINE int string::compareIgnoringCase(const char *str1,
							const string *str2,
								size_t size) {
	return strncasecmp(str1,str2->getString(),size);
}

RUDIMENTS_INLINE int string::compareIgnoringCase(const char *str1,
							const char *str2,
								size_t size) {
	return strncasecmp(str1,str2,size);
}

RUDIMENTS_INLINE int string::equals(const string *str) const {
	return !strcmp(buffer,str->getString());
}

RUDIMENTS_INLINE int string::equals(const char *str) const {
	return !strcmp(buffer,str);
}

RUDIMENTS_INLINE int string::equals(const string *str, size_t size) const {
	return !strncmp(buffer,str->getString(),size);
}

RUDIMENTS_INLINE int string::equals(const char *str, size_t size) const {
	return !strncmp(buffer,str,size);
}

RUDIMENTS_INLINE int string::equalsIgnoringCase(const string *str) const {
	return !strcasecmp(buffer,str->getString());
}

RUDIMENTS_INLINE int string::equalsIgnoringCase(const char *str) const {
	return !strcasecmp(buffer,str);
}

RUDIMENTS_INLINE int string::equalsIgnoringCase(const string *str,
							size_t size) const {
	return !strncasecmp(buffer,str->getString(),size);
}

RUDIMENTS_INLINE int string::equalsIgnoringCase(const char *str,
							size_t size) const {
	return !strncasecmp(buffer,str,size);
}

RUDIMENTS_INLINE int string::equals(const char *str1, const string *str2) {
	return !strcmp(str1,str2->getString());
}

RUDIMENTS_INLINE int string::equals(const char *str1, const char *str2) {
	return !strcmp(str1,str2);
}

RUDIMENTS_INLINE int string::equals(const char *str1, const string *str2,
								size_t size) {
	return !strncmp(str1,str2->getString(),size);
}

RUDIMENTS_INLINE int string::equals(const char *str1, const char *str2, size_t size) {
	return !strncmp(str1,str2,size);
}

RUDIMENTS_INLINE int string::equalsIgnoringCase(const char *str1,
							const string *str2) {
	return !strcasecmp(str1,str2->getString());
}

RUDIMENTS_INLINE int string::equalsIgnoringCase(const char *str1,
							const char *str2) {
	return !strcasecmp(str1,str2);
}

RUDIMENTS_INLINE int string::equalsIgnoringCase(const char *str1,
							const string *str2,
								size_t size) {
	return !strncasecmp(str1,str2->getString(),size);
}

RUDIMENTS_INLINE int string::equalsIgnoringCase(const char *str1,
							const char *str2,
								size_t size) {
	return !strncasecmp(str1,str2,size);
}

RUDIMENTS_INLINE int string::greaterThan(const string *str) const {
	return strcmp(buffer,str->getString())>0;
}

RUDIMENTS_INLINE int string::greaterThan(const char *str) const {
	return strcmp(buffer,str)>0;
}

RUDIMENTS_INLINE int string::greaterThan(const string *str, size_t size) const {
	return strncmp(buffer,str->getString(),size)>0;
}

RUDIMENTS_INLINE int string::greaterThan(const char *str, size_t size) const {
	return strncmp(buffer,str,size)>0;
}

RUDIMENTS_INLINE int string::greaterThanIgnoringCase(const string *str) const {
	return strcasecmp(buffer,str->getString())>0;
}

RUDIMENTS_INLINE int string::greaterThanIgnoringCase(const char *str) const {
	return strcasecmp(buffer,str)>0;
}

RUDIMENTS_INLINE int string::greaterThanIgnoringCase(const string *str,
							size_t size) const {
	return strncasecmp(buffer,str->getString(),size)>0;
}

RUDIMENTS_INLINE int string::greaterThanIgnoringCase(const char *str,
							size_t size) const {
	return strncasecmp(buffer,str,size)>0;
}

RUDIMENTS_INLINE int string::greaterThan(const char *str1, const string *str2) {
	return strcmp(str1,str2->getString())>0;
}

RUDIMENTS_INLINE int string::greaterThan(const char *str1, const char *str2) {
	return strcmp(str1,str2)>0;
}

RUDIMENTS_INLINE int string::greaterThan(const char *str1, const string *str2,
								size_t size) {
	return strncmp(str1,str2->getString(),size)>0;
}

RUDIMENTS_INLINE int string::greaterThan(const char *str1, const char *str2,
								size_t size) {
	return strncmp(str1,str2,size)>0;
}

RUDIMENTS_INLINE int string::greaterThanIgnoringCase(const char *str1,
							const string *str2) {
	return strcasecmp(str1,str2->getString())>0;
}

RUDIMENTS_INLINE int string::greaterThanIgnoringCase(const char *str1,
							const char *str2) {
	return strcasecmp(str1,str2)>0;
}

RUDIMENTS_INLINE int string::greaterThanIgnoringCase(const char *str1,
							const string *str2,
							size_t size) {
	return strncasecmp(str1,str2->getString(),size)>0;
}

RUDIMENTS_INLINE int string::greaterThanIgnoringCase(const char *str1,
							const char *str2,
							size_t size) {
	return strncasecmp(str1,str2,size)>0;
}

RUDIMENTS_INLINE int string::lessThan(const string *str) const {
	return strcmp(buffer,str->getString())<0;
}

RUDIMENTS_INLINE int string::lessThan(const char *str) const {
	return strcmp(buffer,str)<0;
}

RUDIMENTS_INLINE int string::lessThan(const string *str, size_t size) const {
	return strncmp(buffer,str->getString(),size)<0;
}

RUDIMENTS_INLINE int string::lessThan(const char *str, size_t size) const {
	return strncmp(buffer,str,size)<0;
}

RUDIMENTS_INLINE int string::lessThanIgnoringCase(const string *str) const {
	return strcasecmp(buffer,str->getString())<0;
}

RUDIMENTS_INLINE int string::lessThanIgnoringCase(const char *str) const {
	return strcasecmp(buffer,str)<0;
}

RUDIMENTS_INLINE int string::lessThanIgnoringCase(const string *str,
							size_t size) const {
	return strncasecmp(buffer,str->getString(),size)<0;
}

RUDIMENTS_INLINE int string::lessThanIgnoringCase(const char *str,
							size_t size) const {
	return strncasecmp(buffer,str,size)<0;
}

RUDIMENTS_INLINE int string::lessThan(const char *str1, const string *str2) {
	return strcmp(str1,str2->getString())<0;
}

RUDIMENTS_INLINE int string::lessThan(const char *str1, const char *str2) {
	return strcmp(str1,str2)<0;
}

RUDIMENTS_INLINE int string::lessThan(const char *str1,
					const string *str2, size_t size) {
	return strncmp(str1,str2->getString(),size)<0;
}

RUDIMENTS_INLINE int string::lessThan(const char *str1,
					const char *str2, size_t size) {
	return strncmp(str1,str2,size)<0;
}

RUDIMENTS_INLINE int string::lessThanIgnoringCase(const char *str1,
							const string *str2) {
	return strcasecmp(str1,str2->getString())<0;
}

RUDIMENTS_INLINE int string::lessThanIgnoringCase(const char *str1,
							const char *str2) {
	return strcasecmp(str1,str2)<0;
}

RUDIMENTS_INLINE int string::lessThanIgnoringCase(const char *str1,
							const string *str2,
								size_t size) {
	return strncasecmp(str1,str2->getString(),size)<0;
}

RUDIMENTS_INLINE int string::lessThanIgnoringCase(const char *str1,
							const char *str2,
								size_t size) {
	return strncasecmp(str1,str2,size)<0;
}

RUDIMENTS_INLINE char *string::contains(const string *str) const {
	return strstr(buffer,str->getString());
}

RUDIMENTS_INLINE char *string::contains(const char *str) const {
	return strstr(buffer,str);
}

RUDIMENTS_INLINE char *string::contains(const char ch) const {
	return strchr(buffer,ch);
}

RUDIMENTS_INLINE char *string::contains(const char *haystack,
							const string *needle) {
	return strstr(haystack,needle->getString());
}

RUDIMENTS_INLINE char *string::contains(const char *haystack,
							const char *needle) {
	return strstr(haystack,needle);
}

RUDIMENTS_INLINE char *string::contains(const char *haystack,
							const char needle) {
	return strchr(haystack,needle);
}

RUDIMENTS_INLINE char *string::duplicate() const {
	return strdup(buffer);
}

RUDIMENTS_INLINE char *string::duplicate(const char *str) {
	return strdup(str);
}
