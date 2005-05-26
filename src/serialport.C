// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#include <rudiments/serialport.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

serialport::serialport() : filedescriptor() {
	type="serialport";
}

serialport::serialport(const serialport &s) : filedescriptor(s) {
	type="serialport";
}

serialport &serialport::operator=(const serialport &s) {
	if (this!=&s) {
		filedescriptor::operator=(s);
	}
	return *this;
}

serialport::~serialport() {}

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
	int	result;
	do {
		result=tcdrain(fd);
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

bool serialport::sendBreak(int duration) {
	int	result;
	do {
		result=tcsendbreak(fd,duration);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool serialport::getProfile(serialportprofile *profile) {
	termios	tio;
	int	result;
	do {
		result=tcgetattr(fd,&tio);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	if (result) {
		return false;
	}
	profile->setOptions(&tio);
	return true;
}

bool serialport::tcSetAttr(int optional_actions, termios *termios_p) {
	int	result;
	do {
		result=tcsetattr(fd,optional_actions,termios_p);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

bool serialport::tcFlush(int queue_selector) {
	int	result;
	do {
		result=tcflush(fd,queue_selector);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool serialport::tcFlow(int action) {
	int	result;
	do {
		result=tcflow(fd,action);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
