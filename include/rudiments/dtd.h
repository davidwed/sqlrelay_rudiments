// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DTD_H
#define RUDIMENTS_DTD_H

#include <rudiments/private/dtdincludes.h>

// The dtd class implements a minimal XML DTD parser.

class dtd : public errorhandler {
	public:
		int	parseFile(const char *filename);
			// Parse file "filename" and generate a DTD tree.
		int	parseString(const char *string);
			// Parse string "string" and generate a DTD tree.

		xmldom	*xml();
			// Returns an XML representation of the DTD.

	#include <rudiments/private/dtd.h>
};

#ifdef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/dtdinlines.h>
#endif

#endif
