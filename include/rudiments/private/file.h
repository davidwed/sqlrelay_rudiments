// Copyright (c) 2002 David Muse
// See the COPYING file for more information

	protected:
		virtual	int32_t	openInternal(const char *name, int32_t flags);
		virtual	int32_t	openInternal(const char *name,
						int32_t flags, mode_t perms);

	private:
		off64_t	lseek(off64_t offset, int32_t whence) const;
		bool	lock(int32_t method, short type, short whence,
					off64_t start, off64_t len) const;
		bool	checkLock(short type, short whence,
						off64_t start, off64_t len,
						struct flock *retlck) const;
		bool	unlock(short whence, off64_t start, off64_t len) const;

		bool	setAttribute(const char *name,
						const void *value,
						size_t size,
						int32_t flags) const;
		const char * const	*attributeArray(const char *buffer,
							size_t size) const;

		void	fileClone(const file &f);


			bool	posixFadvise(off64_t offset, off64_t len,
							int32_t advice) const;
		static	long	pathConf(const char *path, int32_t name);
			long	fpathConf(int32_t name) const;

		fileprivate	*pvt;
