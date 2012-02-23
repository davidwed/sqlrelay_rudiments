// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERIALPORT_H
#define RUDIMENTS_SERIALPORT_H

#include <rudiments/private/serialportincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class serialportprivate;

/** The serialport class is similar to the filedescriptor class, but read()
 *  returns EOF when a timeout has occurred.  This class should be used when
 *  communicating over a serial port.
 *
 *  Also, this class wraps some functions from termios.h */
class RUDIMENTS_DLLSPEC serialport : public filedescriptor {
	public:

		/** Creates an instance of the serialport class. */
		serialport();

		/** Creates an instance of the serialport class
		 *  that is identical to "s". */
		serialport(const serialport &s);

		/** Makes this instance of the serialport class
		 *  identical to "s". */
		serialport	&operator=(const serialport &s);

		/** Deletes this instance of the serialport class. */
		virtual		~serialport();

		/** Sets the profile of the serial port to "profile"
		 *  immediately. */
		bool	setProfileNow(serialportprofile *profile);

		/** Sets the profile of the serial port to "profile" after all
		 *  output written to the serial port has been transmitted. */
		bool	setProfileOnDrain(serialportprofile *profile);

		/** Sets the profile of the serial port to "profile" after all
		 *  output written to the serial port has been transmitted.
		 *  Also discards all buffered input. */
		bool	setProfileOnFlush(serialportprofile *profile);

		/** Sets "profile" to the current profile of the serial port. */
		bool	getProfile(serialportprofile *profile);

		/** Waits until all output written to the serial port has been
		 *  transmitted. */
		bool	drain();

		/** Discards all output written to the serial port but not yet
		 *  transmitted and all buffered input. */
		bool	flush();

		/** Discards all buffered input. */
		bool	flushInput();

		/** Discards all output written to the serial port but not yet
 		 *  transmitted. */
		bool	flushOutput();

		/** Causes data not yet written not to be written until
		 *  restartOutput() is called.  New writes will block when the
		 *  output buffer is full. */
		bool	suspendOutput();

		/** Restores output operations suspended
		 *  by a call to suspendOutput(). */
		bool	restartOutput();

		/** Sends a STOP character out the serial port, hopefully
		 *  causing the DTE cease transmitting data until a START
		 *  character is sent. */
		bool	transmitStop();

		/** Sends a START character out the serial port, hopefully
 		 *  causing the DTE to resume transmission. */
		bool	transmitStart();

		/** If the DTE is communicating asynchronously, then a 
		 *  continuous stream of 0-valued bytes are sent to the DTE.
		 *  If "duration" is 0 then the stream will be sent for between
		 *  0.25 and 0.5 seconds.  If "duration" is non-zero then the
		 *  stream will be sent for an implementation-specific period
		 *  of time.
		 *
		 *  If the DTE is communicating synchronously, then this method
		 *  does nothing and returns true immediately. */
		bool	sendBreak(int32_t duration);

	#include <rudiments/private/serialport.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
