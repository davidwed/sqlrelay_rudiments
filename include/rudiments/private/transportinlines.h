// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

//#include <rudiments/private/config.h>

#include <netinet/in.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#include <stdio.h>

inline transport::transport(int filedescriptor) {
	this->filedescriptor=filedescriptor;
	retryinterruptedreads=0;
	retryinterruptedwrites=0;
}

inline transport::transport() {
	this->filedescriptor=-1;
	retryinterruptedreads=0;
	retryinterruptedwrites=0;
}

inline	transport::~transport() {
	close();
}

inline int	transport::getFileDescriptor() const {
	return filedescriptor;
}

inline ssize_t	transport::write(unsigned short number) {
	unsigned short	temp=htons(number);
	return safeWrite(filedescriptor,(void *)&temp,sizeof(unsigned short));
}

inline ssize_t	transport::write(unsigned long number) {
	unsigned long	temp=htonl(number);
	return safeWrite(filedescriptor,(void *)&temp,sizeof(unsigned long));
}

inline ssize_t	transport::write(float number) {
	return safeWrite(filedescriptor,(void *)&number,sizeof(float));
}

inline ssize_t	transport::write(double number) {
	return safeWrite(filedescriptor,(void *)&number,sizeof(double));
}

inline ssize_t	transport::write(char character) {
	return safeWrite(filedescriptor,(void *)&character,sizeof(char));
}

inline ssize_t	transport::write(const char *string) {
	return safeWrite(filedescriptor,string,strlen(string));
}

inline ssize_t	transport::write(const char *string, size_t size) {
	return safeWrite(filedescriptor,string,size);
}

inline ssize_t	transport::write(const void *buffer, size_t size) {
	return safeWrite(filedescriptor,(void *)buffer,size);
}

inline ssize_t	transport::read(unsigned short *buffer) {
	unsigned short	temp;
	int	retval=(int)safeRead(filedescriptor,(void *)&temp,
					sizeof(unsigned short));
	*buffer=ntohs(temp);
	return retval;
}

inline ssize_t	transport::read(unsigned long *buffer) {
	unsigned long	temp;
	int	retval=(int)safeRead(filedescriptor,(void *)&temp,
					sizeof(unsigned long));
	*buffer=ntohl(temp);
	return retval;
}

inline ssize_t	transport::read(float *buffer) {
	return safeRead(filedescriptor,(void *)buffer,sizeof(float));
}

inline ssize_t	transport::read(double *buffer) {
	return safeRead(filedescriptor,(void *)buffer,sizeof(double));
}

inline ssize_t	transport::read(char *buffer) {
	return safeRead(filedescriptor,(void *)buffer,sizeof(char));
}

inline ssize_t	transport::read(char *buffer, size_t size) {
	return safeRead(filedescriptor,(void *)buffer,size);
}

inline ssize_t	transport::read(void *buffer, size_t size) {
	return safeRead(filedescriptor,buffer,size);
}

inline int	transport::passFileDescriptor(int descriptor) {
	// by default do nothing and return an error
	return 0;
}

inline int	transport::receiveFileDescriptor(int *descriptor) {
	// by default do nothing and return an error
	return 0;
}

inline int	transport::close() {

	// check for an uninitialized file descriptor
	if (filedescriptor==-1) {
		return 0;
	}

	// close the file descriptor
	if (::close(filedescriptor)==-1) {
		return 0;
	}

	filedescriptor=-1;
	return 1;
}

inline void	transport::retryInterruptedReads() {
	retryinterruptedreads=1;
}

inline void	transport::dontRetryInterruptedReads() {
	retryinterruptedreads=0;
}

inline void	transport::retryInterruptedWrites() {
	retryinterruptedwrites=1;
}

inline void	transport::dontRetryInterruptedWrites() {
	retryinterruptedwrites=0;
}
