// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

	protected:
#ifdef RUDIMENTS_HAS_SSL
		void	*newSSLBIO() const;
#endif
		int32_t	connect(const struct sockaddr *addr,
				socklen_t addrlen, int32_t sec, int32_t usec);
		ssize_t	lowLevelRead(void *buf, ssize_t count) const;
		ssize_t	lowLevelWrite(const void *buf, ssize_t count) const;
		int32_t	lowLevelClose();

		socketclientprivate	*pvt;