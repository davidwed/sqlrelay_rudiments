// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifdef RUDIMENTS_HAS_SSL
	protected:
		BIO	*newSSLBIO() const;
		bool	sslAccept(filedescriptor *sock);
#endif

	private:
		bool	setLingerOnClose(int timeout, int onoff);
		bool	setReuseAddresses(int onoff);
