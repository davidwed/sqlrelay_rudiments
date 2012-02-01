// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/inetsocketutil.h>
#include <rudiments/rawbuffer.h>

// for NULL
#include <stdlib.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class inetsocketutilprivate {
	friend class inetsocketutil;
	private:
		const char	*_address;
		uint16_t	_port;
		sockaddr_in	_sin;
};

inetsocketutil::inetsocketutil() {
	pvt=new inetsocketutilprivate;
	initialize(NULL,0);
}

inetsocketutil::inetsocketutil(const inetsocketutil &i) {
	pvt=new inetsocketutilprivate;
	initialize(i.pvt->_address,i.pvt->_port);
}

inetsocketutil &inetsocketutil::operator=(const inetsocketutil &i) {
	if (this!=&i) {
		initialize(i.pvt->_address,i.pvt->_port);
	}
	return *this;
}

inetsocketutil::~inetsocketutil() {
	delete pvt;
}

void inetsocketutil::initialize(const char *address, uint16_t port) {
	pvt->_address=address;
	pvt->_port=port;
	rawbuffer::zero(&pvt->_sin,sizeof(pvt->_sin));
}

const char *inetsocketutil::_address() {
	return pvt->_address;
}

uint16_t *inetsocketutil::_port() {
	return &pvt->_port;
}

sockaddr_in *inetsocketutil::_sin() {
	return &pvt->_sin;
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif
