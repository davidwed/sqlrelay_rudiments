// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LINKEDLIST_UTIL_H
#define RUDIMENTS_LINKEDLIST_UTIL_H

template <class datatype>
class linkedlistutil {
	public:
		void	print(datatype data) const;
		int	compare(datatype data1, datatype data2) const;
};

#include <rudiments/private/linkedlistutilinlines.h>

#endif
