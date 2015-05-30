// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		void	addFileDescriptor(filedescriptor *fd,
						bool read, bool write);
		bool	rebuildMonitorList();
		void	cleanUp();

		listenerprivate	*pvt;
