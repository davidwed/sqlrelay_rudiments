// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LINKEDLIST_UTIL_H
#define RUDIMENTS_LINKEDLIST_UTIL_H

#include <rudiments/private/inttypes.h>

template <class valuetype>
class linkedlistutil {
	public:
		void	print(valuetype value) const;
		int32_t	compare(valuetype value1, valuetype value2) const;
};

#include <rudiments/private/linkedlistutilinlines.h>

#endif
