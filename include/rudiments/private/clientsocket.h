// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

	protected:
#ifdef RUDIMENTS_HAS_SSL
		BIO	*newSSLBIO() const;
#endif
		int	connect(const struct ::sockaddr *addr,
				socklen_t addrlen, long sec, long usec);

		clientsocketprivate	*pvt;
