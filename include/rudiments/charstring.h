// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CHARSTRING_H
#define RUDIMENTS_CHARSTRING_H

#include <rudiments/private/charstringincludes.h>

// The charstring class provides methods for manipulating strings.
//
// Unlike the functions in string.h, these methods are NULL safe.  If any of
// the pointer arguments are NULL, your application will not crash.

class charstring {
	public:

		static	size_t	length(const char *string);
			// Returns the length of "string".

		static	void	zero(char *str, size_t size);
			// Sets "size" bytes of "str" to NULL.

		static	char	*append(char *dest, const char *source);
			// Appends "source" to "dest".  Assumes that there
			// is enough room remaining in "dest" to accommodate
			// the new string.
			// Returns a pointer to "dest".
		static	char	*append(char *dest, const char *source,
							size_t size);
			// Appends "size" bytes of "source" to "dest".  Assumes
			// that there is enough room remaining in "dest" to
			// accommodate the new string.
			// Returns a pointer to "dest".
		static	char	*append(char *dest, long number);
			// Converts "number" to a string and appends it to
			// "dest".  Assumes that there is enough room remaining
			// in "dest" to accommodate the new string.
			// Returns a pointer to "dest".
		static	char	*append(char *dest, unsigned long number);
			// Converts "number" to a string and appends it to
			// "dest".  Assumes that there is enough room remaining
			// in "dest" to accommodate the new string.
			// Returns a pointer to "dest".
		static	char	*append(char *dest, double number);
			// Converts "number" to a string and appends it to
			// "dest".  Assumes that there is enough room remaining
			// in "dest" to accommodate the new string.
			// Returns a pointer to "dest".
		static	char	*append(char *dest, double number,
							unsigned short scale);
			// Converts "number" to a string using "scale" and
			// appends it to "dest".  Assumes that there is enough
			// room remaining in "dest" to accommodate the new
			// string.  Returns a pointer to "dest".
		static	char	*append(char *dest, double number,
						unsigned short precision,
						unsigned short scale);
			// Converts "number" to a string using "precision" and
			// "scale" and appends it to "dest".  Assumes that there
			// is enough room remaining in "dest" to accommodate
			// the new string.  Returns a pointer to "dest".



		static	char	*copy(char *dest, const char *source);
			// Replaces "dest" with "source".  Assumes that there
			// is enough room in "dest" to accommodate "source".
			// Returns a pointer to "dest".
		static	char	*copy(char *dest,
					const char *source, size_t size);
			// Replaces the first "size" bytes of "dest" with
			// "source".  Assumes that "dest" is at least "size"
			// bytes long.
			// Returns a pointer to "dest".
		static	char	*copy(char *dest, size_t location,
					const char *source);
			// Replaces "dest" with "source", starting "location"
			// bytes into "dest".  Assumes that there is enough
			// room in "dest" (after "location" bytes) to
			// accommodate "source".
			// Returns a pointer to "dest".
		static	char	*copy(char *dest, size_t location,
					const char *source, size_t size);
			// Replaces "size" bytes of "dest" with "source",
			// starting "location" bytes into "dest".  Assumes that
			// there are "size" bytes in "dest" (after "location"
			// bytes).
			// Returns a pointer to "dest".


		static	int	compare(const char *str1, const char *str2);
			// Returns -1,0 or 1 if "str1" is greater than, 
			// equal to or less than "str2".
		static	int	compare(const char *str1, const char *str2,
								size_t size);
			// Returns -1,0 or 1 if "size" bytes of "str1" are
			// greater than, equal to or less than "size" bytes of
			// "str2".
		static	int	compareIgnoringCase(const char *str1,
							const char *str2);
			// Returns -1,0 or 1 if "str1" is greater than, 
			// equal to or less than "str2", ignoring case.
		static	int	compareIgnoringCase(const char *str1,
							const char *str2,
							size_t size);
			// Returns -1,0 or 1 if "size" bytes of "str1" are
			// greater than, equal to or less than "size" bytes of
			// "str2", ignoring case.


		static	bool	contains(const char *haystack,
						const char *needle);
			// Returns true if "haystack" contains "needle" or
			// false otherwise.
		static	bool	contains(const char *haystack,
						char needle);
			// Returns true if "haystack" contains "needle" or
			// false otherwise.
		static	char	*findFirst(const char *haystack,
							const char *needle);
			// Returns a pointer to the first occurrance of "needle"
			// in "haystack" or NULL if not found.
		static	char	*findFirst(const char *haystack,
							char needle);
			// Returns a pointer to the first occurrance of "needle"
			// in "haystack" or NULL if not found.
		static	char	*findLast(const char *haystack,
							const char *needle);
			// Returns a pointer to the last occurrance of "needle"
			// in "haystack" or NULL if not found.
		static	char	*findLast(const char *haystack,
							char needle);
			// Returns a pointer to the last occurrance of "needle"
			// in "haystack" or NULL if not found.


		static	char	*duplicate(const char *str);
			// Creates a duplicate of "str" and returns a pointer
			// to it.  Note that this method allocates a buffer for
			// the duplicate string internally and returns it.  The
			// calling program must deallocate this buffer.
		static	char	*duplicate(const char *str, size_t length);
			// Creates a duplicate of the first "length" bytes of
			// "str" and returns a pointer to it.  Note that this
			// method allocates a buffer for the duplicate string
			// internally and returns it.  The calling program must
			// deallocate this buffer.


		static	void	upper(char *str); 
			// Converts "str" to uppercase.
		static	void	lower(char *str); 
			// Converts "str" to lowercase.

		static	void	rightTrim(char *str);
			// Rrims all spaces off of the right hand side of "str".
		static	void	rightTrim(char *str, char character);
			// Rrims all "character"'s off of the right hand side
			// of "str".

