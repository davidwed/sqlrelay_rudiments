// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		listenerlist	filedescriptorlist;
		listenerlist	readylist;

	protected:

		virtual	int	safeSelect(long sec, long usec,
						bool read, bool write);
		bool	retryinterruptedwaits;
