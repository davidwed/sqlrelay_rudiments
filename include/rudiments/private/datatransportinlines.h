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

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE datatransport::datatransport() : filedescriptor() {}

RUDIMENTS_INLINE datatransport::datatransport(int fd) : filedescriptor(fd) {}

RUDIMENTS_INLINE ssize_t datatransport::write(unsigned short number) {
	unsigned short	temp=htons(number);
	return safeWrite((void *)&temp,sizeof(unsigned short),-1,-1);
}

RUDIMENTS_INLINE ssize_t datatransport::write(unsigned long number) {
	unsigned long	temp=htonl(number);
	return safeWrite((void *)&temp,sizeof(unsigned long),-1,-1);
}

RUDIMENTS_INLINE ssize_t datatransport::write(float number) {
	return filedescriptor::write((void *)&number,sizeof(float));
}

RUDIMENTS_INLINE ssize_t datatransport::write(double number) {
	return filedescriptor::write((void *)&number,sizeof(double));
}

RUDIMENTS_INLINE ssize_t datatransport::write(char character) {
	return filedescriptor::write((void *)&character,sizeof(char));
}

RUDIMENTS_INLINE ssize_t datatransport::write(const char *string) {
	return filedescriptor::write(string,strlen(string));
}

RUDIMENTS_INLINE ssize_t datatransport::write(const char *string, size_t size) {
	return filedescriptor::write(string,size);
}

RUDIMENTS_INLINE ssize_t datatransport::write(const void *buffer, size_t size) {
	return filedescriptor::write((void *)buffer,size);
}

RUDIMENTS_INLINE ssize_t datatransport::write(unsigned short number,
							long sec, long usec) {
	unsigned short	temp=htons(number);
	return safeWrite((void *)&temp,sizeof(unsigned short),sec,usec);
}

RUDIMENTS_INLINE ssize_t datatransport::write(unsigned long number,
							long sec, long usec) {
	unsigned long	temp=htonl(number);
	return safeWrite((void *)&temp,sizeof(unsigned long),sec,usec);
}

RUDIMENTS_INLINE ssize_t datatransport::write(float number,
							long sec, long usec) {
	return filedescriptor::write((void *)&number,sizeof(float),sec,usec);
}

RUDIMENTS_INLINE ssize_t datatransport::write(double number,
							long sec, long usec) {
	return filedescriptor::write((void *)&number,sizeof(double),sec,usec);
}

RUDIMENTS_INLINE ssize_t datatransport::write(char character,
							long sec, long usec) {
	return filedescriptor::write((void *)&character,sizeof(char),sec,usec);
}

RUDIMENTS_INLINE ssize_t datatransport::write(const char *string,
							long sec, long usec) {
	return filedescriptor::write(string,strlen(string),sec,usec);
}

RUDIMENTS_INLINE ssize_t datatransport::write(const char *string, size_t size,
							long sec, long usec) {
	return filedescriptor::write(string,size,sec,usec);
}

RUDIMENTS_INLINE ssize_t datatransport::write(const void *buffer, size_t size,
							long sec, long usec) {
	return filedescriptor::write((void *)buffer,size,sec,usec);
}

RUDIMENTS_INLINE ssize_t datatransport::read(unsigned short *buffer) {
	unsigned short	temp;
	int	retval=(int)safeRead((void *)&temp,
					sizeof(unsigned short),-1,-1);
	*buffer=ntohs(temp);
	return retval;
}

RUDIMENTS_INLINE ssize_t datatransport::read(unsigned long *buffer) {
	unsigned long	temp;
	int	retval=(int)safeRead((void *)&temp,
					sizeof(unsigned long),-1,-1);
	*buffer=ntohl(temp);
	return retval;
}

RUDIMENTS_INLINE ssize_t datatransport::read(float *buffer) {
	return filedescriptor::read((void *)buffer,sizeof(float));
}

RUDIMENTS_INLINE ssize_t datatransport::read(double *buffer) {
	return filedescriptor::read((void *)buffer,sizeof(double));
}

RUDIMENTS_INLINE ssize_t datatransport::read(char *buffer) {
	return filedescriptor::read((void *)buffer,sizeof(char));
}

RUDIMENTS_INLINE ssize_t datatransport::read(char *buffer, size_t size) {
	return filedescriptor::read((void *)buffer,size);
}

RUDIMENTS_INLINE ssize_t datatransport::read(void *buffer, size_t size) {
	return filedescriptor::read(buffer,size);
}

RUDIMENTS_INLINE ssize_t datatransport::read(char **buffer, char *terminator) {
	return filedescriptor::read(buffer,terminator);
}

RUDIMENTS_INLINE ssize_t datatransport::read(unsigned short *buffer,
							long sec, long usec) {
	unsigned short	temp;
	int	retval=(int)safeRead((void *)&temp,
					sizeof(unsigned short),sec,usec);
	*buffer=ntohs(temp);
	return retval;
}

RUDIMENTS_INLINE ssize_t datatransport::read(unsigned long *buffer,
							long sec, long usec) {
	unsigned long	temp;
	int	retval=(int)safeRead((void *)&temp,
					sizeof(unsigned long),sec,usec);
	*buffer=ntohl(temp);
	return retval;
}

RUDIMENTS_INLINE ssize_t datatransport::read(float *buffer,
							long sec, long usec) {
	return filedescriptor::read((void *)buffer,sizeof(float),sec,usec);
}

RUDIMENTS_INLINE ssize_t datatransport::read(double *buffer,
							long sec, long usec) {
	return filedescriptor::read((void *)buffer,sizeof(double),sec,usec);
}

RUDIMENTS_INLINE ssize_t datatransport::read(char *buffer,
							long sec, long usec) {
	return filedescriptor::read((void *)buffer,sizeof(char),sec,usec);
}

RUDIMENTS_INLINE ssize_t datatransport::read(char *buffer, size_t size,
							long sec, long usec) {
	return filedescriptor::read((void *)buffer,size,sec,usec);
}

RUDIMENTS_INLINE ssize_t datatransport::read(void *buffer, size_t size,
							long sec, long usec) {
	return filedescriptor::read(buffer,size,sec,usec);
}

RUDIMENTS_INLINE ssize_t datatransport::read(char **buffer, char *terminator,
							long sec, long usec) {
	return filedescriptor::read(buffer,terminator,sec,usec);
}

RUDIMENTS_INLINE int datatransport::passFileDescriptor(int descriptor) {
	// by default do nothing and return an error
	return 0;
}

RUDIMENTS_INLINE int datatransport::receiveFileDescriptor(int *descriptor) {
	// by default do nothing and return an error
	return 0;
}
