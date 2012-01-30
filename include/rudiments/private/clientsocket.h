// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

	protected:
#ifdef RUDIMENTS_HAS_SSL
		BIO	*newSSLBIO() const;
#endif
		int32_t	connect(const struct ::sockaddr *addr,
				socklen_t addrlen, long sec, long usec);
		ssize_t	lowLevelRead(void *buf, ssize_t count) const;
		ssize_t	lowLevelWrite(const void *buf, ssize_t count) const;
		int32_t	lowLevelClose();

		clientsocketprivate	*pvt;
