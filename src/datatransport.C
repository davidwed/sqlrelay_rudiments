// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/datatransport.h>
#ifndef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/datatransportinlines.h>
#endif

#include <netinet/in.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

ssize_t datatransport::write(unsigned short number) {
	unsigned short	temp=htons(number);
	return safeWrite((void *)&temp,sizeof(unsigned short),-1,-1);
}

ssize_t datatransport::write(unsigned long number) {
	unsigned long	temp=htonl(number);
	return safeWrite((void *)&temp,sizeof(unsigned long),-1,-1);
}

ssize_t datatransport::write(unsigned short number, long sec, long usec) {
	unsigned short	temp=htons(number);
	return safeWrite((void *)&temp,sizeof(unsigned short),sec,usec);
}

ssize_t datatransport::write(unsigned long number, long sec, long usec) {
	unsigned long	temp=htonl(number);
	return safeWrite((void *)&temp,sizeof(unsigned long),sec,usec);
}

ssize_t datatransport::write(const char *string, long sec, long usec) {
	return filedescriptor::write(string,strlen(string),sec,usec);
}

ssize_t datatransport::write(const char *string) {
	return filedescriptor::write(string,strlen(string));
}

ssize_t datatransport::read(unsigned short *buffer) {
	unsigned short	temp;
	int	retval=(int)safeRead((void *)&temp,
					sizeof(unsigned short),-1,-1);
	*buffer=ntohs(temp);
	return retval;
}

ssize_t datatransport::read(unsigned long *buffer) {
	unsigned long	temp;
	int	retval=(int)safeRead((void *)&temp,
					sizeof(unsigned long),-1,-1);
	*buffer=ntohl(temp);
	return retval;
}

ssize_t datatransport::read(unsigned short *buffer, long sec, long usec) {
	unsigned short	temp;
	int	retval=(int)safeRead((void *)&temp,
					sizeof(unsigned short),sec,usec);
	*buffer=ntohs(temp);
	return retval;
}

ssize_t datatransport::read(unsigned long *buffer, long sec, long usec) {
	unsigned long	temp;
	int	retval=(int)safeRead((void *)&temp,
					sizeof(unsigned long),sec,usec);
	*buffer=ntohl(temp);
	return retval;
}
