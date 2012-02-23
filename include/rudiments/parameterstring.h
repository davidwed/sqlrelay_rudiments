// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PARAMETERSTRING_H
#define RUDIMENTS_PARAMETERSTRING_H

#include <rudiments/private/parameterstringincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class parameterstringprivate;

/** The parameterstring class provides methods for parsing and accessing
 *  name/value pairs in so-called "parameter strings" formatted like this:
 * 
 *  char	*string="name1=value1;name2='value2';name3=value3";
 * 
 *  Parameter strings are often used to define connections to servers, databases
 *  or for other similar things. */
class RUDIMENTS_DLLSPEC parameterstring {
	public:

		/** Creates an instance of the parameterstring class. */
		parameterstring();

		/** Deletes this instance of the parameterstring class. */
		~parameterstring();

		/** Defines the delimiter separating the parameters.  The
		 *  default is a semicolon. */
		void		setDelimiter(char delim);

		/** Parses "string" and extracts the names and values. */
		bool		parse(const char *string);

		/** Returns the corresponding value for "name".  parse() must
		 *  have been called already or this method will return NULL. */
		const char	*getValue(const char *name);

		/** Clears the stored set of names and values.  After a call
		 *  to clear(), getValue() will return NULL until parse() is
		 *  called again. */
		void		clear();

	#include <rudiments/private/parameterstring.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
