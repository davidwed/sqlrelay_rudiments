// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

		virtual bool	passFileDescriptor(int descriptor);
		virtual bool	receiveFileDescriptor(int *descriptor);

	protected:
		virtual	unsigned short	hostToNet(unsigned short value) const;
		virtual	unsigned long	hostToNet(unsigned long value) const;
		virtual	unsigned short	netToHost(unsigned short value) const;
		virtual	unsigned long	netToHost(unsigned long value) const;
