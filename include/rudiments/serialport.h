// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERIALPORT_H
#define RUDIMENTS_SERIALPORT_H

#include <rudiments/private/serialportincludes.h>

// like filedescriptor class, but read() returns EOF when a timeout has occurred
// also, wraps termios.h functions

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class serialportprivate;

class serialport : public filedescriptor {
	public:

				serialport();
				serialport(const serialport &s);
		serialport	&operator=(const serialport &s);
		virtual		~serialport();

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

	#include <rudiments/private/serialport.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
