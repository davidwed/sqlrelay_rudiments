// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/private/socket.h>

socket::socket() : datatransport() {}

socket::socket(int filedescriptor) : datatransport(filedescriptor) {}

#ifdef RUDIMENTS_HAS_SSL
BIO *socket::newSSLBIO() {
	return BIO_new_socket(fd,BIO_NOCLOSE);
}
#endif
