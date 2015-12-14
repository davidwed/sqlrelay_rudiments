// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

	protected:
		void	*newSSLBIO() const;
		bool	sslAccept(filedescriptor *sock);
		bool	gssAccept(filedescriptor *sock);

		ssize_t	lowLevelRead(void *buf, ssize_t count);
		ssize_t	lowLevelWrite(const void *buf, ssize_t count) const;
		int32_t	lowLevelClose();

	private:
		bool	setLingerOnClose(int32_t timeout, int32_t onoff);
		bool	setReuseAddresses(int32_t onoff);

		socketserverprivate	*pvt;
