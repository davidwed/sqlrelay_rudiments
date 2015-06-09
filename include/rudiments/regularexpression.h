// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_REGEXP_H
#define RUDIMENTS_REGEXP_H

#include <rudiments/private/regularexpressionincludes.h>

class regularexpressionprivate;

/** The regularexpression class provides methods for making comparisons
 *  between text and regular expressions.
 * 
 *  Regular expressions are complex, powerful, used in command line
 *  programs like grep, sed and find, and extensively in Perl. */
class RUDIMENTS_DLLSPEC regularexpression {
	public:

		/** Returns true if "str" matches "pattern"
		 *  and false if "str" doesn't match "pattern". */
		static	bool	match(const char *str, const char *pattern);


		/** Creates an instance of the regularexpression class. */
		regularexpression();

		/** Creates an instance of the regularexpression class and
 		 *  compiles the regular expression given in "pattern". */
		regularexpression(const char *pattern);

		/** Deletes this instance of the regular expression class. */
		~regularexpression();

		/** Compiles the regular expression given in "pattern".
		 *
		 *  Returns true if the compilation succeeded and false
		 *  if it failed. */
		bool	compile(const char *pattern);

		/** Studies the previously compiled pattern so it can be
		 *  executed faster.  If you plan on calling match()
		 *  several times on this pattern, studying it may be
		 *  worthwhile.  If not, the studying the pattern may
		 *  take longer than the time saved by studying it.
		 * 
		 *  Returns true if the study succeeded
		 *  and false if if failed. */
		bool	study();

		/** Matches "str" against the regular expression
		 *  compiled earlier using the compile method.
		 * 
		 *  Returns true if the match was successful and
		 *  false if it was not. */
		bool	match(const char *str);

		/** Returns the number of substrings of "str" passed into
		 *  match() that match "pattern" passed into compile(). */
		int32_t	getSubstringCount();

		/** Returns the "index"'th matching substring or NULL
		 *  if index is invalid. */
		const char	*getSubstringStart(int32_t index);

		/** Returns the data directly after the "index"'th
		 *  matching substring or NULL if index is invalid. */
		const char	*getSubstringEnd(int32_t index);

		/** Returns the offset of the "index"'th matching
		 *  substring or -1 if index is invalid. */
		int32_t	getSubstringStartOffset(int32_t index);

		/** Returns the offset of the data directly after the
		 *  "index"'th matching substring or -1 if index is
		 *  invalid. */
		int32_t	getSubstringEndOffset(int32_t index);

	#include <rudiments/private/regularexpression.h>
};

#endif
