#include <rudiments/modemserver.h>
#include <rudiments/charstring.h>
#include <rudiments/device.h>
#include <rudiments/chat.h>

#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_STAT_H
	#include <sys/stat.h>
#endif
#ifdef RUDIMENTS_HAVE_FCNTL_H
	#include <fcntl.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif

// windows doesn't have O_NOCTTY, set it to a benign value
#ifndef O_NOCTTY
	#define O_NOCTTY	0
#endif

class modemserverprivate {
	friend class modemserver;
	private:
		const char	*_listenscript;
		const char	*_acceptscript;
		const char	*_disconnectscript;
};

modemserver::modemserver() : server(), modemutil() {
	pvt=new modemserverprivate;
	pvt->_listenscript="";
	pvt->_acceptscript="";
	pvt->_disconnectscript="";
	type("modemserver");
}

modemserver::modemserver(const modemserver &m) : server(m), modemutil(m) {
	pvt=new modemserverprivate;
	modemserverClone(m);
	type("modemserver");
}

modemserver &modemserver::operator=(const modemserver &m) {
	if (this!=&m) {
		server::operator=(m);
		modemutil::operator=(m);
		modemserverClone(m);
	}
	return *this;
}

void modemserver::modemserverClone(const modemserver &m) {
	pvt->_listenscript=m.pvt->_listenscript;
	pvt->_acceptscript=m.pvt->_acceptscript;
	pvt->_disconnectscript=m.pvt->_disconnectscript;
}

modemserver::~modemserver() {
	close();
	delete pvt;
}

void modemserver::initialize(const char *device, const char *baud,
					const char *listenscript,
					const char *acceptscript,
					const char *disconnectscript) {
	modemutil::initialize(device,baud);
	pvt->_listenscript=listenscript;
	pvt->_acceptscript=acceptscript;
	pvt->_disconnectscript=disconnectscript;
}

bool modemserver::listen(const char *device, const char *baud,
					const char *listenscript,
					const char *acceptscript,
					const char *disconnectscript) {

	initialize(device,baud,listenscript,acceptscript,disconnectscript);
	return listen(0);
}

bool modemserver::listen(int32_t backlog) {

	// open the serial port
	// this is kind of lame, this class should somehow
	// inherit from device
	device	modem;
	if (!modem.open(_devicename(),O_RDWR|O_NOCTTY)) {
		return false;
	}
	setFileDescriptor(modem.getFileDescriptor());
	// set the file descriptor to -1 so it won't get
	// closed when the instance of modem goes away
	modem.setFileDescriptor(-1);

	// configure the serial port
	if (!configureSerialPort(fd(),_baud())) {
		filedescriptor::close();
		return false;
	}

	chat	ch(this);
	if (ch.runScript(pvt->_listenscript,NULL)!=RESULT_SUCCESS) {
		close();
		return false;
	}
	return true;
}
	
filedescriptor *modemserver::accept() {

	chat	ch(this);
	if (ch.runScript(pvt->_acceptscript,NULL)==RESULT_SUCCESS) {
		return server::accept();
	}
	return NULL;
}

bool modemserver::close() {
	chat	ch(this);
	ch.runScript(pvt->_disconnectscript,NULL);
	return filedescriptor::close();
}
