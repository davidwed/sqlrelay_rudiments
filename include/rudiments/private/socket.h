// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifdef RUDIMENTS_HAS_SSL
	protected:
		BIO	*newSSLBIO() const;
		bool	sslAccept(socket *sock);
#endif
		int	connect(struct sockaddr *addr, socklen_t addrlen,
							long sec, long usec);

	private:
		bool	setLingerOnClose(int timeout, int onoff);
		bool	setReuseAddresses(int onoff);
