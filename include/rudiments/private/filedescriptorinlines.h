// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

inline filedescriptor::filedescriptor(int fd) {
	this->fd=fd;
	retryinterruptedreads=0;
	retryinterruptedwrites=0;
}

inline filedescriptor::filedescriptor() {
	this->fd=-1;
	retryinterruptedreads=0;
	retryinterruptedwrites=0;
}

inline	filedescriptor::~filedescriptor() {
	close();
}

inline int	filedescriptor::getFileDescriptor() const {
	return fd;
}

inline void	filedescriptor::setFileDescriptor(int fd) {
	this->fd=fd;
}

inline ssize_t	filedescriptor::write(unsigned short number) {
	return safeWrite((void *)&number,sizeof(unsigned short));
}

inline ssize_t	filedescriptor::write(unsigned long number) {
	return safeWrite((void *)&number,sizeof(unsigned long));
}

inline ssize_t	filedescriptor::write(float number) {
	return safeWrite((void *)&number,sizeof(float));
}

inline ssize_t	filedescriptor::write(double number) {
	return safeWrite((void *)&number,sizeof(double));
}

inline ssize_t	filedescriptor::write(char character) {
	return safeWrite((void *)&character,sizeof(char));
}

inline ssize_t	filedescriptor::write(const char *string) {
	return safeWrite(string,strlen(string));
}

inline ssize_t	filedescriptor::write(const char *string, size_t size) {
	return safeWrite(string,size);
}

inline ssize_t	filedescriptor::write(const void *buffer, size_t size) {
	return safeWrite((void *)buffer,size);
}

inline ssize_t	filedescriptor::read(unsigned short *buffer) {
	return safeRead((void *)buffer,sizeof(unsigned short));
}

inline ssize_t	filedescriptor::read(unsigned long *buffer) {
	return safeRead((void *)buffer,sizeof(unsigned long));
}

inline ssize_t	filedescriptor::read(float *buffer) {
	return safeRead((void *)buffer,sizeof(float));
}

inline ssize_t	filedescriptor::read(double *buffer) {
	return safeRead((void *)buffer,sizeof(double));
}

inline ssize_t	filedescriptor::read(char *buffer) {
	return safeRead((void *)buffer,sizeof(char));
}

inline ssize_t	filedescriptor::read(char *buffer, size_t size) {
	return safeRead((void *)buffer,size);
}

inline ssize_t	filedescriptor::read(void *buffer, size_t size) {
	return safeRead(buffer,size);
}

inline int	filedescriptor::close() {
	if (::close(fd)==-1) {
		return 0;
	}
	fd=-1;
	return 1;
}

inline void	filedescriptor::retryInterruptedReads() {
	retryinterruptedreads=1;
}

inline void	filedescriptor::dontRetryInterruptedReads() {
	retryinterruptedreads=0;
}

inline void	filedescriptor::retryInterruptedWrites() {
	retryinterruptedwrites=1;
}

inline void	filedescriptor::dontRetryInterruptedWrites() {
	retryinterruptedwrites=0;
}

inline void	filedescriptor::retryInterruptedWaits() {
	retryinterruptedwaits=1;
}

inline void	filedescriptor::dontRetryInterruptedWaits() {
	retryinterruptedwaits=0;
}
