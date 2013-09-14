// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LINKEDLIST_UTIL_H
#define RUDIMENTS_LINKEDLIST_UTIL_H

#include <rudiments/private/inttypes.h>

template <class datatype>
class linkedlistutil {
	public:
		void	print(datatype data) const;
		int32_t	compare(datatype data1, datatype data2) const;
};

#include <rudiments/private/linkedlistutilinlines.h>

#endif
