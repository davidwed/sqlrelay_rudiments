// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UNIXSOCKETUTIL_H
#define RUDIMENTS_UNIXSOCKETUTIL_H

#include <rudiments/private/unixsocketutilincludes.h>

class unixsocketutil {
	public:
			unixsocketutil();
		virtual ~unixsocketutil();
		void	initialize(const char *filename);

	#include <rudiments/private/unixsocketutil.h>
};

#endif
