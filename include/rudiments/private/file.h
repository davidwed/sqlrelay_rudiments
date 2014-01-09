// Copyright (c) 2002 David Muse
// See the COPYING file for more information

	protected:
		virtual	void	openInternal(const char *name, int32_t flags,
						mode_t perms, bool useperms);

	private:
		static	bool	stat(const char *filename, void *st);

		off64_t	lseek(off64_t offset, int32_t whence) const;
		bool	lock(int32_t method, int16_t type, int16_t whence,
					off64_t start, off64_t len) const;
		bool	checkLock(int16_t type,
					int16_t whence,
					off64_t start,
					off64_t len,
					int16_t *conftype,
					int16_t *confwhence,
					off64_t *confstart,
					off64_t *conflen) const;
		bool	unlock(int16_t whence,
					off64_t start, off64_t len) const;

		bool	setAttribute(const char *name,
						const void *value,
						size_t size,
						int32_t flags) const;
		const char * const	*attributeArray(const char *buffer,
							size_t size) const;

		void	fileClone(const file &f);


			bool	posixFadvise(off64_t offset, off64_t len,
							int32_t advice) const;
		static	int64_t	pathConf(const char *path, int32_t name);
			int64_t	fpathConf(int32_t name) const;

		fileprivate	*pvt;
