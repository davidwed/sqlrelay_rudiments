// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INETSOCKETUTIL_H
#define RUDIMENTS_INETSOCKETUTIL_H

#include <rudiments/private/inetsocketutilincludes.h>

class inetsocketutil {
	public:
			inetsocketutil();
		virtual	~inetsocketutil();
		void	initialize(const char *address, unsigned short port);

	#include <rudiments/private/inetsocketutil.h>
};

#endif
