// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INETSOCKETUTIL_H
#define RUDIMENTS_INETSOCKETUTIL_H

#include <rudiments/private/inetsocketutilincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class inetsocketutil {
	public:
				inetsocketutil();
				inetsocketutil(const inetsocketutil &i);
		inetsocketutil	&operator=(const inetsocketutil &i);
		virtual		~inetsocketutil();

		void		initialize(const char *address,
						unsigned short port);

	#include <rudiments/private/inetsocketutil.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
