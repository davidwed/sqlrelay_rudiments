// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef XMLDOM_H
#define XMLDOM_H

#include <rudiments/private/config.h>

#include <sys/types.h>

#include <rudiments/xmlsax.h>
#include <rudiments/xmldomnode.h>

// The xmldom class implements a minimal XML DOM parser.

class xmldom : public xmlsax {
	public:

			xmldom();
		virtual	~xmldom();

		int	parseFile(const char *filename);
			// Parse file "filename" and generate a DOM tree.
		int	parseString(const char *string);
			// Parse string "string" and generate a DOM tree.

		xmldomnode	*getRootNode() const;
				// Returns the root node of the DOM tree.

		void	createRootNode();
			// Creates a new root node.  Useful for building a
			// tree from scratch.

		int	writeFile(const char *filename, mode_t perms) const;
			// Writes the current DOM tree to "filename" and sets
			// permissions of the file to "perms".

	#include <rudiments/private/xmldom.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/xmldominlines.h>
#endif

#endif
