// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#include <rudiments/serialport.h>
#include <rudiments/rawbuffer.h>

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
	return !tcsetattr(fd,TCSANOW,profile->getTermios());
}

bool serialport::setProfileOnDrain(serialportprofile *profile) {
	return !tcsetattr(fd,TCSADRAIN,profile->getTermios());
}

bool serialport::setProfileOnFlush(serialportprofile *profile) {
	return !tcsetattr(fd,TCSAFLUSH,profile->getTermios());
}

bool serialport::drain() {
	return !tcdrain(fd);
}

bool serialport::flush() {
	return !tcflush(fd,TCIOFLUSH);
}

bool serialport::flushInput() {
	return !tcflush(fd,TCIFLUSH);
}

bool serialport::flushOutput() {
	return !tcflush(fd,TCOFLUSH);
}

bool serialport::suspendOutput() {
	return !tcflow(fd,TCOOFF);
}

bool serialport::restartOutput() {
	return !tcflow(fd,TCOON);
}

bool serialport::transmitStop() {
	return !tcflow(fd,TCIOFF);
}

bool serialport::transmitStart() {
	return !tcflow(fd,TCION);
}

bool serialport::sendBreak(int duration) {
	return !tcsendbreak(fd,duration);
}

bool serialport::getProfile(serialportprofile *profile) {
	termios	tio;
	if (tcgetattr(fd,&tio)) {
		return false;
	}
	profile->setOptions(&tio);
	return true;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
