// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DTD_H
#define RUDIMENTS_DTD_H

#include <rudiments/private/dtdincludes.h>

// The dtd class implements a minimal XML DTD parser.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class dtdprivate;

class RUDIMENTS_DLLSPEC dtd {
	public:

		/** Creates an instance of the dtd class. */
		dtd();

		/** Deletes this instance of the dtd class. */
		~dtd();

		/** Parse file "filename" and generate DTD tree.
	 	 *  Returns true on success and false on failure. */
		bool		parseFile(const char *filename);

		/** Parse string "string" and generate a DTD tree.
		 *  Returns true on success and false on failure. */
		bool		parseString(const char *string);

		/** If parseFile() or parseString() fails,
		 *  returns the error that caused the failure. */
		const char	*getError();

		/** Returns an XML representation of the DTD. */
		xmldomnode	*xml();

	#include <rudiments/private/dtd.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
