// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DATATRANSPORT_H
#define RUDIMENTS_DATATRANSPORT_H

#include <rudiments/private/datatransportincludes.h>

// The datatransport class is a base class for other classes that transport data
// over (or associated with) a file descriptor (such as clients and servers).

class datatransport : virtual public filedescriptor {
	public:

			datatransport();
				// Creates an uninitialized datatransport.
			datatransport(int filedesc);
				// Creates a datatransport and associates the
				// already open file descriptor "filedesc"
				// with it.

		virtual	~datatransport();

	#include <rudiments/private/datatransport.h>
};

#endif
