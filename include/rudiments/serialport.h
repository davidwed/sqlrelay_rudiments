// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERIALPORT_H
#define RUDIMENTS_SERIALPORT_H

#include <rudiments/private/serialport.h>

// like filedescriptor class, but read() returns EOF when a timeout has occurred
// also, wraps termios.h functions

class serialport : public filedescriptor {
	public:

	#include <rudiments/private/serialport.h>
};

#endif
