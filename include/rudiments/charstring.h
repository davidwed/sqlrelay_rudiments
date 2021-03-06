// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CHARSTRING_H
#define RUDIMENTS_CHARSTRING_H

#include <rudiments/private/charstringincludes.h>

/** The charstring class provides static methods for manipulating C-style
 *  character strings.
 * 
 *  In addition to some unique methods, analogs for the standard C string
 *  functions are provided.  However, unlike the standard C string functions,
 *  the charstring methods are NULL safe.  Your application will not crash if a
 *  NULL is passed in, and instead, will give intuitive results. */
class RUDIMENTS_DLLSPEC charstring {
	public:

		/** Returns the length of "string". */
		static	size_t	length(const char *string);

		/** Returns the length of "string". */
		static	size_t	length(const unsigned char *string);

		/** Returns true if "string" is NULL or the empty string. */
		static	bool	isNullOrEmpty(const char *string);

		/** Returns true if "string" is NULL or the empty string. */
		static	bool	isNullOrEmpty(const unsigned char *string);

		/** Returns true if "string" starts with the standalone "word":
		 *  "Yes" (case-insensitive), "True" (case-insensitive), 
		 *  "Y" (case-insensitive), "T" (case-insensitive), or
		 *  "1" and false otherwise.
		 *
		 *  (Eg. returns true for "Yes,", "True ", and "1 2 3" but
		 *  returns false for "Yesterday", "Truest", and "123") */
		static	bool	isYes(const char *string);

		/** Returns true if "string" starts with the standalone word:
		 *  "No" (case-insensitive), "False" (case-insensitive), or
		 *  "0" and false otherwise.
		 *
		 *  (Eg. returns true for "No,", "False ", and "0 1 2" but
		 *  returns false for "Nothing", "Falsest", and "012") */
		static	bool	isNo(const char *string);

		/** Sets "size" bytes of "str" to NULL. */
		static	void	zero(char *str, size_t size);

		/** Appends "source" to "dest".  Assumes that there is
		 *  enough room remaining in "dest" to accommodate the new
		 *  string.  Returns a pointer to "dest". */
		static	char	*append(char *dest, const char *source);

		/** Appends "size" bytes of "source" to "dest".  Assumes
		 *  that there is enough room remaining in "dest" to
		 *  accommodate the new string.  Returns a pointer to "dest". */
		static	char	*append(char *dest, const char *source,
								size_t size);

		/** Converts "number" to a string and appends it to "dest".
		 *  Assumes that there is enough room remaining in "dest" to
		 *  accommodate the new string.  Returns a pointer to "dest". */
		static	char	*append(char *dest, int64_t number);

		/** Converts "number" to a string and appends it to "dest".
		 *  Assumes that there is enough room remaining in "dest" to
		 *  accommodate the new string.  Returns a pointer to "dest". */
		static	char	*append(char *dest, uint64_t number);

		/** Converts "number" to a string and appends it to "dest".
		 *  Assumes that there is enough room remaining in "dest" to
		 *  accommodate the new string.  Returns a pointer to "dest". */
		static	char	*append(char *dest, double number);

		/** Converts "number" to a string using "scale" and appends it
		 *  to "dest".  Assumes that there is enough room remaining in
		 *  "dest" to accommodate the new string.  Returns a pointer to
		 *  "dest". */
		static	char	*append(char *dest, double number,
							uint16_t scale);

		/** Converts "number" to a string using "precision" and "scale"
		 *  and appends it to "dest".  Assumes that there is enough
		 *  room remaining in "dest" to accommodate the new string.
		 *  Returns a pointer to "dest". */
		static	char	*append(char *dest, double number,
						uint16_t precision,
						uint16_t scale);

		/** Replaces "dest" with "source".  Assumes that there is
		 *  enough room in "dest" to accommodate "source".  Returns a
		 *  pointer to "dest". */
		static	char	*copy(char *dest, const char *source);

		/** Replaces the first "size" bytes of "dest" with "source".
		 *  Assumes that "dest" is at least "size" bytes long.
		 *  Returns a pointer to "dest". */
		static	char	*copy(char *dest,
					const char *source, size_t size);

		/** Replaces "dest" with "source", starting "location" bytes
		 *  into "dest".  Assumes that there is enough room in "dest"
		 *  (after "location" bytes) to accommodate "source".  Returns
		 *  a pointer to "dest". */
		static	char	*copy(char *dest, size_t location,
						const char *source);

