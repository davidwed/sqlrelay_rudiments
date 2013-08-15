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
		virtual	int32_t	safeSelect(long sec, long usec,
						bool read, bool write) const;
		virtual	int32_t	safePoll(long sec, long usec,
						bool read, bool write) const;
		bool		setNoDelay(int32_t onoff);

		#ifdef RUDIMENTS_HAS_SSL
		virtual	BIO	*newSSLBIO() const;
		#endif

		int32_t	getSockOpt(int32_t level, int32_t optname,
					void *optval, socklen_t *optlen);
		int32_t	setSockOpt(int32_t level, int32_t optname,
					const void *optval, socklen_t optlen);

		virtual	ssize_t	lowLevelRead(void *buf,
						ssize_t count) const;
		virtual	ssize_t	lowLevelWrite(const void *buf,
						ssize_t count) const;
		virtual int32_t	lowLevelClose();

		const char	*type() const;
		void		type(const char *tp);

		int32_t	fd() const;
		void	fd(int32_t filedes);

		#ifdef RUDIMENTS_HAS_SSL
		SSL_CTX	*ctx();
		SSL	*ssl();
		int32_t	sslresult();
		void	sslresult(int32_t sslrslt);
		#endif

	private:
		filedescriptorprivate	*pvt;

	public:
		filedescriptor(int32_t fd);
