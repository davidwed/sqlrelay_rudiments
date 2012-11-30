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

		bool	parse(const char *input,
					const char *grammar,
					const char *starttoken,
					rudiments::xmldomnode *output);
		bool	write(rudiments::xmldomnode *input,
					const char *grammar,
					rudiments::stringbuffer *output);

	#include <rudiments/private/codetree.h>
};

// @endcond

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
