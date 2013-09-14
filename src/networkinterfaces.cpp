// Copyright (c) 2005 David Muse
// See the COPYING file for more information.

#include <rudiments/networkinterfaces.h>

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


int16_t networkinterface::getFlags() {
}

int32_t networkinterface::getMetric() {
}

int32_t networkinterface::getMaximumTransmitUnit() {
}

ifmap networkinterface::getDeviceMap() {
}

int32_t networkinterface::getSlaveDevice() {
}

int32_t networkinterface::getForUseByInterfaceData() {
}

int32_t networkinterface::getIndex() {
}

int32_t networkinterface::getLinkBandwidth() {
}

int32_t networkinterface::getQueueLength() {
}

const char *networkinterface::getNewName() {
}


networkinterfacelist *networkinterfaces::getList() {
}

networkinterface *networkinterfaces::getInterface(const char *name) {
}

#endif
