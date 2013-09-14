// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LINK_H
#define RUDIMENTS_LINK_H

#include <rudiments/private/link.h>

class linkprivate;

// like file class, but some methods operate on the link instead of the
// file the link points to

// wrap:
//	unistd.h - lchown()
//	sys/stat.h - lstat(),lchmod(),
//	sys/xattr.h - lsetxattr(),lgetxattr(),llistxattr(),lremovexattr()
class RUDIMENTS_DLLSPEC link : public file {
	public:

	#include <rudiments/private/link.h>
};

#endif
