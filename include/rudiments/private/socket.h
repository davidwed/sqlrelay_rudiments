// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SOCKET_H
#define RUDIMENTS_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <rudiments/datatransport.h>

class socket : virtual public datatransport {
	public:
			socket();
			socket(int filedesc);
#ifdef FIONBIO
		virtual bool	useNonBlockingMode();
		virtual bool	useBlockingMode();
#endif
#ifdef RUDIMENTS_HAS_SSL
	protected:
		BIO	*newSSLBIO() const;
		bool	sslConnect(struct sockaddr *sin, socklen_t length);
		bool	sslAccept(socket *sock);

#endif
};

#endif
