// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifdef RUDIMENTS_HAS_SSL
	protected:
		BIO	*newSSLBIO() const;
		bool	sslAccept(filedescriptor *sock);
#endif
		ssize_t	lowLevelRead(void *buf, ssize_t count) const;
		ssize_t	lowLevelWrite(const void *buf, ssize_t count) const;
		int	lowLevelClose();

	private:
		bool	setLingerOnClose(int timeout, int onoff);
		bool	setReuseAddresses(int onoff);

		serversocketprivate	*pvt;
