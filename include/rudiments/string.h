// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_STRING_H
#define RUDIMENTS_STRING_H

#include <rudiments/private/stringincludes.h>

// The string class provides methods for manipulating strings.

class string {
	public:

		// The static methods operate on standalone strings.

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


		static	char	*copyInto(char *dest, const char *source);
			// Replaces "dest" with "source".  Assumes that there
			// is enough room in "dest" to accommodate "source".
			// Returns a pointer to "dest".
		static	char	*copyInto(char *dest,
					const char *source, size_t size);
			// Replaces the first "size" bytes of "dest" with
			// "source".  Assumes that "dest" is at least "size"
			// bytes long.
			// Returns a pointer to "dest".
		static	char	*copyInto(char *dest, size_t location,
					const char *source);
			// Replaces "dest" with "source", starting "location"
			// bytes into "dest".  Assumes that there is enough
			// room in "dest" (after "location" bytes) to
			// accommodate "source".
			// Returns a pointer to "dest".
		static	char	*copyInto(char *dest, size_t location,
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


		static	char	*contains(const char *haystack,
						const char *needle);
			// If "haystack" contains "needle", then this method
			// returns a pointer to the substring.  Otherwise it
			// returns NULL.
		static	char	*contains(const char *haystack,
						char needle);
			// If "haystack" contains "needle", then this method
			// returns a pointer to the substring.  Otherwise it
			// returns NULL.


		static	char	*duplicate(const char *str);
			// Creates a duplicate of "str" and returns a pointer
			// to it.  Note that this method allocates a buffer for
			// the duplicate string internally and returns it.  The
			// calling program must deallocate this buffer.


		static	void	upper(char *str); 
			// converts "str" to uppercase
		static	void	lower(char *str); 
			// converts "str" to lowercase

		static	void	rightTrim(char *str);
			// trims all spaces off of the right hand side of "str"
		static	void	rightTrim(char *str, char character);
			// trims all "character"'s off of the right hand side
			// of "str"

		static	void	leftTrim(char *str);
			// trims all spaces off of the left hand side of "str"
		static	void	leftTrim(char *str, char character);
			// trims all "character"'s off of the left hand side
			// of "str"

		static	void	bothTrim(char *str);
			// trims all spaces off of both sides of "str"
		static	void	bothTrim(char *str, char character);
			// trims all characters off of both sides of "str"

		static	void	strip(char *str, char character);
			// strips all instances of "character" from "str"

		static	void	strip(char *str1, char *str2);
			// strips all instances of "str2" from "str1"

		static	int	integerLength(long number);
			// returns the number of characters needed to represent
			// "number" as a string

		static	int	isInteger(const char *val);
		static	int	isInteger(const char *val, int size);
			// returns 1 the string "val" is an integer and 0 if it
			// is not an integer

		static	int	isNumber(const char *val);
		static	int	isNumber(const char *val, int size);
			// returns 1 the string "val" is a number and 0 if it
			// is not a number
		static	char	*parseNumber(long number);
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




		// The following methods allow you to create an instance of
		// the string class to store and manipulate a string.

			string(const char *str);
			// Creates an string containing "str".
			string(const char *str, size_t size);
			// Creates an string "size" bytes long containing "str".
			string(size_t size);
			// Creates an uninitialized string of "size" bytes.
			~string();


		char	*getString() const;
			// Returns a pointer to the internal buffer containing
			// the string.
		size_t	getLength() const;
			// Returns the length of the string currently stored.
		size_t	getSize() const;
			// Returns the size of the internal buffer.


		void	zero();
			// Sets every byte in the internal buffer to NULL.


		char	*append(const string *str);
		char	*append(const char *str);
			// Appends "str" to the currently stored string.
		char	*append(const string *str, size_t size);
		char	*append(const char *str, size_t size);
			// Appends "size" bytes of "str" to the currently
			// stored string.


		char	*copyInto(const char *str);
			// Replaces the current string with "str".  Assumes
			// that there is enough room in the internal buffer
			// to accommodate "str".
			// Returns a pointer to the current string.
		char	*copyInto(const char *str, size_t size);
			// Replaces the first "size" bytes of the current string
			// with "str".  Assumes that there are at least "size"
			// bytes available in the internal buffer.
			// Returns a pointer to the current string.
		char	*copyInto(size_t location, const char *str);
			// Replaces the current string with "str", starting
			// "location" bytes into the internal buffer.  Assumes
			// that there is enough room in the internal buffer
			// (after "location" bytes) to accommodate "str".
			// Returns a pointer to the current string.
		char	*copyInto(size_t location, const char *str,
							size_t size);
			// Replaces "size" bytes of the current string with
			// "str", starting "location" bytes into the internal
			// buffer.  Assumes that there are at least "size"
			// bytes in the internal buffer (after "location"
			// bytes).
			// Returns a pointer to the current string.


		int	compare(const char *str) const;
			// Returns -1,0 or 1 if the current string is greater
			// than, equal to or less than "str".
		int	compare(const char *str, size_t size) const;
			// Returns -1,0 or 1 if "size" bytes of the current
			// string are greater than, equal to or less than
			// "size" bytes of "str".
		int	compareIgnoringCase(const char *str) const;
			// Returns -1,0 or 1 if the current string is greater
			// than, equal to or less than "str", ignoring case.
		int	compareIgnoringCase(const char *str,
						size_t size) const;
			// Returns -1,0 or 1 if "size" bytes of the current
			// string are greater than, equal to or less than
			// "size" bytes of "str", ignoring case.


		char	*contains(const char *str) const;
			// If the current string contains "str", then this
			// method returns a pointer to the substring.
			// Otherwise it returns NULL.
		char	*contains(char ch) const;
			// If the current string contains "ch", then this
			// method returns a pointer to the substring.
			// Otherwise it returns NULL.


		char	*duplicate() const;
			// Creates a duplicate of the string and returns a
			// pointer to it.  Note that this method allocates a
			// buffer for the duplicate string internally and
			// returns it.  The calling program must deallocate
			// this buffer.

	#include <rudiments/private/string.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/stringinlines.h>
#endif

#endif
