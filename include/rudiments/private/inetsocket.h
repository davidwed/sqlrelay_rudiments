// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INETSOCKET_H
#define RUDIMENTS_INETSOCKET_H

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <rudiments/datatransport.h>

class inetsocket : virtual public datatransport {
	public:
			inetsocket();
			inetsocket(int filedescriptor);
		void	initialize(const char *address, unsigned short port);
	protected:
		char		*address;
		unsigned short	port;
		sockaddr_in	sin;
		#ifdef RUDIMENTS_HAS_SSL
		BIO	*newSSLBIO();
		#endif
};

#endif
