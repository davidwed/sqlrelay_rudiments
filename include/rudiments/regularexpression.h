// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_REGEXP_H
#define RUDIMENTS_REGEXP_H

#include <rudiments/private/regularexpressionincludes.h>

// The regular expression class provides methods for making comparisons
// between text and regular expressions.
//
// Regular expressions are complex, powerful, used in command line
// programs like grep, sed and find, and extensively in Perl.

class regularexpression {
	public:

		// if you need to do a quick comparison, use this method
		static	bool	match(const char *str, const char *pattern);
				// Returns true if "str" matches "pattern"
				// and false if "str" doesn't match "pattern".


		// if you need to do many comparisons against a single
		// expression, use these methods
			regularexpression();
			regularexpression(const char *pattern);
				// this constructor calls the compile method
				// below during initialization
		virtual	~regularexpression();

		bool	compile(const char *pattern);
				// Compiles the regular expression given in
				// "pattern".
				//
				// Returns true if the compilation 
				// succeeded and false if it failed.
		bool	match(const char *str);
				// Matches "str" against the regular expression
				// compiled earlier using the compile method.
				//
				// Returns true if the match was successful and
				// false if it was not.

	#include <rudiments/private/regularexpression.h>
};

#ifdef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/regularexpressioninlines.h>
#endif

#endif
