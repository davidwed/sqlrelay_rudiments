// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		filedescriptornode	*first;
		filedescriptornode	*last;
		filedescriptornode	*current;

	protected:

		virtual	ssize_t	safeSelect(long sec, long usec,
						int read, int write);
		int	retryinterruptedwaits;
