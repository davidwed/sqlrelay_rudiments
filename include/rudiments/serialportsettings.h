// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERIALPORTSETTINGS_H
#define RUDIMENTS_SERIALPORTSETTINGS_H

#include <termios.h>
#include <unistd.h>
#include <rudiments/private/config.h>

class serialportcontrol {
	public:
		int	getCurrentSettings(int fd);
			// Reads the current serial port settings from the
			// serial port.

		int	applySettings(int fd);
			// Applies the settings to the port without waiting
			// for the data in the output buffer to finish
			// transmitting and without discarding data in the
			// input buffer.

		int	applySettingsAndDrain(int fd);
			// Waits for the data in the output buffer to finish
			// transmitting, then applies the settings to the port.
			// Does not discard data in the input buffer.

		int	applySettingsAndFlush(int fd);
			// Waits for the data in the output buffer to finish
			// transmitting, discards the data in the input buffer,
			// then applies the settings to the port.
};

class serialportsettings {
	public:
			serialportsettings();
			~serialportsettings();
	private:
		termios	ti;
};

inline serialportsettings::serialportsettings() {
	memcpy(&ti,0,sizeof(ti));
}

inline int	serialportcontrol::getCurrentSettings(int fd) {
	return tcgetattr(fd,&ti);
}

inline int	serialportcontrol::applySettingsImmediately(int fd) {
	return tcsetattr(fd,TCSANOW,&ti);
}

inline int	serialportcontrol::applySettingsAndDrainOutput(int fd) {
	return tcsetattr(fd,TCSADRAIN,&ti);
}

inline int	serialportcontrol::applySettingsAndDiscardInput(int fd) {
	return tcsetattr(fd,TCSAFLUSH,&ti);
}

#endif
