// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

				transport();
		virtual int	passFileDescriptor(int descriptor);
		virtual int	receiveFileDescriptor(int *descriptor);

	protected:

		int	filedescriptor;

		virtual	ssize_t	safeRead(int fd, void *buf,
							ssize_t count);
		virtual	ssize_t	safeWrite(int fd, const void *buf,
							ssize_t count);

		int	retryinterruptedreads;
		int	retryinterruptedwrites;
