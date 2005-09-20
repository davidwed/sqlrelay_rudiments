// Copyright (c) 2005 David Muse
// See the COPYING file for more information.

#include <rudiments/networkinterfaces.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class networkinterfaceprivate {
	friend class networkinterface;
	private:
};

networkinterface::networkinterface() {
	pvt=new networkinterfaceprivate;
}

networkinterface::~networkinterface() {
	delete pvt;
}

sockaddr networkinterface::getAddress() {
}

sockaddr networkinterface::getDestinationAddress() {
}

sockaddr networkinterface::getBroadcastAddress() {
}

sockaddr networkinterface::getNetmask() {
}

sockaddr networkinterface::getHardwareAddress() {
}


short networkinterface::getFlags() {
}

int networkinterface::getMetric() {
}

int networkinterface::getMaximumTransmitUnit() {
}

ifmap networkinterface::getDeviceMap() {
}

int networkinterface::getSlaveDevice() {
}

int networkinterface::getForUseByInterfaceData() {
}

int networkinterface::getIndex() {
}

int networkinterface::getLinkBandwidth() {
}

int networkinterface::getQueueLength() {
}

const char *networkinterface::getNewName() {
}


networkinterfacelist *networkinterfaces::getList() {
}

networkinterface *networkinterfaces::getInterface(const char *name) {
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
