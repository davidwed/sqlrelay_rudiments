// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PARAMETERSTRING_H
#define RUDIMENTS_PARAMETERSTRING_H

#include <rudiments/private/parameterstringincludes.h>

// The string class provides methods for parsing and accessing strings
// formatted like this:
//
// char	*string="name1=value1;name2='value2';name3=value3";
//
// These are often used as parameterstrings for databases or other similar
// uses.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class parameterstringprivate;

class RUDIMENTS_DLLSPEC parameterstring {
	public:

			parameterstring();
			~parameterstring();

		void		setDelimiter(char delim);
		bool		parse(const char *string);
		const char	*getValue(const char *name);
		void		clear();

	#include <rudiments/private/parameterstring.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
