// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/datatransport.h>

#include <netinet/in.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif


datatransport::datatransport() : filedescriptor() {}

datatransport::datatransport(int filedesc) : filedescriptor(filedesc) {}

unsigned short datatransport::hostToNet(unsigned short value) const {
	return htons(value);
}

unsigned long datatransport::hostToNet(unsigned long value) const {
	return htonl(value);
}

unsigned short datatransport::netToHost(unsigned short value) const {
	return ntohs(value);
}

unsigned long datatransport::netToHost(unsigned long value) const {
	return ntohl(value);
}

bool datatransport::passFileDescriptor(int filedesc) {
	// by default do nothing and return an error
	return false;
}

bool datatransport::receiveFileDescriptor(int *filedesc) {
	// by default do nothing and return an error
	return false;
}
