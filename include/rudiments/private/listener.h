// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		listenerlist	filedescriptorlist;

	protected:

		virtual	int	safeSelect(long sec, long usec,
						int read, int write);
		int	retryinterruptedwaits;
