// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE filedescriptor::filedescriptor(int fd) {
	this->fd=fd;
	retryinterruptedreads=0;
	retryinterruptedwrites=0;
	lstnr=NULL;
}

RUDIMENTS_INLINE filedescriptor::filedescriptor() {
	this->fd=-1;
	retryinterruptedreads=0;
	retryinterruptedwrites=0;
	lstnr=NULL;
}

RUDIMENTS_INLINE filedescriptor::~filedescriptor() {
	close();
}

RUDIMENTS_INLINE int filedescriptor::getFileDescriptor() const {
	return fd;
}

RUDIMENTS_INLINE void filedescriptor::setFileDescriptor(int fd) {
	this->fd=fd;
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(unsigned short number) {
	return safeWrite((void *)&number,sizeof(unsigned short));
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(unsigned long number) {
	return safeWrite((void *)&number,sizeof(unsigned long));
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(short number) {
	return safeWrite((void *)&number,sizeof(short));
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(long number) {
	return safeWrite((void *)&number,sizeof(long));
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(float number) {
	return safeWrite((void *)&number,sizeof(float));
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(double number) {
	return safeWrite((void *)&number,sizeof(double));
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(unsigned char character) {
	return safeWrite((void *)&character,sizeof(unsigned char));
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(char character) {
	return safeWrite((void *)&character,sizeof(char));
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(const unsigned char *string) {
	return safeWrite((void *)string,strlen((char *)string));
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(const char *string) {
	return safeWrite((void *)string,strlen(string));
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(const unsigned char *string,
								size_t size) {
	return safeWrite((void *)string,size);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(const char *string,
								size_t size) {
	return safeWrite((void *)string,size);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(const void *buffer,
								size_t size) {
	return safeWrite((void *)buffer,size);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(unsigned short *buffer) {
	return safeRead((void *)buffer,sizeof(unsigned short));
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(unsigned long *buffer) {
	return safeRead((void *)buffer,sizeof(unsigned long));
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(short *buffer) {
	return safeRead((void *)buffer,sizeof(short));
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(long *buffer) {
	return safeRead((void *)buffer,sizeof(long));
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(float *buffer) {
	return safeRead((void *)buffer,sizeof(float));
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(double *buffer) {
	return safeRead((void *)buffer,sizeof(double));
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(unsigned char *buffer) {
	return safeRead((void *)buffer,sizeof(unsigned char));
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(char *buffer) {
	return safeRead((void *)buffer,sizeof(char));
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(unsigned char *buffer,
								size_t size) {
	return safeRead((void *)buffer,size);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(char *buffer, size_t size) {
	return safeRead((void *)buffer,size);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(void *buffer, size_t size) {
	return safeRead(buffer,size);
}

RUDIMENTS_INLINE int filedescriptor::close() {
	if (::close(fd)==-1) {
		return 0;
	}
	fd=-1;
	return 1;
}

RUDIMENTS_INLINE void filedescriptor::retryInterruptedReads() {
	retryinterruptedreads=1;
}

RUDIMENTS_INLINE void filedescriptor::dontRetryInterruptedReads() {
	retryinterruptedreads=0;
}

RUDIMENTS_INLINE void filedescriptor::retryInterruptedWrites() {
	retryinterruptedwrites=1;
}

RUDIMENTS_INLINE void filedescriptor::dontRetryInterruptedWrites() {
	retryinterruptedwrites=0;
}

RUDIMENTS_INLINE void filedescriptor::retryInterruptedWaits() {
	retryinterruptedwaits=1;
}

RUDIMENTS_INLINE void filedescriptor::dontRetryInterruptedWaits() {
	retryinterruptedwaits=0;
}

RUDIMENTS_INLINE void filedescriptor::useListener(listener *lstnr) {
	this->lstnr=lstnr;
}

RUDIMENTS_INLINE void filedescriptor::dontUseListener() {
	this->lstnr=NULL;
}
