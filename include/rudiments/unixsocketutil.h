// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UNIXSOCKETUTIL_H
#define RUDIMENTS_UNIXSOCKETUTIL_H

#include <rudiments/private/unixsocketutilincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class unixsocketutilprivate;

class unixsocketutil {
	public:
			unixsocketutil();
			unixsocketutil(const unixsocketutil &u);
		unixsocketutil	&operator=(const unixsocketutil &u);
		virtual		~unixsocketutil();

		void	initialize(const char *filename);

	#include <rudiments/private/unixsocketutil.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
