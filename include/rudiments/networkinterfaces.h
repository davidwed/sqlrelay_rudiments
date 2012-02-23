// Copyright (c) 2005 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_NETWORKINTERFACES_H
#define RUDIMENTS_NETWORKINTERFACES_H

#include <rudiments/private/networkinterfacesincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

// @cond

typedef listnode< networkinterface * >	networkinterfacelistnode;
typedef list< networkinterface *, networkinterfacelistnode >
						networkinterfacelist;

class networkinterfaceprivate;

// FIXME: implement this class
class RUDIMENTS_DLLSPEC networkinterface {
	public:
			networkinterface();
			~networkinterface();

		sockaddr	getAddress();
		sockaddr	getDestinationAddress();
		sockaddr	getBroadcastAddress();
		sockaddr	getNetmask();
		sockaddr	getHardwareAddress();

		int16_t		getFlags();
		int32_t		getMetric();
		int32_t		getMaximumTransmitUnit();
		ifmap		getDeviceMap();
		int32_t		getSlaveDevice();
		int32_t		getForUseByInterfaceData();
		int32_t		getIndex();
		int32_t		getLinkBandwidth();
		int32_t		getQueueLength();
		const char	*getNewName();

	#include <rudiments/private/networkinterface.h>
};

class RUDIMENTS_DLLSPEC networkinterfaces {
	public:
		static	networkinterfacelist	*getList();
		static	networkinterface	*getInterface(const char *name);
	#include <rudiments/private/networkinterfaces.h>
}

// @endcond

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
