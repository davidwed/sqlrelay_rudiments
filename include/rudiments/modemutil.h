// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MODEMUTIL_H
#define RUDIMENTS_MODEMUTIL_H

#include <rudiments/private/modemutilincludes.h>

class modemutil {
	public:
			modemutil();
		virtual	~modemutil();

		void	initialize(const char *device, const char *baud);

	#include <rudiments/private/modemutil.h>
};

#endif
