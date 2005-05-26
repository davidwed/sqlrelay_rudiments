// Copyright (c) 2002 David Muse
// See the COPYING file for more information

	protected:
		virtual	int	openInternal(const char *name, int flags);
		virtual	int	openInternal(const char *name,
						int flags, mode_t perms);

	private:
		struct	stat	st;
		bool		getcurrentpropertiesonopen;
		bool		retryinterruptedlockops;

		off_t	lseek(off_t offset, int whence) const;
		bool	lock(int method, short type, short whence,
						off_t start, off_t len) const;
		bool	checkLock(short type, short whence,
						off_t start, off_t len,
						struct flock *retlck) const;
		bool	unlock(short whence, off_t start, off_t len) const;

		#ifdef HAVE_XATTRS
		bool	setAttribute(const char *name,
						const void *value,
						size_t size, int flags) const;
		const char * const	*attributeArray(const char *buffer,
							size_t size) const;
		#endif

		void	fileClone(const file &f);


			bool	posixFadvise(off_t offset, off_t len,
							int advice) const;
		static	long	pathConf(const char *path, int name);
			long	fpathConf(int name) const;