		/** Replaces "size" bytes of "dest" with "source", starting
		 *  "location" bytes into "dest".  Assumes that there are
		 *  "size" bytes in "dest" (after "location" bytes).  Returns a
		 *  pointer to "dest". */
		static	char	*copy(char *dest, size_t location,
					const char *source, size_t size);

		/** Replaces "dest" with "source" unless the length of "source"
		 *  is greater than "destsize", in which case only "destsize"
		 *  bytes of "dest" will be replaced.  Returns a pointer to
		 *  "dest". */
		static char	*safeCopy(char *dest, size_t destsize,
							const char *source);

		/** Replaces "sourcesize" bytes of "dest" with "source" unless
		 *  "sourcesize" is greater than "destsize", in which case only
		 *  "destsize" bytes of "dest" will be replaced.  Returns a
		 *  pointer to "dest". */
		static char	*safeCopy(char *dest, size_t destsize,
					const char *source, size_t sourcesize);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2". */
		static	int32_t	compare(const char *str1, const char *str2);

		/** Returns -1,0 or 1 if "size" bytes of "str1" are greater
		 * than, equal to or less than "size" bytes of "str2". */
		static	int32_t	compare(const char *str1, const char *str2,
								size_t size);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2", ignoring case. */
		static	int32_t	compareIgnoringCase(const char *str1,
							const char *str2);

		/** Returns -1,0 or 1 if "size" bytes of "str1" are greater
		 *  than, equal to or less than "size" bytes of "str2",
		 *  ignoring case. */
		static	int32_t	compareIgnoringCase(const char *str1,
							const char *str2,
							size_t size);

		/** Returns true if "str" is found among the values in the
		 *  NULL-terminated array "set".  Also returns true if "str"
		 *  is NULL and set is NULL or contains only a NULL-terminator.
		 *  Otherwise returns false. */
		static	bool	inSet(const char *str, const char * const *set);

		/** Returns true if "str" is found among the values in the
		 *  NULL-terminated array "set", ignoring case.  Also returns
		 *  true if "str" is NULL and set is NULL or contains only a
		 *  NULL-terminator.  Otherwise returns false. */
		static	bool	inSetIgnoringCase(const char *str,
						const char * const *set);

		/** Returns true if "haystack" contains "needle" or
		 *  false otherwise. */
		static	bool	contains(const char *haystack,
							const char *needle);

