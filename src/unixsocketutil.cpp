// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/unixsocketutil.h>
#include <rudiments/rawbuffer.h>

// for NULL
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class unixsocketutilprivate {
	friend class unixsocketutil;
	private:
		const char	*_filename;
		sockaddr_un	_sun;
};

unixsocketutil::unixsocketutil() {
	pvt=new unixsocketutilprivate;
	initialize(NULL);
}

unixsocketutil::unixsocketutil(const unixsocketutil &u) {
	pvt=new unixsocketutilprivate;
	pvt->_filename=u.pvt->_filename;
	pvt->_sun=u.pvt->_sun;
}

unixsocketutil &unixsocketutil::operator=(const unixsocketutil &u) {
	if (this!=&u) {
		pvt->_filename=u.pvt->_filename;
		pvt->_sun=u.pvt->_sun;
	}
	return *this;
}

unixsocketutil::~unixsocketutil() {
	delete pvt;
}

void unixsocketutil::initialize(const char *filename) {
	pvt->_filename=filename;
	rawbuffer::zero(&pvt->_sun,sizeof(pvt->_sun));
}

const char *unixsocketutil::_filename() {
	return pvt->_filename;
}

sockaddr_un *unixsocketutil::_sun() {
	return &pvt->_sun;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
