// FIXME: implement this class

// Copyright (c) 2005 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_NETWORKINTERFACES_H
#define RUDIMENTS_NETWORKINTERFACES_H

#include <rudiments/private/networkinterfacesincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

typedef listnode< networkinterface * >	networkinterfacelistnode;
typedef list< networkinterface *, networkinterfacelistnode >
						networkinterfacelist;

class networkinterfaceprivate;

class networkinterface {
	public:
			networkinterface();
			~networkinterface();

		sockaddr	getAddress();
		sockaddr	getDestinationAddress();
		sockaddr	getBroadcastAddress();
		sockaddr	getNetmask();
		sockaddr	getHardwareAddress();

		short		getFlags();
		int		getMetric();
		int		getMaximumTransmitUnit();
		ifmap		getDeviceMap();
		int		getSlaveDevice();
		int		getForUseByInterfaceData();
		int		getIndex();
		int		getLinkBandwidth();
		int		getQueueLength();
		const char	*getNewName();

	#include <rudiments/private/networkinterface.h>
};

class networkinterfaces {
	public:
		static	networkinterfacelist	*getList();
		static	networkinterface	*getInterface(const char *name);
	#include <rudiments/private/networkinterfaces.h>
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
