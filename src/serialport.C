// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#include <rudiments/serialport.h>

#ifndef HAVE_RUDIMENTS_INLINES
	#include <rudiments/private/serialportinlines.h>
#endif

bool serialport::getProfile(serialportprofile *profile) {
	termios	tio;
	if (tcgetattr(fd,&tio)) {
		return false;
	}
	profile->setOptions(&tio);
	return true;
}
