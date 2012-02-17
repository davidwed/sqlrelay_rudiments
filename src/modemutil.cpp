// Copyright (c) 2005 David Muse
// See the COPYING file for more information

#include <rudiments/private/modemutil.h>
#include <rudiments/serialport.h>
#include <rudiments/serialportprofile.h>
#include <rudiments/signalclasses.h>
#include <rudiments/file.h>

#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_STAT_H
	#include <sys/stat.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_WAIT_H
	#include <sys/wait.h>
#endif
#ifdef RUDIMENTS_HAVE_FCNTL_H
	#include <fcntl.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <string.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class modemutilprivate {
	friend class modemutil;
	private:
		const char	*_devicename;
		const char	*_baud;
};

modemutil::modemutil() {
	pvt=new modemutilprivate;
	pvt->_devicename="";
	pvt->_baud="";
}

modemutil::~modemutil() {
	delete pvt;
}

void modemutil::initialize(const char *device, const char *baud) {
	pvt->_devicename=device;
	pvt->_baud=baud;
}

bool modemutil::configureSerialPort(int32_t fd, const char *baud) {

	serialportprofile	spp;
	spp.baud(baud);
	spp.ignoreParityErrors(true);
	spp.hardwareFlowControl(true);
	spp.characterSize(serialportprofile::cs_8);
	spp.receiverOn(true);
	spp.ignoreModemControlLines(true);
	spp.readThreshold(1);
	spp.readTimeout(0);

	// this is kind of lame, this class should somehow
	// inherit from serialport
	serialport	sp;
	sp.setFileDescriptor(fd);
	bool	retval=(sp.flush() && sp.setProfileNow(&spp));
	// set the file descriptor to -1 so it won't get
	// closed when the instance of serialport goes away
	sp.setFileDescriptor(-1);
	return retval;
}

const char *modemutil::_devicename() {
	return pvt->_devicename;
}

const char *modemutil::_baud() {
	return pvt->_baud;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
