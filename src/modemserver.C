#include <rudiments/modemserver.h>
#include <rudiments/charstring.h>
#include <rudiments/device.h>
#include <rudiments/chat.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

modemserver::modemserver() : server() {
	devicename="";
	baud="";
	listenscript="";
	acceptscript="";
	disconnectscript="";
}

modemserver::~modemserver() {
	close();
}

void modemserver::initialize(const char *device, const char *baud,
					const char *listenscript,
					const char *acceptscript,
					const char *disconnectscript) {
	modemutil::initialize(device,baud);
	this->listenscript=(char *)listenscript;
	this->acceptscript=(char *)acceptscript;
	this->disconnectscript=(char *)disconnectscript;
}

bool modemserver::listen(const char *device, const char *baud,
					const char *listenscript,
					const char *acceptscript,
					const char *disconnectscript) {

	initialize(device,baud,listenscript,acceptscript,disconnectscript);
	return listen(0);
}

bool modemserver::bind() {
	return true;
}

bool modemserver::listen(int backlog) {

	// open the serial port
	// this is kind of lame, this class should somehow
	// inherit from device
	device	modem;
	if (!modem.open(devicename,O_RDWR|O_NOCTTY)) {
		return false;
	}
	setFileDescriptor(modem.getFileDescriptor());
	// set the file descriptor to -1 so it won't get
	// closed when the instance of modem goes away
	modem.setFileDescriptor(-1);

	// configure the serial port
	if (!configureSerialPort(fd,baud)) {
		filedescriptor::close();
		return false;
	}

	chat	ch(this,this);
	if (ch.runScript(listenscript)!=RESULT_SUCCESS) {
		close();
		return false;
	}
	return true;
}
	
filedescriptor *modemserver::accept() {

	chat	ch(this,this);
	if (ch.runScript(acceptscript)==RESULT_SUCCESS) {
		filedescriptor	*retval=new filedescriptor;
		retval->setFileDescriptor(fd);
		return retval;
	}
	return NULL;
}

bool modemserver::close() {
	chat	ch(this,this);
	ch.runScript(disconnectscript);
	return filedescriptor::close();
}
