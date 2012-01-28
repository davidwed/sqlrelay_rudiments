// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

	private:
		// FIXME: it should be possible to copy a memorymap
				memorymap(const memorymap &m);
		memorymap	&operator=(const memorymap &m);

			bool	mAdvise(unsigned char *start,
					size_t length, int32_t advice);
		static	bool	mLockAll(int32_t flags);

		memorymapprivate	*pvt;
