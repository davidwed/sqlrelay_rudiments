// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/modemclient.h>
#include <rudiments/chat.h>

#include <rudiments/device.h>
#include <rudiments/charstring.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>

modemclient::modemclient() : client(), modemutil() {
	connectscript="";
	disconnectscript="";
	retrywait=0;
	retrycount=0;
}

modemclient::~modemclient() {
	close();
}

void modemclient::initialize(namevaluepairs *cd) {
	if (cd) {
		cd->getData("device",&devicename);
		cd->getData("baud",&baud);
		cd->getData("connectscript",&connectscript);
		cd->getData("phonenumber",&phonenumber);
		cd->getData("disconnectscript",&disconnectscript);
		char	*rwstr;
		cd->getData("retrywait",&rwstr);
		retrywait=charstring::toLong(rwstr);
		char	*rcstr;
		cd->getData("retrycount",&rcstr);
		retrycount=charstring::toLong(rcstr);
	}
}

void modemclient::initialize(const char *devicename, const char *baud,
			const char *connectscript,
			const char *phonenumber,
			const char *disconnectscript,
			unsigned int retrywait,
			unsigned int retrycount) {
	modemutil::initialize(devicename,baud);
	this->connectscript=(char *)connectscript;
	this->phonenumber=(char *)phonenumber;
	this->disconnectscript=(char *)disconnectscript;
	this->retrywait=retrywait;
	this->retrycount=retrycount;
}

int modemclient::connect() {

	namevaluepairs	phnvp;
	phnvp.setData("phonenumber",phonenumber);

	unsigned int	whichtry=0;
	for (;;) {

		// open the serial port
		// this is kind of lame, this class should somehow
		// inherit from device
		device	modem;
		if (!modem.open(devicename,O_RDWR|O_NOCTTY)) {
			return RESULT_ERROR;
		}
		setFileDescriptor(modem.getFileDescriptor());
		// set the file descriptor to -1 so it won't get
		// closed when the instance of modem goes away
		modem.setFileDescriptor(-1);

		// configure the serial port
		if (!configureSerialPort(fd,baud)) {
			filedescriptor::close();
			return RESULT_ERROR;
		}

		// run connectscript here...
		chat	ch(this,this);
		int	result=ch.runScript(connectscript,&phnvp);

		// runScript() will return RESULT_(SUCCESS|ABORT|TIMEOUT|ERROR)
		// or a number >= 2 indicating that one of the abort conditions
		// occurred. If we got an abort condition or timeout, retry.
		if (result<=RESULT_SUCCESS && result!=RESULT_TIMEOUT) {
			return result;
		}

		// handle retry
		close();
		whichtry++;
		if (whichtry==retrycount) {
			return RESULT_TIMEOUT;
		}

		// even though the modem's file descriptor is closed here,
		// we use waitForNonBlockingRead to pause between retrys,
		// in case we're using a listener which could return an abort
		int	waitresult=waitForNonBlockingRead(retrywait,0);
		if (waitresult==RESULT_ABORT || waitresult==RESULT_ERROR) {
			return waitresult;
		}
	}
}

bool modemclient::close() {

	chat	ch(this,this);
	ch.runScript(disconnectscript);
	return filedescriptor::close();
}
