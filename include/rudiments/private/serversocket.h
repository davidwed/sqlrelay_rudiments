// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifdef RUDIMENTS_HAS_SSL
	protected:
		void	*newSSLBIO() const;
		bool	sslAccept(filedescriptor *sock);
#endif
		ssize_t	lowLevelRead(void *buf, ssize_t count) const;
		ssize_t	lowLevelWrite(const void *buf, ssize_t count) const;
		int32_t	lowLevelClose();

	private:
		bool	setLingerOnClose(int32_t timeout, int32_t onoff);
		bool	setReuseAddresses(int32_t onoff);

		serversocketprivate	*pvt;