		/** Returns true if "haystack" contains "needle" or
		 *  false otherwise. */
		static	bool	contains(const char *haystack, char needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	const char	*findFirst(const char *haystack,
							const char *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	const char	*findFirst(const char *haystack,
							char needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or a pointer to the NULL terminator
		 *  at the end of the string if not found. */
		static	const char	*findFirstOrEnd(const char *haystack,
							char needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or a pointer to the NULL terminator
		 *  at the end of the string if not found. */
		static	const char	*findFirstOrEnd(const char *haystack,
							const char *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	const char	*findLast(const char *haystack,
							const char *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	const char	*findLast(const char *haystack,
							char needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	char	*findFirst(char *haystack, const char *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	char	*findFirst(char *haystack, char needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or a pointer to the NULL terminator
		 *  at the end of the string if not found. */
		static	char	*findFirstOrEnd(char *haystack,
							const char *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	char	*findLast(char *haystack, const char *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	char	*findLast(char *haystack, char needle);

		/** Returns a pointer to the first occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or NULL if not found. */
		static const char	*findFirstOfSet(const char *haystack,
							const char *set);

		/** Returns a pointer to the first occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or NULL if not found. */
		static char		*findFirstOfSet(char *haystack,
							const char *set);

		/** Returns a pointer to the first occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or a pointer to the NULL terminator at the end
		 *  of the string if not found. */
		static const char	*findFirstOfSetOrEnd(
							const char *haystack,
							const char *set);

		/** Returns a pointer to the first occurence in
		 *  "haystack" of any of the characters in "set".
		 *  or a pointer to the NULL terminator at the end
		 *  of the string if not found. */
		static char		*findFirstOfSetOrEnd(
							char *haystack,
							const char *set);

		/** Returns a pointer to the last occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or NULL if not found. */
		static const char	*findLastOfSet(const char *haystack,
							const char *set);

		/** Returns a pointer to the last occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or NULL if not found. */
		static char		*findLastOfSet(char *haystack,
							const char *set);

		/** Returns a pointer to the last occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or a pointer to the NULL terminator at the end
		 *  of the string if not found. */
		static const char	*findLastOfSetOrEnd(
							const char *haystack,
							const char *set);

		/** Returns a pointer to the last occurence in
		 *  "haystack" of any of the characters in "set".
		 *  or a pointer to the NULL terminator at the end
		 *  of the string if not found. */
		static char		*findLastOfSetOrEnd(
							char *haystack,
							const char *set);

		/** Returns the number of characters, starting at the
		 *  beginning of "haystack" which consists entirely of
		 *  characters in "set". */
		static size_t	lengthContainingSet(const char *haystack,
							const char *set);

		/** Returns the number of characters, starting at the
		 *  beginning of "haystack" which consists entirely of
		 *  characters not in "set". */
		static size_t	lengthNotContainingSet(const char *haystack,
							const char *set);

		/** Creates a duplicate of "str" and returns a pointer
		 *  to it.  Note that this method allocates a buffer for
		 *  the duplicate string internally and returns it.  The
		 *  calling program must deallocate this buffer. */
		static	char	*duplicate(const char *str);

		/** Creates a duplicate of the first "length" bytes of
		 *  "str" and returns a pointer to it.  Note that this
		 *  method allocates a buffer for the duplicate string
		 *  internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	char	*duplicate(const char *str, size_t length);

		/** Converts "str" to uppercase. */
		static	void	upper(char *str); 

		/** Converts "str" to lowercase. */
		static	void	lower(char *str); 

		/** Captitalizes "str", converting characters to upper
		 *  or lower case as necessary. */
		static void	capitalize(char *str);

		/** Trims all spaces off of the right hand side of "str". */
		static	void	rightTrim(char *str);

		/** Rrims all "character"'s off of the right hand side
		 *  of "str". */
		static	void	rightTrim(char *str, char character);

		/** Trims all spaces off of the left hand side of "str". */
		static	void	leftTrim(char *str);

		/** Trims all "character"'s off of the left hand side
		 *  of "str". */
		static	void	leftTrim(char *str, char character);

		/** Trims all spaces off of both sides of "str". */
		static	void	bothTrim(char *str);

		/** Trims all characters off of both sides of "str". */
		static	void	bothTrim(char *str, char character);

		/** Strips all instances of "character" from "str".
		 *  Returns true if any characters were stripped and
		 *  false if no characters were stripped. */
		static	bool	strip(char *str, char character);

		/** Strips all instances of "str2" from "str1".
		 *  Returns true if any characters were stripped and
		 *  false if no characters were stripped. */
		static	bool	strip(char *str1, const char *str2);

		/** Strips all instances of any character in "set" from
		 *  "str1".  Returns true if any characters were stripped
		 *  and false if no characters were stripped. */
		static	bool	stripSet(char *str1, const char *set);

		/** Replaces all instances of "oldchar"
		 *  in "str" with "newchar" */
		static	void	replace(char *str,
					char oldchar, char newchar);

		/** Replaces all instances of any of the characters in
		 *  the set "oldchar" in "str" with "newchar" */
		static	void	replace(char *str,
					const char *oldchars, char newchar);

		/** Returns a new string which is a copy of "str" in which
		 *  all instances of "oldstr" have been replaced with
		 *  "newstr" */
		static	char	*replace(const char *str,
						const char *oldstr,
						const char *newstr);

		/** Returns a new string which is a copy of "str" in which
		 *  all instances of strings found in the NULL-terminated
		 *  array "oldstrset", the lengths of which are given by the
		 *  array "oldstrlen", have been replaced with the commensurate
		 *  string found in the NULL-terminated array "newstrset",
		 *  which must have the same number of members as "oldstrset" */
		static	char	*replace(const char *str,
						const char * const *oldstrset,
						ssize_t *oldstrlen,
						const char * const *newstrset);

		/** Returns a new string which is a copy of "str" in which
		 *  parts that match "from" have been replaced with "to".
		 *
		 *  If "replaceglobal" is true then all matching parts are
		 *  replaced.  If "replaceglobal" is false then only the first
		 *  matching part is replaced. */
		static	char	*replace(const char *str,
						regularexpression *from,
						const char *to,
						bool replaceglobal);

		/** Returns a new string which is a copy of "str" in which
		 *  parts that match "match" have been modified such that
		 *  parts of them that match "from" have been replaced with
		 *  "to".
		 *
		 *  If "matchglobal" is true then all matching parts are
		 *  found and modified.  If "matchglobal" is false then only
		 *  the first matching part is found and modified.
		 *
		 *  If "replaceglobal" is true then all matching parts are
		 *  replaced.  If "replaceglobal" is false then only the first
		 *  matching part is replaced. */
		static	char	*replace(const char *str,
						regularexpression *match,
						bool matchglobal,
						regularexpression *from,
						const char *to,
						bool replaceglobal);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	integerLength(int16_t number);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	integerLength(int32_t number);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	integerLength(int64_t number);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	integerLength(uint16_t number);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	integerLength(uint32_t number);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	integerLength(uint64_t number);

		/** Returns true if the string "val" is an integer and
		 *  false if it is not an integer. */
		static	bool	isInteger(const char *val);

		/** Returns true if the string "val" is an integer and
		 *  false if it is not an integer. */
		static	bool	isInteger(const char *val, int32_t size);

		/** Returns true the string "val" is a number and false
		 *  if it is not a number */
		static	bool	isNumber(const char *val);

		/** Returns true the string "val" is a number and false
		 *  if it is not a number */
		static	bool	isNumber(const char *val, int32_t size);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(int16_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(uint16_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(int16_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(uint16_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(int32_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(uint32_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(int32_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(uint32_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(int64_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(uint64_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(int64_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(uint64_t number,
						uint16_t zeropadding);


		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(float number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(float number,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(float number,
						uint16_t precision,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(double number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(double number,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(double number,
						uint16_t precision,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(long double number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(long double number,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(long double number,
						uint16_t precision,
						uint16_t scale);

		/** Converts "string" to a 64-bit integer. */
		static	int64_t	toInteger(const char *string);

		/** Converts "string" to a 64-bit integer.  If non-NULL,
		 *  endptr will be set to the first character in the
		 *  string after the number. */
		static	int64_t	toInteger(const char *string,
						const char **endptr);

		/** Converts "string" to a 64-bit integer of base "base". */
		static	int64_t	toInteger(const char *string, int32_t base);

		/** Converts "string" to a 64-bit integer of base "base".
		 *  If non-NULL, endptr will be set to the first
		 *  character in the string after the number. */
		static	int64_t	toInteger(const char *string,
					const char **endptr, int32_t base);

		/** Converts "string" to a 64-bit unsigned integer. */
		static	uint64_t	toUnsignedInteger(const char *string);

		/** Converts "string" to a 64-bit unsigned integer.  If
		 *  non-NULL, endptr will be set to the first character
		 *  in the string after the number. */
		static	uint64_t	toUnsignedInteger(const char *string,
							const char **endptr);

		/** Converts "string" to a 64-bit unsigned integer of
		 *  base "base". */
		static	uint64_t	toUnsignedInteger(const char *string,
								int32_t base);

		/** Converts "string" to a 64-bit unsigned integer of
		 *  base "base".
		 *  If non-NULL, endptr will be set to the first
		 *  character in the string after the number. */
		static	uint64_t	toUnsignedInteger(const char *string,
							const char **endptr,
							int32_t base);


		/** Converts "string" to a floating point number. */
		static	long double	toFloat(const char *string);

		/** Converts "string" to a floating point number.
		 *  Presumes "string" to be formatted per the C locale. */
		static	long double	toFloatC(const char *string);

		/** Converts "string" to a floating point number.  If
		 *  non-NULL, endptr will be set to the first character
		 *  in the string after the number. */
		static	long double	toFloat(const char *string,
							const char **endptr);

		/** Converts "amount" which is assumed to be a dollar amount
		 *  into pennies. */
		static	int64_t	convertAmount(const char *amount);

		/** Converts "amount" which is assumed to be a number of
 		 *  pennies into a dollar amount string. */
		static	char	*convertAmount(int64_t amount);

		/** Converts "amount" which is assumed to be a number of
 		 *  pennies into a dollar amount string where there
 		 *  are "padding" places between the dollar sign and decimal
 		 *  point.   These will be space padded if the amount is
 		 *  small enough not to fill them.  */
		static	char	*convertAmount(int64_t amount,
						uint16_t padding);


		/** http escapes "input" and returns it in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	char	*httpEscape(const char *input);

		/** http unescapes "input" and returns it in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	char	*httpUnescape(const char *input);

		/** escapes all characters in "characters" found in
		 *  "input" using \'s and returns it in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	char	*escape(const char *input,
					const char *characters);

		/** unescapes all \-escaped characters found in
		 *  "input" and returns the result in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	char	*unescape(const char *input);

		/** similar to escape() above, but takes an "inputsize"
		 *  parameter and returns the result in "output" and
		 *  "outputsize" rather than in a return value */
		static	void	escape(const char *input, uint64_t inputsize,
					char **output, uint64_t *outputsize,
					const char *characters);

		/** similar to unescape() above, but takes an "inputsize"
		 *  parameter and returns the result in "output" and
		 *  "outputsize" rather than in a return value */
		static	void	unescape(const char *input, uint64_t inputsize,
					char **output, uint64_t *outputsize);

		/** base64-encodes "input" and returns it in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	char	*base64Encode(const unsigned char *input);

		/** similar to base64Encode above but only encodes
		 *  the first "inputsize" characters of "input" */
		static	char	*base64Encode(const unsigned char *input,
						uint64_t inputsize);

		/** similar to base64Encode() above, but returns the
		 *  result in "output" and "outputsize" rather than in a
		 *  return value */
		static	void	base64Encode(const unsigned char *input,
						uint64_t inputsize,
						char **output,
						uint64_t *outputsize);

		/** base64-decodes "input" and returns it in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	unsigned char	*base64Decode(const char *input);

		/** similar to base64Decode above but only decodes
		 *  the first "inputsize" characters of "input" */
		static	unsigned char	*base64Decode(const char *input,
							uint64_t inputsize);

		/** similar to base64Decode() above, but returns the
		 *  result in "output" and "outputsize" rather than in a
		 *  return value */
		static	void	base64Decode(const char *input,
						uint64_t inputsize,
						unsigned char **output,
						uint64_t *outputsize);

		/** Obfuscates "str" by adding 128 to each character. */
		static void	obfuscate(char *str);

		/** Deobfuscates "str" which was obfusacted using the
		 *  obfuscate method of this class. */
		static void	deobfuscate(char *str);

		/** Moves leading spaces to the end of "str" for
		 *  "length" characters.
		 *
		 *  Example: "   hello   " -> "hello      " */
		static	void	leftJustify(char *str, int32_t length);

		/** Moves trailing spaces to the beginning of "str" for
		 *  "length" characters.
		 *  
		 *  Example: "   hello   " -> "      hello"  */
		static	void	rightJustify(char *str, int32_t length);

		/** Centers the text of "str" for "length" characters.
		 *
		 *  Example: "hello      " -> "   hello   "  */
		static	void	center(char *str, int32_t length);

		/** Returns a copy of "string", padded with "padchar" to a
		 *  length of "totallength".  Set "direction" to -1 to
		 *  left-pad, 0 to center-pad and 1 to right-pad.  Note that
		 *  this method allocates a buffer internally and returns it.
		 *  The calling program must deallocate this buffer. */
		static char	*pad(const char *string,
						char padchar,
						int16_t direction,
						uint64_t totallength);

		/** Parses "string" of length "stringlength" delimited by
		 *  "delimiter" of length "delimiterlength" and allocates
		 *  "listlength" elements of "list" to return the parts.  If
		 *  "collapse" is true then multiple instances of the delimiter
		 *  in a row will be interpreted as one instance of the
		 *  delimiter.  Each member of "list" and "list" itseslf must
		 *  be deallocated by the calling program. */
		static void	split(const char *string,
					ssize_t stringlength,
					const char *delimiter,
					ssize_t delimiterlength,
					bool collapse,
					char ***list,
					uint64_t *listlength);

		/** Parses NULL-terminated "string" delimited by "delimiter" of
		 *  length "delimiterlength" and allocates "listlength"
		 *  elements of "list" to return the parts.  If "collapse" is
		 *  true then multiple instances of the delimiter in a row will
		 *  be interpreted as one instance of the delimiter.  Each
		 *  member of "list" and "list" itseslf must be deallocated by
		 *  the calling program. */
		static void	split(const char *string,
					const char *delimiter,
					ssize_t delimiterlength,
					bool collapse,
					char ***list,
					uint64_t *listlength);
		/** Parses "string" of "stringlength" delimited by
		 *  NULL-terminated "delimiter" and allocates "listlength"
		 *  elements of "list" to return the parts.  If "collapse" is
		 *  true then multiple instances of the delimiter in a row will
		 *  be interpreted as one instance of the delimiter.  Each
		 *  member of "list" and "list" itseslf must be deallocated by
		 *  the calling program. */
		static void	split(const char *string,
					ssize_t stringlength,
					const char *delimiter,
					bool collapse,
					char ***list,
					uint64_t *listlength);

		/** Parses NULL-terminated "string" delimited by
		 *  NULL-terminated "delimiter" and allocates "listlength"
		 *  elements of "list" to return the parts.  If "collapse" is
		 *  true then multiple instances of the delimiter in a row will
		 *  be interpreted as one instance of the delimiter.  Each
		 *  member of "list" and "list" itseslf must be deallocated by
		 *  the calling program. */
		static void	split(const char *string,
					const char *delimiter,
					bool collapse,
					char ***list,
					uint64_t *listlength);

		/** Returns a copy of the segment of "str"
		 *  between string indices "start" and "end",
		 *  inclusive. */
		static char	*subString(const char *str,
						size_t start, size_t end);

		/** Returns a copy of the segment of "str"
		 *  between string index "start" and the end
		 *  of the string, inclusive. */
		static char	*subString(const char *str, size_t start);

		/** Creates a new string with "src" inserted into "dest" at
                 *  "index". */
		static char	*insertString(const char *dest,
						const char *src,
						uint64_t index);

		/** Appends "..." to "buffer" of length "length" using
		 *  "format" which should comply with standard printf
		 *  formatting rules.
		 *
		 *  Returns the number of characters written to "buffer"
		 *  excluding the NULL terminator.  If string wasn't large
		 *  enough to accommodate the data then it only writes
		 *  "length" bytes to "buffer" but still returns the number
		 *  of characters that would have been written had "buffer"
		 *  been large enough.
		 *
		 *  If "buffer" is NULL or "length" is 0 then nothing is written
		 *  to "buffer" but the number of characters that would have
		 *  been written had "buffer" been large enough is still
		 *  returned.
		 *
		 *  Returns -1 if an error occurred. */
		static ssize_t	printf(char *buffer, size_t length,
						const char *format, ...);

		/** Appends "argp" to "buffer" of length "length" using
		 *  "format" which should comply with standard printf
		 *  formatting rules.
		 *
		 *  Note that argp is a pointer to a va_list, not just a
		 *  va_list.
		 *
		 *  Returns the number of characters written to "buffer"
		 *  excluding the NULL terminator.  If "buffer" wasn't large
		 *  enough to accommodate the data then it only writes
		 *  "length" bytes to "buffer" but still returns the number
		 *  of characters that would have been written had "buffer"
		 *  been large enough.
		 *
		 *  If "buffer" is NULL or "length" is 0 then nothing is written
		 *  to "buffer" but the number of characters that would have
		 *  been written had "buffer" been large enough is still
		 *  returned.
		 *
		 *  Returns -1 if an error occurred. */
		static ssize_t	printf(char *buffer, size_t length,
						const char *format,
						va_list *argp);

		/** Allocates "buffer" to sufficient size and writes "..." to
		 *  it using "format" which should comply with standard printf
		 *  formatting rules.
		 *
		 *  Returns the number of characters written to "buffer"
		 *  excluding the NULL terminator.
		 *
		 *  If "buffer" is NULL then nothing is written to "buffer" and
		 *  0 is returned.
		 *
		 *  Returns -1 if an error occurred. */
		static ssize_t	printf(char **buffer,
						const char *format, ...);

		/** Allocates "buffer" to sufficient size and writes "argp" to
		 *  it using "format" which should comply with standard printf
		 *  formatting rules.
		 *
		 *  Returns the number of characters written to "buffer"
		 *  excluding the NULL terminator.
		 *
		 *  If "buffer" is NULL then nothing is written to "buffer" and
		 *  0 is returned.
		 *
		 *  Returns -1 if an error occurred. */
		static ssize_t	printf(char **buffer,
						const char *format,
						va_list *argp);

	#include <rudiments/private/charstring.h>
};

#endif
