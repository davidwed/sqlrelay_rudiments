// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <netinet/in.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

inline transport::transport() : filedescriptor() {}

inline transport::transport(int fd) : filedescriptor(fd) {}

inline ssize_t	transport::write(unsigned short number) {
	unsigned short	temp=htons(number);
	return safeWrite((void *)&temp,sizeof(unsigned short));
}

inline ssize_t	transport::write(unsigned long number) {
	unsigned long	temp=htonl(number);
	return safeWrite((void *)&temp,sizeof(unsigned long));
}

inline ssize_t	transport::read(unsigned short *buffer) {
	unsigned short	temp;
	int	retval=(int)safeRead((void *)&temp,sizeof(unsigned short));
	*buffer=ntohs(temp);
	return retval;
}

inline ssize_t	transport::read(unsigned long *buffer) {
	unsigned long	temp;
	int	retval=(int)safeRead((void *)&temp,sizeof(unsigned long));
	*buffer=ntohl(temp);
	return retval;
}

inline int	transport::passFileDescriptor(int descriptor) {
	// by default do nothing and return an error
	return 0;
}

inline int	transport::receiveFileDescriptor(int *descriptor) {
	// by default do nothing and return an error
	return 0;
}
