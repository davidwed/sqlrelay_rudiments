// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERIALPORT_H
#define RUDIMENTS_SERIALPORT_H

#include <rudiments/private/serialportincludes.h>

// like filedescriptor class, but read() returns EOF when a timeout has occurred
// also, wraps termios.h functions

class serialport : public filedescriptor {
	public:

		bool	setProfileNow(serialportprofile *profile);
			// tcsetattr(TCSANOW)
		bool	setProfileOnDrain(serialportprofile *profile);
			// tcsetattr(TCSADRAIN)
		bool	setProfileOnFlush(serialportprofile *profile);
			// tcsetattr(TCSAFLUSH)

		bool	getProfile(serialportprofile *profile);
			// tcgetattr

		bool	drain();
			// tcdrain
		bool	flush();
			// tcflush(TCIOFLUSH)
		bool	flushInput();
			// tcflush(TCIFLUSH)
		bool	flushOutput();
			// tcflush(TCOFLUSH)

		bool	suspendOutput();
			// tcflow(TCOOFF)
		bool	restartOutput();
			// tcflow(TCOON)
		bool	transmitStop();
			// tcflow(TCIOFF)
		bool	transmitStart();
			// tcflow(TCION)

		bool	sendBreak(int duration);
			// tcsendbreak()
};

#ifdef HAVE_RUDIMENTS_INLINES
	#include <rudiments/private/serialportinlines.h>
#endif

#endif
