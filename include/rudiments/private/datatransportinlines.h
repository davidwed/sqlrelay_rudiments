// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <netinet/in.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif


INLINE datatransport::datatransport() : filedescriptor() {}

INLINE datatransport::datatransport(int fd) : filedescriptor(fd) {}

INLINE ssize_t datatransport::write(unsigned short number) {
	unsigned short	temp=htons(number);
	return safeWrite((void *)&temp,sizeof(unsigned short));
}

INLINE ssize_t datatransport::write(unsigned long number) {
	unsigned long	temp=htonl(number);
	return safeWrite((void *)&temp,sizeof(unsigned long));
}

INLINE ssize_t datatransport::write(float number) {
	return filedescriptor::write((void *)&number,sizeof(float));
}

INLINE ssize_t datatransport::write(double number) {
	return filedescriptor::write((void *)&number,sizeof(double));
}

INLINE ssize_t datatransport::write(char character) {
	return filedescriptor::write((void *)&character,sizeof(char));
}

INLINE ssize_t datatransport::write(const char *string) {
	return filedescriptor::write(string,strlen(string));
}

INLINE ssize_t datatransport::write(const char *string, size_t size) {
	return filedescriptor::write(string,size);
}

INLINE ssize_t datatransport::write(const void *buffer, size_t size) {
	return filedescriptor::write((void *)buffer,size);
}

INLINE ssize_t datatransport::read(unsigned short *buffer) {
	unsigned short	temp;
	int	retval=(int)safeRead((void *)&temp,sizeof(unsigned short));
	*buffer=ntohs(temp);
	return retval;
}

INLINE ssize_t datatransport::read(unsigned long *buffer) {
	unsigned long	temp;
	int	retval=(int)safeRead((void *)&temp,sizeof(unsigned long));
	*buffer=ntohl(temp);
	return retval;
}

INLINE ssize_t datatransport::read(float *buffer) {
	return filedescriptor::read((void *)buffer,sizeof(float));
}

INLINE ssize_t datatransport::read(double *buffer) {
	return filedescriptor::read((void *)buffer,sizeof(double));
}

INLINE ssize_t datatransport::read(char *buffer) {
	return filedescriptor::read((void *)buffer,sizeof(char));
}

INLINE ssize_t datatransport::read(char *buffer, size_t size) {
	return filedescriptor::read((void *)buffer,size);
}

INLINE ssize_t datatransport::read(void *buffer, size_t size) {
	return filedescriptor::read(buffer,size);
}

INLINE ssize_t datatransport::read(char **buffer, char *terminator) {
	return filedescriptor::read(buffer,terminator);
}

INLINE int datatransport::passFileDescriptor(int descriptor) {
	// by default do nothing and return an error
	return 0;
}

INLINE int datatransport::receiveFileDescriptor(int *descriptor) {
	// by default do nothing and return an error
	return 0;
}
