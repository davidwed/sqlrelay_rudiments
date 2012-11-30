// Copyright (c) 2012 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CODETREE_H
#define RUDIMENTS_CODETREE_H

#include <rudiments/private/codetreeincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

// @cond

class codetree {
	public:
			codetree();

		rudiments::xmldom	*parse(const char *code,
						const char *grammar,
						const char *starttoken);
		bool	write(rudiments::xmldom *tree,
					rudiments::stringbuffer *output);

		void	print(rudiments::xmldom *tree);

	#include <rudiments/private/codetree.h>
};

// @endcond

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
