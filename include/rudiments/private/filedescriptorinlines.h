// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

INLINE filedescriptor::filedescriptor(int fd) {
	this->fd=fd;
	retryinterruptedreads=0;
	retryinterruptedwrites=0;
	lstnr=NULL;
}

INLINE filedescriptor::filedescriptor() {
	this->fd=-1;
	retryinterruptedreads=0;
	retryinterruptedwrites=0;
	lstnr=NULL;
}

INLINE filedescriptor::~filedescriptor() {
	close();
}

INLINE int filedescriptor::getFileDescriptor() const {
	return fd;
}

INLINE void filedescriptor::setFileDescriptor(int fd) {
	this->fd=fd;
}

INLINE ssize_t filedescriptor::write(unsigned short number) {
	return safeWrite((void *)&number,sizeof(unsigned short));
}

INLINE ssize_t filedescriptor::write(unsigned long number) {
	return safeWrite((void *)&number,sizeof(unsigned long));
}

INLINE ssize_t filedescriptor::write(short number) {
	return safeWrite((void *)&number,sizeof(short));
}

INLINE ssize_t filedescriptor::write(long number) {
	return safeWrite((void *)&number,sizeof(long));
}

INLINE ssize_t filedescriptor::write(float number) {
	return safeWrite((void *)&number,sizeof(float));
}

INLINE ssize_t filedescriptor::write(double number) {
	return safeWrite((void *)&number,sizeof(double));
}

INLINE ssize_t filedescriptor::write(unsigned char character) {
	return safeWrite((void *)&character,sizeof(unsigned char));
}

INLINE ssize_t filedescriptor::write(char character) {
	return safeWrite((void *)&character,sizeof(char));
}

INLINE ssize_t filedescriptor::write(const unsigned char *string) {
	return safeWrite((void *)string,strlen((char *)string));
}

INLINE ssize_t filedescriptor::write(const char *string) {
	return safeWrite((void *)string,strlen(string));
}

INLINE ssize_t filedescriptor::write(const unsigned char *string, size_t size) {
	return safeWrite((void *)string,size);
}

INLINE ssize_t filedescriptor::write(const char *string, size_t size) {
	return safeWrite((void *)string,size);
}

INLINE ssize_t filedescriptor::write(const void *buffer, size_t size) {
	return safeWrite((void *)buffer,size);
}

INLINE ssize_t filedescriptor::read(unsigned short *buffer) {
	return safeRead((void *)buffer,sizeof(unsigned short));
}

INLINE ssize_t filedescriptor::read(unsigned long *buffer) {
	return safeRead((void *)buffer,sizeof(unsigned long));
}

INLINE ssize_t filedescriptor::read(short *buffer) {
	return safeRead((void *)buffer,sizeof(short));
}

INLINE ssize_t filedescriptor::read(long *buffer) {
	return safeRead((void *)buffer,sizeof(long));
}

INLINE ssize_t filedescriptor::read(float *buffer) {
	return safeRead((void *)buffer,sizeof(float));
}

INLINE ssize_t filedescriptor::read(double *buffer) {
	return safeRead((void *)buffer,sizeof(double));
}

INLINE ssize_t filedescriptor::read(unsigned char *buffer) {
	return safeRead((void *)buffer,sizeof(unsigned char));
}

INLINE ssize_t filedescriptor::read(char *buffer) {
	return safeRead((void *)buffer,sizeof(char));
}

INLINE ssize_t filedescriptor::read(unsigned char *buffer, size_t size) {
	return safeRead((void *)buffer,size);
}

INLINE ssize_t filedescriptor::read(char *buffer, size_t size) {
	return safeRead((void *)buffer,size);
}

INLINE ssize_t filedescriptor::read(void *buffer, size_t size) {
	return safeRead(buffer,size);
}

INLINE int filedescriptor::close() {
	if (::close(fd)==-1) {
		return 0;
	}
	fd=-1;
	return 1;
}

INLINE void filedescriptor::retryInterruptedReads() {
	retryinterruptedreads=1;
}

INLINE void filedescriptor::dontRetryInterruptedReads() {
	retryinterruptedreads=0;
}

INLINE void filedescriptor::retryInterruptedWrites() {
	retryinterruptedwrites=1;
}

INLINE void filedescriptor::dontRetryInterruptedWrites() {
	retryinterruptedwrites=0;
}

INLINE void filedescriptor::retryInterruptedWaits() {
	retryinterruptedwaits=1;
}

INLINE void filedescriptor::dontRetryInterruptedWaits() {
	retryinterruptedwaits=0;
}

INLINE void filedescriptor::useListener(listener *lstnr) {
	this->lstnr=lstnr;
}

INLINE void filedescriptor::dontUseListener() {
	this->lstnr=NULL;
}
