// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	protected:

		virtual	ssize_t	safeRead(void *buf, ssize_t count,
							long sec, long usec);
		virtual	ssize_t	safeWrite(const void *buf, ssize_t count,
							long sec, long usec);
		virtual	int	safeSelect(long sec, long usec,
							int read, int write);

		int	fd;
		int	retryinterruptedreads;
		int	retryinterruptedwrites;
		int	retryinterruptedwaits;
		int	allowshortreads;

		listener	*lstnr;
