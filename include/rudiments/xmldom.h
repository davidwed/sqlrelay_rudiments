// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_XMLDOM_H
#define RUDIMENTS_XMLDOM_H

#include <rudiments/private/xmldomincludes.h>

/** The xmldom class implements a minimal XML DOM parser. */

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class xmldomprivate;

class RUDIMENTS_DLLSPEC xmldom : public xmlsax {
	public:

		/** Creates a new instance of the xmldom class. */
		xmldom();

		/** Deletes this instance of the xmldom class. */
		virtual	~xmldom();

		/** Parses file "filename" and generates a DOM tree. */
		bool	parseFile(const char *filename);

		/** Parses string "string" and generates a DOM tree. */
		bool	parseString(const char *string);

		/** Returns the root node of the DOM tree. */
		xmldomnode	*getRootNode() const;

		/** Creates a new root node.  This is useful for building a
		 *  tree from scratch. */
		void	createRootNode();

		/** Writes the current DOM tree to "filename" and sets
		 *  permissions of the file to "perms". */
		bool	writeFile(const char *filename, mode_t perms) const;

	#include <rudiments/private/xmldom.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
