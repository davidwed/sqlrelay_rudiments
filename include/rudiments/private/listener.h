// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	protected:
		listenerlist	filedescriptorlist;
		listenerlist	readylist;

		virtual	int	safeSelect(long sec, long usec,
						bool read, bool write);
		bool		retryinterruptedwaits;

		// FIXME: it should be possible to copy a listener
				listener(const listener &l);
		listener	&operator=(const listener &l);
