// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	protected:

		virtual	ssize_t	safeRead(void *buf, ssize_t count);
		virtual	ssize_t	safeWrite(const void *buf, ssize_t count);
		virtual	ssize_t	safeSelect(long sec, long usec,
						int read, int write);

		int	fd;
		int	retryinterruptedreads;
		int	retryinterruptedwrites;
		int	retryinterruptedwaits;
