// FIXME: implement this class

// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_NETWORKINTERFACES_H
#define RUDIMENTS_NETWORKINTERFACES_H

#include <rudiments/private/networkinterfacesincludes.h>

typedef listnode< networkinterface * >	networkinterfacelistnode;
typedef list< networkinterface *, networkinterfacelistnode >
						networkinterfacelist;

class networkinterface {
	public:
		sockaddr	getAddress();
		sockaddr	getDestinationAddress();
		sockaddr	getBroadcastAddress();
		sockaddr	getNetmask();
		sockaddr	getHardwareAddress();

		short	getFlags();
		int	getMetric();
		int	getMaximumTransmitUnit();
		ifmap	getDeviceMap();
		int	getSlaveDevice();
		int	getForUseByInterfaceData();
		int	getIndex();
		int	getLinkBandwidth();
		int	getQueueLength();
		char	*getNewName();

	#include <rudiments/private/networkinterface.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/networkinterfaceinlines.h>
#endif

class networkinterfaces {
	public:
		static	networkinterfacelist	*getList();
}

#endif
