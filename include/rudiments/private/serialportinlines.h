// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#include <rudiments/rawbuffer.h>

RUDIMENTS_INLINE
bool serialport::setProfileNow(serialportprofile *profile) {
	return !tcsetattr(fd,TCSANOW,profile->getTermios());
}

RUDIMENTS_INLINE
bool serialport::setProfileOnDrain(serialportprofile *profile) {
	return !tcsetattr(fd,TCSADRAIN,profile->getTermios());
}

RUDIMENTS_INLINE
bool serialport::setProfileOnFlush(serialportprofile *profile) {
	return !tcsetattr(fd,TCSAFLUSH,profile->getTermios());
}

RUDIMENTS_INLINE
bool serialport::drain() {
	return !tcdrain(fd);
}

RUDIMENTS_INLINE
bool serialport::flush() {
	return !tcflush(fd,TCIOFLUSH);
}

RUDIMENTS_INLINE
bool serialport::flushInput() {
	return !tcflush(fd,TCIFLUSH);
}

RUDIMENTS_INLINE
bool serialport::flushOutput() {
	return !tcflush(fd,TCOFLUSH);
}

RUDIMENTS_INLINE
bool serialport::suspendOutput() {
	return !tcflow(fd,TCOOFF);
}

RUDIMENTS_INLINE
bool serialport::restartOutput() {
	return !tcflow(fd,TCOON);
}

RUDIMENTS_INLINE
bool serialport::transmitStop() {
	return !tcflow(fd,TCIOFF);
}

RUDIMENTS_INLINE
bool serialport::transmitStart() {
	return !tcflow(fd,TCION);
}

RUDIMENTS_INLINE
bool serialport::sendBreak(int duration) {
	return !tcsendbreak(fd,duration);
}
