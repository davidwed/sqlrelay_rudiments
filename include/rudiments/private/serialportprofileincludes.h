// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/private/inttypes.h>

#ifdef RUDIMENTS_HAVE_TERMIOS_H
	#include <termios.h>
#else
	#define NCCS	32
	struct termios {
		uint32_t	c_iflag;
		uint32_t	c_oflag;
		uint32_t	c_cflag;
		uint32_t	c_lflag;
		unsigned char	c_cc[NCCS];
	};
#endif
