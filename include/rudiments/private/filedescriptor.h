// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	protected:

		void	filedescriptorInit();
		void	filedescriptorClone(const filedescriptor &f);

		virtual	unsigned short	hostToNet(unsigned short value) const;
		virtual	unsigned long	hostToNet(unsigned long value) const;
		virtual	unsigned short	netToHost(unsigned short value) const;
		virtual	unsigned long	netToHost(unsigned long value) const;

		virtual	ssize_t	safeRead(void *buf, ssize_t count,
						long sec, long usec) const;
		virtual	ssize_t	safeWrite(const void *buf, ssize_t count,
						long sec, long usec) const;
		virtual	int	safeSelect(long sec, long usec,
						bool read, bool write) const;
		bool	setNoDelay(int onoff);

		int	fd;
		bool	retryinterruptedreads;
		bool	retryinterruptedwrites;
		bool	retryinterruptedwaits;
		bool	retryinterruptedfcntl;
		bool	retryinterruptedioctl;
		bool	allowshortreads;
		bool	translatebyteorder;

		listener	*lstnr;
		bool		uselistenerinsidereads;
		bool		uselistenerinsidewrites;

		#ifdef RUDIMENTS_HAS_SSL
		virtual	BIO	*newSSLBIO() const;
			SSL_CTX	*ctx;
			SSL	*ssl;
			BIO	*bio;
		mutable	int	sslresult;
		#endif
