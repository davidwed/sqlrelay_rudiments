// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <string.h>
#ifdef STRINGS_H
	#include <strings.h>
#endif

inline string::string(const char *str) {
	buffer=strdup(str);
	this->size=strlen(str);
}

inline string::string(const char *str, size_t size) {
	memcpy((void *)buffer,(void *)str,size);
	this->size=size;
}

inline string::string(size_t size) {
	buffer=new char[size];
	this->size=size;
}

inline string::~string() {
	delete[] buffer;
}

inline char *string::getString() const {
	return buffer;
}

inline size_t string::getLength() const {
	return strlen(buffer);
}

inline size_t string::getSize() const {
	return size;
}

inline void string::zero() {
	memset((void *)buffer,0,size);
}

inline void string::zero(char *str, size_t size) {
	memset((void *)str,0,size);
}

inline char *string::append(const string *str) {
	return strcat(buffer,str->getString());
}

inline char *string::append(const char *str) {
	return strcat(buffer,str);
}

inline char *string::append(const string *str, size_t size) {
	return strncat(buffer,str->getString(),size);
}

inline char *string::append(const char *str, size_t size) {
	return strncat(buffer,str,size);
}

inline char *string::append(char *dest, const string *source) {
	return strcat(dest,source->getString());
}

inline char *string::append(char *dest, const char *source) {
	return strcat(dest,source);
}

inline char *string::append(char *dest, const string *source, size_t size) {
	return strncat(dest,source->getString(),size);
}

inline char *string::append(char *dest, const char *source, size_t size) {
	return strncat(dest,source,size);
}

inline char *string::copyInto(const string *str) {
	return strcpy(buffer,str->getString());
}

inline char *string::copyInto(const char *str) {
	return strcpy(buffer,str);
}

inline char *string::copyInto(const char *str, size_t size) {
	return strncpy(buffer,str,size);
}

inline char *string::copyInto(unsigned long location,
					const string *str) {
	return strcpy(buffer+location,str->getString());
}

inline char *string::copyInto(unsigned long location,
					const char *str) {
	return strcpy(buffer+location,str);
}

inline char *string::copyInto(unsigned long location,
					const char *str, size_t size) {
	return strncpy(buffer+location,str,size);
}

inline char *string::copyInto(char *dest, const string *source) {
	return strcpy(dest,source->getString());
}

inline char *string::copyInto(char *dest, const char *source) {
	return strcpy(dest,source);
}

inline char *string::copyInto(char *dest, const char *source, size_t size) {
	return strncpy(dest,source,size);
}

inline char *string::copyInto(char *dest, unsigned long location,
						const string *source) {
	return strcpy(dest+location,source->getString());
}

inline char *string::copyInto(char *dest, unsigned long location,
						const char *source) {
	return strcpy(dest+location,source);
}

inline char *string::copyInto(char *dest, unsigned long location,
					const char *source, size_t size) {
	return strncpy(dest+location,source,size);
}

inline int string::compare(const string *str) const {
	return strcmp(buffer,str->getString());
}

inline int string::compare(const char *str) const {
	return strcmp(buffer,str);
}

inline int string::compare(const string *str, size_t size) const {
	return strncmp(buffer,str->getString(),size);
}

inline int string::compare(const char *str, size_t size) const {
	return strncmp(buffer,str,size);
}

inline int string::compareIgnoringCase(const string *str) const {
	return strcasecmp(buffer,str->getString());
}

inline int string::compareIgnoringCase(const char *str) const {
	return strcasecmp(buffer,str);
}

inline int string::compareIgnoringCase(const string *str, size_t size) const {
	return strncasecmp(buffer,str->getString(),size);
}

inline int string::compareIgnoringCase(const char *str, size_t size) const {
	return strncasecmp(buffer,str,size);
}

inline int string::compare(const char *str1, const string *str2) {
	return strcmp(str1,str2->getString());
}

inline int string::compare(const char *str1, const char *str2) {
	return strcmp(str1,str2);
}

inline int string::compare(const char *str1, const string *str2, size_t size) {
	return strncmp(str1,str2->getString(),size);
}

inline int string::compare(const char *str1, const char *str2, size_t size) {
	return strncmp(str1,str2,size);
}

inline int string::compareIgnoringCase(const char *str1, const string *str2) {
	return strcasecmp(str1,str2->getString());
}

inline int string::compareIgnoringCase(const char *str1, const char *str2) {
	return strcasecmp(str1,str2);
}

inline int string::compareIgnoringCase(const char *str1, const string *str2,
								size_t size) {
	return strncasecmp(str1,str2->getString(),size);
}

inline int string::compareIgnoringCase(const char *str1, const char *str2,
								size_t size) {
	return strncasecmp(str1,str2,size);
}

inline int string::equals(const string *str) const {
	return !strcmp(buffer,str->getString());
}

inline int string::equals(const char *str) const {
	return !strcmp(buffer,str);
}

inline int string::equals(const string *str, size_t size) const {
	return !strncmp(buffer,str->getString(),size);
}

inline int string::equals(const char *str, size_t size) const {
	return !strncmp(buffer,str,size);
}

inline int string::equalsIgnoringCase(const string *str) const {
	return !strcasecmp(buffer,str->getString());
}

inline int string::equalsIgnoringCase(const char *str) const {
	return !strcasecmp(buffer,str);
}

inline int string::equalsIgnoringCase(const string *str, size_t size) const {
	return !strncasecmp(buffer,str->getString(),size);
}

inline int string::equalsIgnoringCase(const char *str, size_t size) const {
	return !strncasecmp(buffer,str,size);
}

inline int string::equals(const char *str1, const string *str2) {
	return !strcmp(str1,str2->getString());
}

