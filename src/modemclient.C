// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#define EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
#include <rudiments/modemclient.h>
#include <rudiments/chat.h>

#include <rudiments/device.h>
#include <rudiments/charstring.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

modemclient::modemclient() : client(), modemutil() {
	connectscript="";
	disconnectscript="";
	phonenumber="";
	retrywait=0;
	retrycount=0;
}

modemclient::modemclient(const modemclient &m) : client(m), modemutil(m) {
	modemclientClone(m);
}

modemclient &modemclient::operator=(const modemclient &m) {
	if (this!=&m) {
		client::operator=(m);
		modemutil::operator=(m);
		modemclientClone(m);
	}
	return *this;
}

void modemclient::modemclientClone(const modemclient &m) {
	connectscript=m.connectscript;
	phonenumber=m.phonenumber;
	disconnectscript=m.disconnectscript;
	retrywait=m.retrywait;
	retrycount=m.retrycount;
}

modemclient::~modemclient() {
	close();
}

void modemclient::initialize(namevaluepairs *cd) {
	if (cd) {
		cd->getData("device",
			const_cast<char **>(&devicename));
		cd->getData("baud",
			const_cast<char **>(&baud));
		cd->getData("connectscript",
			const_cast<char **>(&connectscript));
		cd->getData("phonenumber",
			const_cast<char **>(&phonenumber));
		cd->getData("disconnectscript",
			const_cast<char **>(&disconnectscript));
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
				unsigned long retrywait,
				unsigned long retrycount) {
	modemutil::initialize(devicename,baud);
	this->connectscript=connectscript;
	this->phonenumber=phonenumber;
	this->disconnectscript=disconnectscript;
	this->retrywait=retrywait;
	this->retrycount=retrycount;
}

int modemclient::connect() {

	namevaluepairs	phnvp;
	phnvp.setData("phonenumber",const_cast<char *>(phonenumber));

	unsigned long	whichtry=0;
	for (;;) {

		delete[] connecterror;
		connecterror=NULL;

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
		chat	ch(this);
		int	result=ch.runScript(connectscript,
						&connecterror,&phnvp);

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
			// return the last thing that caused us to retry
			return result;
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

	if (fd!=-1) {
		chat	ch(this);
		ch.runScript(disconnectscript,NULL);
		return filedescriptor::close();
	}
	return true;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
