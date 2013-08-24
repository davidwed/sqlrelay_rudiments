// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_XMLDOM_H
#define RUDIMENTS_XMLDOM_H

#include <rudiments/private/xmldomincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class xmldomprivate;

/** The xmldom class implements a minimal XML DOM parser. */
class RUDIMENTS_DLLSPEC xmldom : public xmlsax {
	public:

		/** Creates a new instance of the xmldom class. */
		xmldom();

		/** Creates a new instance of the xmldom class, allowing the
		 *  user to enable or disable the "string cache" for this
		 *  instance.
		 *
		 *  By default, as each xmldomnode is added to the xmldom tree,
		 *  tag and attribute names and values are stored in a string
		 *  cache and pointers into the cache are assigned to each node.
		 *  Reference counters are kept and the string is removed from
		 *  the cache when no more nodes are using it.  As xml tends to
		 *  be highly repetitive this conserves memory at the cost of
		 *  speed.
		 *
		 *  If the string cache is disabled then memory is allocated in
		 *  each xmldomnode for names and values and freed when the
		 *  xmldomnode is freed.  This is faster but uses much more
		 *  memory. */
		xmldom(bool stringcacheenabled);

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

		/** Returns true if the string cache is enabled and false
		 *  if it is disabled. */
		bool	stringCacheEnabled();

	#include <rudiments/private/xmldom.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
