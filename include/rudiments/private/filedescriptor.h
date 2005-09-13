// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	protected:

		void	filedescriptorInit();
		void	filedescriptorClone(const filedescriptor &f);

		virtual	uint16_t	hostToNet(uint16_t value) const;
		virtual	uint32_t	hostToNet(uint32_t value) const;
		virtual	uint64_t	hostToNet(uint64_t value) const;
		virtual	uint16_t	netToHost(uint16_t value) const;
		virtual	uint32_t	netToHost(uint32_t value) const;
		virtual	uint64_t	netToHost(uint64_t value) const;

		virtual	ssize_t	bufferedRead(void *buf, ssize_t count,
						long sec, long usec) const;
		virtual	ssize_t	bufferedWrite(const void *buf, ssize_t count,
						long sec, long usec) const;
		virtual	ssize_t	safeRead(void *buf, ssize_t count,
						long sec, long usec) const;
		virtual	ssize_t	safeWrite(const void *buf, ssize_t count,
						long sec, long usec) const;
		virtual	int	safeSelect(long sec, long usec,
						bool read, bool write) const;
		bool		setNoDelay(int onoff);

		#ifdef RUDIMENTS_HAS_SSL
		virtual	BIO	*newSSLBIO() const;
		#endif

		int	getSockOpt(int level, int optname,
					void *optval, socklen_t *optlen);
		int	setSockOpt(int level, int optname,
					const void *optval, socklen_t optlen);

		const char	*type() const;
		void		type(const char *tp);

		int	fd() const;
		void	fd(int filedes);

		#ifdef RUDIMENTS_HAS_SSL
		SSL_CTX	*ctx();
		SSL	*ssl();
		int	sslresult();
		void	sslresult(int sslrslt);
		#endif

	private:
		filedescriptorprivate	*pvt;

		/*int	fd;
		bool	retryinterruptedreads;
		bool	retryinterruptedwrites;
		bool	retryinterruptedwaits;
		#ifdef HAVE_FCNTL
		bool	retryinterruptedfcntl;
		#endif
		#ifdef HAVE_IOCTL
		bool	retryinterruptedioctl;
		#endif
		mutable bool	allowshortreads;
		mutable bool	allowshortwrites;
		bool	translatebyteorder;

		listener	*lstnr;
		bool		uselistenerinsidereads;
		bool		uselistenerinsidewrites;

		#ifdef RUDIMENTS_HAS_SSL
			SSL_CTX	*ctx;
			SSL	*ssl;
			BIO	*bio;
		mutable	int	sslresult;
		#endif

		const char	*type;

		mutable unsigned char	*writebuffer;
		mutable unsigned char	*writebufferend;
		mutable unsigned char	*writebufferptr;

		mutable unsigned char	*readbuffer;
		mutable unsigned char	*readbufferend;
		mutable unsigned char	*readbufferhead;
		mutable unsigned char	*readbuffertail;*/
