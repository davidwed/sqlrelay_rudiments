// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	protected:

		// FIXME: it should be possible to copy a listener
				listener(const listener &l);
		listener	&operator=(const listener &l);

	private:
		void	addFileDescriptor(filedescriptor *fd,
						bool read, bool write);
		bool	rebuildMonitorList();
		void	cleanUp();

		listenerprivate	*pvt;