inline int string::equals(const char *str1, const char *str2) {
	return !strcmp(str1,str2);
}

inline int string::equals(const char *str1, const string *str2, size_t size) {
	return !strncmp(str1,str2->getString(),size);
}

inline int string::equals(const char *str1, const char *str2, size_t size) {
	return !strncmp(str1,str2,size);
}

inline int string::equalsIgnoringCase(const char *str1, const string *str2) {
	return !strcasecmp(str1,str2->getString());
}

inline int string::equalsIgnoringCase(const char *str1, const char *str2) {
	return !strcasecmp(str1,str2);
}

inline int string::equalsIgnoringCase(const char *str1, const string *str2,
								size_t size) {
	return !strncasecmp(str1,str2->getString(),size);
}

inline int string::equalsIgnoringCase(const char *str1, const char *str2,
								size_t size) {
	return !strncasecmp(str1,str2,size);
}

inline int string::greaterThan(const string *str) const {
	return strcmp(buffer,str->getString())>0;
}

inline int string::greaterThan(const char *str) const {
	return strcmp(buffer,str)>0;
}

inline int string::greaterThan(const string *str, size_t size) const {
	return strncmp(buffer,str->getString(),size)>0;
}

inline int string::greaterThan(const char *str, size_t size) const {
	return strncmp(buffer,str,size)>0;
}

inline int string::greaterThanIgnoringCase(const string *str) const {
	return strcasecmp(buffer,str->getString())>0;
}

inline int string::greaterThanIgnoringCase(const char *str) const {
	return strcasecmp(buffer,str)>0;
}

inline int string::greaterThanIgnoringCase(const string *str,
							size_t size) const {
	return strncasecmp(buffer,str->getString(),size)>0;
}

inline int string::greaterThanIgnoringCase(const char *str, size_t size) const {
	return strncasecmp(buffer,str,size)>0;
}

inline int string::greaterThan(const char *str1, const string *str2) {
	return strcmp(str1,str2->getString())>0;
}

inline int string::greaterThan(const char *str1, const char *str2) {
	return strcmp(str1,str2)>0;
}

inline int string::greaterThan(const char *str1, const string *str2,
								size_t size) {
	return strncmp(str1,str2->getString(),size)>0;
}

inline int string::greaterThan(const char *str1, const char *str2,
								size_t size) {
	return strncmp(str1,str2,size)>0;
}

inline int string::greaterThanIgnoringCase(const char *str1,
							const string *str2) {
	return strcasecmp(str1,str2->getString())>0;
}

inline int string::greaterThanIgnoringCase(const char *str1,
							const char *str2) {
	return strcasecmp(str1,str2)>0;
}

inline int string::greaterThanIgnoringCase(const char *str1,
							const string *str2,
							size_t size) {
	return strncasecmp(str1,str2->getString(),size)>0;
}

inline int string::greaterThanIgnoringCase(const char *str1,
							const char *str2,
							size_t size) {
	return strncasecmp(str1,str2,size)>0;
}

inline int string::lessThan(const string *str) const {
	return strcmp(buffer,str->getString())<0;
}

inline int string::lessThan(const char *str) const {
	return strcmp(buffer,str)<0;
}

inline int string::lessThan(const string *str, size_t size) const {
	return strncmp(buffer,str->getString(),size)<0;
}

inline int string::lessThan(const char *str, size_t size) const {
	return strncmp(buffer,str,size)<0;
}

inline int string::lessThanIgnoringCase(const string *str) const {
	return strcasecmp(buffer,str->getString())<0;
}

inline int string::lessThanIgnoringCase(const char *str) const {
	return strcasecmp(buffer,str)<0;
}

inline int string::lessThanIgnoringCase(const string *str, size_t size) const {
	return strncasecmp(buffer,str->getString(),size)<0;
}

inline int string::lessThanIgnoringCase(const char *str, size_t size) const {
	return strncasecmp(buffer,str,size)<0;
}

inline int string::lessThan(const char *str1, const string *str2) {
	return strcmp(str1,str2->getString())<0;
}

inline int string::lessThan(const char *str1, const char *str2) {
	return strcmp(str1,str2)<0;
}

inline int string::lessThan(const char *str1, const string *str2, size_t size) {
	return strncmp(str1,str2->getString(),size)<0;
}

inline int string::lessThan(const char *str1, const char *str2, size_t size) {
	return strncmp(str1,str2,size)<0;
}

inline int string::lessThanIgnoringCase(const char *str1, const string *str2) {
	return strcasecmp(str1,str2->getString())<0;
}

inline int string::lessThanIgnoringCase(const char *str1, const char *str2) {
	return strcasecmp(str1,str2)<0;
}

inline int string::lessThanIgnoringCase(const char *str1, const string *str2,
								size_t size) {
	return strncasecmp(str1,str2->getString(),size)<0;
}

inline int string::lessThanIgnoringCase(const char *str1, const char *str2,
								size_t size) {
	return strncasecmp(str1,str2,size)<0;
}

inline char *string::contains(const string *str) const {
	return strstr(buffer,str->getString());
}

inline char *string::contains(const char *str) const {
	return strstr(buffer,str);
}

inline char *string::contains(const char ch) const {
	return strchr(buffer,ch);
}

inline char *string::contains(const char *haystack, const string *needle) {
	return strstr(haystack,needle->getString());
}

inline char *string::contains(const char *haystack, const char *needle) {
	return strstr(haystack,needle);
}

inline char *string::contains(const char *haystack, const char needle) {
	return strchr(haystack,needle);
}

inline char *string::duplicate() const {
	return strdup(buffer);
}

inline char *string::duplicate(const char *str) {
	return strdup(str);
}