		static	void	leftTrim(char *str);
			// Trims all spaces off of the left hand side of "str".
		static	void	leftTrim(char *str, char character);
			// Trims all "character"'s off of the left hand side
			// of "str".

		static	void	bothTrim(char *str);
			// Trims all spaces off of both sides of "str".
		static	void	bothTrim(char *str, char character);
			// Trims all characters off of both sides of "str".

		static	void	strip(char *str, char character);
			// Strips all instances of "character" from "str".

		static	void	strip(char *str1, char *str2);
			// Strips all instances of "str2" from "str1".

		static	int	integerLength(long number);
		static	int	integerLength(unsigned long number);
			// Returns the number of characters needed to represent
			// "number" as a string.

		static	bool	isInteger(const char *val);
		static	bool	isInteger(const char *val, int size);
			// Returns true if the string "val" is an integer and
			// false if it is not an integer.

		static	bool	isNumber(const char *val);
		static	bool	isNumber(const char *val, int size);
			// Returns true the string "val" is a number and false
			// if it is not a number
		static	char	*parseNumber(long number);
		static	char	*parseNumber(unsigned long number);
			// Returns a string representing "number".  The string
			// is allocated inside the function and must be deleted
			// by the calling program.
		static	char	*parseNumber(double number);
		static	char	*parseNumber(double number,
						unsigned short scale);
		static	char	*parseNumber(double number,
						unsigned short precision,
						unsigned short scale);
			// Returns a string representing "number".  The string
			// is allocated inside the function and must be deleted
			// by the calling program.


		static	long	toLong(const char *string);
			// Converts "string" to a long integer.
		static	long	toLong(const char *string, char **endptr);
			// Converts "string" to a long integer.  If non-NULL,
			// endptr will be set to the first character in the
			// string after the number.
		static	long	toLong(const char *string, int base);
			// Converts "string" to a long integer of base "base".
		static	long	toLong(const char *string,
					char **endptr, int base);
			// Converts "string" to a long integer of base "base".
			// If non-NULL, endptr will be set to the first
			// character in the string after the number.


		static	unsigned long	toUnsignedLong(const char *string);
			// Converts "string" to a unsigned long integer.
		static	unsigned long	toUnsignedLong(const char *string,
							char **endptr);
			// Converts "string" to a unsigned long integer.  If
			// non-NULL, endptr will be set to the first character
			// in the string after the number.
		static	unsigned long	toUnsignedLong(const char *string,
								int base);
			// Converts "string" to a unsigned long integer of base
			// "base".
		static	unsigned long	toUnsignedLong(const char *string,
							char **endptr,
							int base);
			// Converts "string" to a unsigned long integer of base
			// "base".
			// If non-NULL, endptr will be set to the first
			// character in the string after the number.


		static	long long	toLongLong(const char *string);
			// Converts "string" to a long long integer.
		static	long long	toLongLong(const char *string,
							char **endptr);
			// Converts "string" to a long long integer.  If
			// non-NULL, endptr will be set to the first character
			// in the string after the number.
		static	long long	toLongLong(const char *string,
								int base);
			// Converts "string" to a long long integer of
			// base "base".
		static	long long	toLongLong(const char *string,
						char **endptr, int base);
			// Converts "string" to a long long integer of
			// base "base".  If non-NULL, endptr will be set to the
			// first character in the string after the number.


		static	unsigned long long	toUnsignedLongLong(
							const char *string);
			// Converts "string" to an unsigned long long integer.
		static	unsigned long long	toUnsignedLongLong(
							const char *string,
							char **endptr);
			// Converts "string" to a unsigned long long integer.  If
			// non-NULL, endptr will be set to the first character
			// in the string after the number.
		static	unsigned long long	toUnsignedLongLong(
							const char *string,
							int base);
			// Converts "string" to a unsigned long long integer of
			// base "base".
		static	unsigned long long	toUnsignedLongLong(
							const char *string,
							char **endptr,
							int base);
			// Converts "string" to a unsigned long long integer of
			// base "base".  If non-NULL, endptr will be set to the
			// first character in the string after the number.


		static	float	toFloat(const char *string);
			// Converts "string" to a floating point number.
		static	float	toFloat(const char *string, char **endptr);
			// Converts "string" to a floating point number.  If
			// non-NULL, endptr will be set to the first character
			// in the string after the number.


		static	double	toDouble(const char *string);
			// Converts "string" to a double precision
			// floating point number.
		static	double	toDouble(const char *string, char **endptr);
			// Converts "string" to a double precision floating
			// point number.  If non-NULL, endptr will be set to
			// the first character in the string after the number.


		static	long double	toLongDouble(const char *string);
			// Converts "string" to a long double precision
			// floating point number.
		static	long double	toLongDouble(const char *string,
								char **endptr);
			// Converts "string" to a long double precision floating
			// point number.  If non-NULL, endptr will be set to
			// the first character in the string after the number.


		static	char	*httpEscape(const char *input);
			// http escapes "input" and returns it in a buffer
			// allocated inside the function.  This buffer must be
			// deleted by the calling program.

		static	void	leftJustify(char *str, int length);
			// Moves leading spaces to the end of "str" for
			// "length" characters.
			// 
			// Example:
			//	"   hello   " -> "hello      "
		static	void	rightJustify(char *str, int length);
			// Moves trailing spaces to the beginning of "str" for
			// "length" characters.
			// 
			// Example:
			//	"   hello   " -> "      hello"
		static	void	center(char *str, int length);
			// Centers the text of "str" for "length" characters.
			// 
			// Example:
			//	"hello      " -> "   hello   "

	#include <rudiments/private/charstring.h>
};

#endif
