// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE filedescriptor::filedescriptor(int fd) {
	this->fd=fd;
	retryinterruptedreads=0;
	retryinterruptedwrites=0;
	allowshortreads=0;
	lstnr=NULL;
}

RUDIMENTS_INLINE filedescriptor::filedescriptor() {
	this->fd=-1;
	retryinterruptedreads=0;
	retryinterruptedwrites=0;
	allowshortreads=0;
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
	return safeWrite((void *)&number,sizeof(unsigned short),-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(unsigned long number) {
	return safeWrite((void *)&number,sizeof(unsigned long),-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(short number) {
	return safeWrite((void *)&number,sizeof(short),-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(long number) {
	return safeWrite((void *)&number,sizeof(long),-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(float number) {
	return safeWrite((void *)&number,sizeof(float),-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(double number) {
	return safeWrite((void *)&number,sizeof(double),-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(unsigned char character) {
	return safeWrite((void *)&character,sizeof(unsigned char),-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(char character) {
	return safeWrite((void *)&character,sizeof(char),-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(const unsigned char *string,
								size_t size) {
	return safeWrite((void *)string,size,-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(const char *string,
								size_t size) {
	return safeWrite((void *)string,size,-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(const void *buffer,
								size_t size) {
	return safeWrite((void *)buffer,size,-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(unsigned short number,
							long sec, long usec) {
	return safeWrite((void *)&number,sizeof(unsigned short),sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(unsigned long number,
							long sec, long usec) {
	return safeWrite((void *)&number,sizeof(unsigned long),sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(short number,
							long sec, long usec) {
	return safeWrite((void *)&number,sizeof(short),sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(long number,
							long sec, long usec) {
	return safeWrite((void *)&number,sizeof(long),sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(float number,
							long sec, long usec) {
	return safeWrite((void *)&number,sizeof(float),sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(double number,
							long sec, long usec) {
	return safeWrite((void *)&number,sizeof(double),sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(unsigned char character,
							long sec, long usec) {
	return safeWrite((void *)&character,sizeof(unsigned char),sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(char character,
							long sec, long usec) {
	return safeWrite((void *)&character,sizeof(char),sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(const unsigned char *string,
							size_t size,
							long sec, long usec) {
	return safeWrite((void *)string,size,sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(const char *string,
							size_t size,
							long sec, long usec) {
	return safeWrite((void *)string,size,sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::write(const void *buffer,
							size_t size,
							long sec, long usec) {
	return safeWrite((void *)buffer,size,sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(unsigned short *buffer) {
	return safeRead((void *)buffer,sizeof(unsigned short),-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(unsigned long *buffer) {
	return safeRead((void *)buffer,sizeof(unsigned long),-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(short *buffer) {
	return safeRead((void *)buffer,sizeof(short),-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(long *buffer) {
	return safeRead((void *)buffer,sizeof(long),-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(float *buffer) {
	return safeRead((void *)buffer,sizeof(float),-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(double *buffer) {
	return safeRead((void *)buffer,sizeof(double),-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(unsigned char *buffer) {
	return safeRead((void *)buffer,sizeof(unsigned char),-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(char *buffer) {
	return safeRead((void *)buffer,sizeof(char),-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(unsigned char *buffer,
								size_t size) {
	return safeRead((void *)buffer,size,-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(char *buffer, size_t size) {
	return safeRead((void *)buffer,size,-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(void *buffer, size_t size) {
	return safeRead(buffer,size,-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(char **buffer, char *terminator) {
	return read(buffer,terminator,-1,-1);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(unsigned short *buffer,
							long sec, long usec) {
	return safeRead((void *)buffer,sizeof(unsigned short),sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(unsigned long *buffer,
							long sec, long usec) {
	return safeRead((void *)buffer,sizeof(unsigned long),sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(short *buffer,
							long sec, long usec) {
	return safeRead((void *)buffer,sizeof(short),sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(long *buffer,
							long sec, long usec) {
	return safeRead((void *)buffer,sizeof(long),sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(float *buffer,
							long sec, long usec) {
	return safeRead((void *)buffer,sizeof(float),sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(double *buffer,
							long sec, long usec) {
	return safeRead((void *)buffer,sizeof(double),sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(unsigned char *buffer,
							long sec, long usec) {
	return safeRead((void *)buffer,sizeof(unsigned char),sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(char *buffer,
							long sec, long usec) {
	return safeRead((void *)buffer,sizeof(char),sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(unsigned char *buffer,
							size_t size,
							long sec, long usec) {
	return safeRead((void *)buffer,size,sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(char *buffer, size_t size,
							long sec, long usec) {
	return safeRead((void *)buffer,size,sec,usec);
}

RUDIMENTS_INLINE ssize_t filedescriptor::read(void *buffer, size_t size,
							long sec, long usec) {
	return safeRead(buffer,size,sec,usec);
}

RUDIMENTS_INLINE bool filedescriptor::close() {
	if (::close(fd)==-1) {
		return false;
	}
	fd=-1;
	return true;
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

RUDIMENTS_INLINE void filedescriptor::allowShortReads() {
	allowshortreads=1;
}

RUDIMENTS_INLINE void filedescriptor::dontAllowShortReads() {
	allowshortreads=0;
}

RUDIMENTS_INLINE void filedescriptor::useListener(listener *lstnr) {
	this->lstnr=lstnr;
}

RUDIMENTS_INLINE void filedescriptor::dontUseListener() {
	this->lstnr=NULL;
}
