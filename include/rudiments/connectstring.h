// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CONNECTSTRING_H
#define RUDIMENTS_CONNECTSTRING_H

#include <rudiments/private/connectstringincludes.h>

// The string class provides methods for parsing and accessing strings
// formatted like this:
//
// char	*string="name1=value1;name2='value2';name3=value3";
//
// These are often used as connectstrings for databases or other similar
// uses.

class connectstring {
	public:

			connectstring();
			~connectstring();

		int	parse(const char *string);
		char	*getValue(const char *name);

	#include <rudiments/private/connectstring.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/connectstringinlines.h>
#endif

#endif
