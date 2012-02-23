// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DIRECTORYLISTING_H
#define RUDIMENTS_DIRECTORYLISTING_H

#include <rudiments/private/directorylisting.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

// @cond

// wrap:
//	glob.h - glob(),globfree(),glob_pattern_p()
//	not in posix - 
//	dirent/direct.h - scandir(),alphasort(),versionsort()
class RUDIMENTS_DLLSPEC directorylisting {
	public:

	#include <rudiments/private/directorylisting.h>
};

// @endcond

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
