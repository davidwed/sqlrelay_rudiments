// Copyright (c) 2002 David Muse
// See the COPYING file for more information

	protected:
		virtual	int	openInternal(const char *name, int flags);
		virtual	int	openInternal(const char *name,
						int flags, mode_t perms);

	private:
		off64_t	lseek(off64_t offset, int whence) const;
		bool	lock(int method, short type, short whence,
					off64_t start, off64_t len) const;
		bool	checkLock(short type, short whence,
						off64_t start, off64_t len,
						struct flock *retlck) const;
		bool	unlock(short whence, off64_t start, off64_t len) const;

		bool	setAttribute(const char *name,
						const void *value,
						size_t size, int flags) const;
		const char * const	*attributeArray(const char *buffer,
							size_t size) const;

		void	fileClone(const file &f);


			bool	posixFadvise(off64_t offset, off64_t len,
							int advice) const;
		static	long	pathConf(const char *path, int name);
			long	fpathConf(int name) const;

		fileprivate	*pvt;
