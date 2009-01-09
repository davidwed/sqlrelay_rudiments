// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	friend class timezonefile;
	protected:

		void	filedescriptorInit();
		void	filedescriptorClone(const filedescriptor &f);

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
