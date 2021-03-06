// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/modemclient.h>
#include <rudiments/chat.h>

#include <rudiments/device.h>
#include <rudiments/charstring.h>
#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_STAT_H
	#include <sys/stat.h>
#endif
#ifdef RUDIMENTS_HAVE_FCNTL_H
	#include <fcntl.h>
#endif
#ifdef RUDIMENTS_HAVE_STRING_H
	#include <string.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_IOCTL_H
	#include <sys/ioctl.h>
#endif

// windows doesn't have O_NOCTTY, set it to a benign value
#ifndef O_NOCTTY
	#define O_NOCTTY	0
#endif

class modemclientprivate {
	friend class modemclient;
	private:
		const char	*_customatcommands;
		const char	*_connectscript;
		const char	*_disconnectscript;
		const char	*_phonenumber;
};

modemclient::modemclient() : client(), modemutil() {
	pvt=new modemclientprivate;
	pvt->_customatcommands="";
	pvt->_connectscript="";
	pvt->_disconnectscript="";
	pvt->_phonenumber="";
	type("modemclient");
}

modemclient::modemclient(const modemclient &m) : client(m), modemutil(m) {
	pvt=new modemclientprivate;
	modemclientClone(m);
	type("modemclient");
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
	pvt->_customatcommands=m.pvt->_customatcommands;
	pvt->_connectscript=m.pvt->_connectscript;
	pvt->_phonenumber=m.pvt->_phonenumber;
	pvt->_disconnectscript=m.pvt->_disconnectscript;
}

modemclient::~modemclient() {
	close();
	delete pvt;
}

void modemclient::initialize(constnamevaluepairs *cd) {
	if (cd) {
		const char	*devicename=NULL;
		cd->getValue("device",&devicename);
		const char	*baud=NULL;
		cd->getValue("baud",&baud);
		const char	*customatcommands=NULL;
		cd->getValue("customatcommands",&customatcommands);
		const char	*connectscript=NULL;
		cd->getValue("connectscript",&connectscript);
		const char	*phonenumber=NULL;
		cd->getValue("phonenumber",&phonenumber);
		const char	*disconnectscript=NULL;
		cd->getValue("disconnectscript",&disconnectscript);
		const char	*rwstr=NULL;
		cd->getValue("retrywait",&rwstr);
		const char	*rcstr=NULL;
		cd->getValue("tries",&rcstr);
		initialize(devicename,baud,
				customatcommands,
				connectscript,
				phonenumber,
				disconnectscript,
				charstring::toInteger(rwstr),
				charstring::toInteger(rcstr));
	}
}

void modemclient::initialize(const char *devicename,
				const char *baud,
				const char *customatcommands,
				const char *connectscript,
				const char *phonenumber,
				const char *disconnectscript,
				uint32_t retrywait,
				uint32_t tries) {
	modemutil::initialize(devicename,baud);
	pvt->_customatcommands=customatcommands;
	pvt->_connectscript=connectscript;
	pvt->_phonenumber=phonenumber;
	pvt->_disconnectscript=disconnectscript;
	client::initialize(NULL,-1,-1,retrywait,tries);
}

int32_t modemclient::connect() {

	constnamevaluepairs	phnvp;
	phnvp.setValue("customatcommands",pvt->_customatcommands);
	phnvp.setValue("phonenumber",pvt->_phonenumber);

	uint32_t	whichtry=0;
	for (;;) {

		delete[] *(_connecterror());
		*(_connecterror())=NULL;

		// open the serial port
		// this is kind of lame, this class should somehow
		// inherit from device
		device	modem;
		if (!modem.open(_devicename(),O_RDWR|O_NOCTTY)) {
			return RESULT_ERROR;
		}
		setFileDescriptor(modem.getFileDescriptor());
		// set the file descriptor to -1 so it won't get
		// closed when the instance of modem goes away
		modem.setFileDescriptor(-1);

		// configure the serial port
		if (!configureSerialPort(fd(),_baud())) {
			filedescriptor::close();
			return RESULT_ERROR;
		}

		// run connectscript here...
		chat	ch(this);
		int32_t	result=ch.runScript(pvt->_connectscript,
						_connecterror(),&phnvp);

		// runScript() will return RESULT_(SUCCESS|ABORT|TIMEOUT|ERROR)
		// or a number >= 2 indicating that one of the abort conditions
		// occurred. If we got an abort condition or timeout, retry.
		if (result<=RESULT_SUCCESS && result!=RESULT_TIMEOUT) {
			return result;
		}

		// handle retry
		close();
		whichtry++;
		if (whichtry==_tries()) {
			// return the last thing that caused us to retry
			return result;
		}

		// even though the modem's file descriptor is closed here,
		// we use waitForNonBlockingRead to pause between retrys,
		// in case we're using a listener which could return an abort
		int32_t	waitresult=waitForNonBlockingRead(_retrywait(),0);
		if (waitresult==RESULT_ABORT || waitresult==RESULT_ERROR) {
			return waitresult;
		}
	}
}

bool modemclient::close() {

	if (fd()!=-1) {
		chat	ch(this);
		ch.runScript(pvt->_disconnectscript,NULL);
		return filedescriptor::close();
	}
	return true;
}
