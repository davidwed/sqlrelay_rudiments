// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <sys/types.h>

#include <rudiments/private/config.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class memorypoolnode {
	friend class memorypool;
	private:
			memorypoolnode(size_t size);
			~memorypoolnode();

		unsigned char	*buffer;
		size_t		size;
		size_t		remaining;
		size_t		position;
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif
