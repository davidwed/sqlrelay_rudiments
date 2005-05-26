// Copyright (c) 2005 David Muse
// See the COPYING file for more information

#include <rudiments/private/modemutil.h>
#include <rudiments/serialport.h>
#include <rudiments/serialportprofile.h>
#include <rudiments/signalclasses.h>
#include <rudiments/file.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <string.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

modemutil::modemutil() {
	devicename="";
	baud="";
}

modemutil::~modemutil() {}

void modemutil::initialize(const char *device, const char *baud) {
	this->devicename=device;
	this->baud=baud;
}

bool modemutil::configureSerialPort(int fd, const char *baud) {

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

#ifdef RUDIMENTS_NAMESPACE
}
#endif
