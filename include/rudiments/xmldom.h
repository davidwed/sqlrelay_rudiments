// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_XMLDOM_H
#define RUDIMENTS_XMLDOM_H

#include <rudiments/private/xmldomincludes.h>

// The xmldom class implements a minimal XML DOM parser.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class xmldomprivate;

class xmldom : public xmlsax {
	public:

			xmldom();
		virtual	~xmldom();

		bool	parseFile(const char *filename);
			// Parse file "filename" and generate a DOM tree.
		bool	parseString(const char *string);
			// Parse string "string" and generate a DOM tree.

		xmldomnode	*getRootNode() const;
				// Returns the root node of the DOM tree.

		void	createRootNode();
			// Creates a new root node.  Useful for building a
			// tree from scratch.

		bool	writeFile(const char *filename, mode_t perms) const;
			// Writes the current DOM tree to "filename" and sets
			// permissions of the file to "perms".

	#include <rudiments/private/xmldom.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
