// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DTD_H
#define RUDIMENTS_DTD_H

#include <rudiments/private/config.h>

#include <rudiments/xmldom.h>

// The dtd class implements a minimal XML DTD parser.

class dtd {
	public:

			dtd();
			~dtd();

		int	parseFile(const char *filename);
			// Parse file "filename" and generate a DTD tree.
		int	parseString(const char *string);
			// Parse string "string" and generate a DTD tree.

		stringbuffer	*xml() const;
				// Allocates a stringbuffer, writes a textual
				// representation of the tree and returns the
				// stringbuffer; The calling program must
				// deallocate the stringbuffer.

	#include <rudiments/private/dtd.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/dtdinlines.h>
#endif

#endif
