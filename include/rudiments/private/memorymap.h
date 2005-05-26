// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

	private:
		void	*data;
		#ifdef HAVE_CREATE_FILE_MAPPING
		HANDLE	map;
		#endif
		size_t	length;

		// FIXME: it should be possible to copy a memorymap
				memorymap(const memorymap &m);
		memorymap	&operator=(const memorymap &m);

			bool	mAdvise(unsigned char *start,
					size_t length, int advice);
		static	bool	mLockAll(int flags);
