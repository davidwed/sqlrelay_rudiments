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

class modemclientprivate {
	friend class modemclient;
	private:
		const char	*_connectscript;
		const char	*_disconnectscript;
		const char	*_phonenumber;
};

modemclient::modemclient() : client(), modemutil() {
	pvt=new modemclientprivate;
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
		const char	*devicename;
		cd->getData("device",&devicename);
		const char	*baud;
		cd->getData("baud",&baud);
		const char	*connectscript;
		cd->getData("connectscript",&connectscript);
		const char	*phonenumber;
		cd->getData("phonenumber",&phonenumber);
		const char	*disconnectscript;
		cd->getData("disconnectscript",&disconnectscript);
		const char	*rwstr;
		cd->getData("retrywait",&rwstr);
		const char	*rcstr;
		cd->getData("retrycount",&rcstr);
		initialize(devicename,baud,connectscript,phonenumber,
				disconnectscript,
				charstring::toInteger(rwstr),
				charstring::toInteger(rcstr));
	}
}

void modemclient::initialize(const char *devicename, const char *baud,
				const char *connectscript,
				const char *phonenumber,
				const char *disconnectscript,
				unsigned long retrywait,
				unsigned long retrycount) {
	modemutil::initialize(devicename,baud);
	pvt->_connectscript=connectscript;
	pvt->_phonenumber=phonenumber;
	pvt->_disconnectscript=disconnectscript;
	client::initialize("",-1,-1,retrywait,retrycount);
}

int modemclient::connect() {

	constnamevaluepairs	phnvp;
	phnvp.setData("phonenumber",const_cast<char *>(pvt->_phonenumber));

	unsigned long	whichtry=0;
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
		int	result=ch.runScript(pvt->_connectscript,
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
		if (whichtry==_retrycount()) {
			// return the last thing that caused us to retry
			return result;
		}

		// even though the modem's file descriptor is closed here,
		// we use waitForNonBlockingRead to pause between retrys,
		// in case we're using a listener which could return an abort
		int	waitresult=waitForNonBlockingRead(_retrywait(),0);
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

#ifdef RUDIMENTS_NAMESPACE
}
#endif
