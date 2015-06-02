// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#include <rudiments/serialport.h>
#include <rudiments/bytestring.h>
#include <rudiments/error.h>

// on SCO OSR5, tcdrain and friends are macros that
// wrap ioctl() which is defined in unistd.h
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif

class serialportprivate {
	friend class serialport;
	private:
};

serialport::serialport() : filedescriptor() {
	pvt=new serialportprivate;
	type("serialport");
}

serialport::serialport(const serialport &s) : filedescriptor(s) {
	pvt=new serialportprivate;
	type("serialport");
}

serialport &serialport::operator=(const serialport &s) {
	if (this!=&s) {
		filedescriptor::operator=(s);
	}
	return *this;
}

serialport::~serialport() {
	delete pvt;
}

bool serialport::setProfileNow(serialportprofile *profile) {
	return tcSetAttr(TCSANOW,profile->getTermios());
}

bool serialport::setProfileOnDrain(serialportprofile *profile) {
	return tcSetAttr(TCSADRAIN,profile->getTermios());
}

bool serialport::setProfileOnFlush(serialportprofile *profile) {
	return tcSetAttr(TCSAFLUSH,profile->getTermios());
}

bool serialport::drain() {
	int32_t	result;
	error::clearError();
	do {
		result=tcdrain(fd());
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool serialport::flush() {
	return tcFlush(TCIOFLUSH);
}

bool serialport::flushInput() {
	return tcFlush(TCIFLUSH);
}

bool serialport::flushOutput() {
	return tcFlush(TCOFLUSH);
}

bool serialport::suspendOutput() {
	return tcFlow(TCOOFF);
}

bool serialport::restartOutput() {
	return tcFlow(TCOON);
}

bool serialport::transmitStop() {
	return tcFlow(TCIOFF);
}

bool serialport::transmitStart() {
	return tcFlow(TCION);
}

bool serialport::sendBreak(int32_t duration) {
	int32_t	result;
	error::clearError();
	do {
		result=tcsendbreak(fd(),duration);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool serialport::getProfile(serialportprofile *profile) {
	termios	tio;
	int32_t	result;
	error::clearError();
	do {
		result=tcgetattr(fd(),&tio);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	if (result) {
		return false;
	}
	profile->setOptions(&tio);
	return true;
}

bool serialport::tcSetAttr(int32_t optional_actions, termios *termios_p) {
	int32_t	result;
	error::clearError();
	do {
		result=tcsetattr(fd(),optional_actions,termios_p);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

bool serialport::tcFlush(int32_t queue_selector) {
	int32_t	result;
	error::clearError();
	do {
		result=tcflush(fd(),queue_selector);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool serialport::tcFlow(int32_t action) {
	int32_t	result;
	error::clearError();
	do {
		result=tcflow(fd(),action);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}
