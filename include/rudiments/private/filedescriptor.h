// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	friend class securitycontext;
	friend class gsscontext;
	friend class tlscontext;
	protected:

		void	filedescriptorInit();
		void	filedescriptorClone(const filedescriptor &f);

		virtual	ssize_t	bufferedRead(void *buf, ssize_t count,
					int32_t sec, int32_t usec);
		virtual	ssize_t	bufferedWrite(const void *buf, ssize_t count,
					int32_t sec, int32_t usec) const;
		virtual	ssize_t	safeRead(void *buf, ssize_t count,
					int32_t sec, int32_t usec);
		virtual	ssize_t	safeWrite(const void *buf, ssize_t count,
					int32_t sec, int32_t usec) const;
		bool		setNoDelay(int32_t onoff);

		int32_t	getSockOpt(int32_t level, int32_t optname,
					void *optval, socklen_t *optlen);
		int32_t	setSockOpt(int32_t level, int32_t optname,
					const void *optval, socklen_t optlen);

		virtual	ssize_t	lowLevelRead(void *buf,
						ssize_t count);
		virtual	ssize_t	midLevelWrite(const void *buf,
						ssize_t count) const;
		virtual	ssize_t	lowLevelWrite(const void *buf,
						ssize_t count) const;
		virtual int32_t	lowLevelClose();

		const char	*type() const;
		void		type(const char *tp);

		int32_t	fd() const;
		void	fd(int32_t filedes);

		securitycontext	*secctx();

	private:
		static void	lowLevelWriteWorker(void *attr);
		filedescriptorprivate	*pvt;

	public:
		filedescriptor(int32_t fd);
		static void	*getHandleFromFileDescriptor(int32_t fd);

		void	useAsyncWrite();
		void	dontUseAsyncWrite();
