// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef INETSOCKET_H
#define INETSOCKET_H

#include <netdb.h>
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
};

#include <rudiments/private/inetsocketinlines.h>

#endif
