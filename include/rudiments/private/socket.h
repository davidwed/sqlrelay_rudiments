// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SOCKET_H
#define RUDIMENTS_SOCKET_H

#include <sys/types.h>
//#define _XPG4_2
#include <sys/socket.h>
//#undef _XPG4_2
#include <sys/ioctl.h>

#include <rudiments/datatransport.h>

class socket : virtual public datatransport {
	public:
			socket();
			socket(int filedesc);
		virtual	~socket();

#ifdef FIONBIO
		virtual bool	useNonBlockingMode();
		virtual bool	useBlockingMode();
#endif
#ifdef RUDIMENTS_HAS_SSL
	protected:
		BIO	*newSSLBIO() const;
		bool	sslAccept(socket *sock);
#endif
		int	connect(struct sockaddr *addr, socklen_t addrlen,
							long sec, long usec);
};

#endif
