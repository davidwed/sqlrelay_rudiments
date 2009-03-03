// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CHARSTRING_H
#define RUDIMENTS_CHARSTRING_H

#include <rudiments/private/charstringincludes.h>

// The charstring class provides methods for manipulating strings.
//
// Unlike the functions in string.h, these methods are NULL safe.  If any of
// the pointer arguments are NULL, your application will not crash.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class charstring {
	public:

		static	size_t	length(const char *string);
			// Returns the length of "string".

		static	size_t	length(const unsigned char *string);
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
		static	char	*append(char *dest, int32_t number);
			// Converts "number" to a string and appends it to
			// "dest".  Assumes that there is enough room remaining
			// in "dest" to accommodate the new string.
			// Returns a pointer to "dest".
		static	char	*append(char *dest, uint32_t number);
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
		static	bool	contains(const char *haystack, char needle);
			// Returns true if "haystack" contains "needle" or
			// false otherwise.

		static	const char	*findFirst(const char *haystack,
							const char *needle);
			// Returns a pointer to the first occurrance of "needle"
			// in "haystack" or NULL if not found.
		static	const char	*findFirst(const char *haystack,
							char needle);
			// Returns a pointer to the first occurrance of "needle"
			// in "haystack" or NULL if not found.
		static	const char	*findLast(const char *haystack,
							const char *needle);
			// Returns a pointer to the last occurrance of "needle"
			// in "haystack" or NULL if not found.
		static	const char	*findLast(const char *haystack,
							char needle);
			// Returns a pointer to the last occurrance of "needle"
			// in "haystack" or NULL if not found.

		static	char	*findFirst(char *haystack, const char *needle);
			// Returns a pointer to the first occurrance of "needle"
			// in "haystack" or NULL if not found.
		static	char	*findFirst(char *haystack, char needle);
			// Returns a pointer to the first occurrance of "needle"
			// in "haystack" or NULL if not found.
		static	char	*findLast(char *haystack, const char *needle);
			// Returns a pointer to the last occurrance of "needle"
			// in "haystack" or NULL if not found.
		static	char	*findLast(char *haystack, char needle);
			// Returns a pointer to the last occurrance of "needle"
			// in "haystack" or NULL if not found.

		static const char	*findFirstOfSet(const char *haystack,
							const char *set);
			// Returns a pointer to the first occurence in
			// "haystack" of any of the characters in "set".
		static char		*findFirstOfSet(char *haystack,
							const char *set);
			// Returns a pointer to the first occurence in
			// "haystack" of any of the characters in "set".


		static size_t	lengthContainingSet(const char *haystack,
							const char *set);
			// Returns the number of characters, starting at the
			// beginning of "haystack" which consists entirely of
			// characters in "set".
		static size_t	lengthNotContainingSet(const char *haystack,
							const char *set);
			// Returns the number of characters, starting at the
			// beginning of "haystack" which consists entirely of
			// characters not in "set".


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

		static	void	strip(char *str1, const char *str2);
			// Strips all instances of "str2" from "str1".

		static	int	integerLength(int16_t number);
		static	int	integerLength(int32_t number);
		static	int	integerLength(int64_t number);
		static	int	integerLength(uint16_t number);
		static	int	integerLength(uint32_t number);
		static	int	integerLength(uint64_t number);
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
		static	char	*parseNumber(int16_t number);
		static	char	*parseNumber(uint16_t number);
		static	char	*parseNumber(int16_t number,
						unsigned short zeropadding);
		static	char	*parseNumber(uint16_t number,
						unsigned short zeropadding);
		static	char	*parseNumber(int32_t number);
		static	char	*parseNumber(uint32_t number);
		static	char	*parseNumber(int32_t number,
						unsigned short zeropadding);
		static	char	*parseNumber(uint32_t number,
						unsigned short zeropadding);
		static	char	*parseNumber(int64_t number);
		static	char	*parseNumber(uint64_t number);
		static	char	*parseNumber(int64_t number,
						unsigned short zeropadding);
		static	char	*parseNumber(uint64_t number,
						unsigned short zeropadding);
			// Returns a string representing "number".  The string
			// is allocated inside the function and must be deleted
			// by the calling program.
		static	char	*parseNumber(float number);
		static	char	*parseNumber(float number,
						unsigned short scale);
		static	char	*parseNumber(float number,
						unsigned short precision,
						unsigned short scale);
		static	char	*parseNumber(double number);
		static	char	*parseNumber(double number,
						unsigned short scale);
		static	char	*parseNumber(double number,
						unsigned short precision,
						unsigned short scale);
		static	char	*parseNumber(long double number);
		static	char	*parseNumber(long double number,
						unsigned short scale);
		static	char	*parseNumber(long double number,
						unsigned short precision,
						unsigned short scale);
			// Returns a string representing "number".  The string
			// is allocated inside the function and must be deleted
			// by the calling program.


		static	int64_t	toInteger(const char *string);
			// Converts "string" to a 64-bit integer.
		static	int64_t	toInteger(const char *string, char **endptr);
			// Converts "string" to a 64-bit integer.  If non-NULL,
			// endptr will be set to the first character in the
			// string after the number.
		static	int64_t	toInteger(const char *string, int base);
			// Converts "string" to a 64-bit integer of base "base".
		static	int64_t	toInteger(const char *string,
					char **endptr, int base);
			// Converts "string" to a 64-bit integer of base "base".
			// If non-NULL, endptr will be set to the first
			// character in the string after the number.


		static	uint64_t	toUnsignedInteger(const char *string);
			// Converts "string" to a 64-bit unsigned integer.
		static	uint64_t	toUnsignedInteger(const char *string,
							char **endptr);
			// Converts "string" to a 64-bit unsigned integer.  If
			// non-NULL, endptr will be set to the first character
			// in the string after the number.
		static	uint64_t	toUnsignedInteger(const char *string,
								int base);
			// Converts "string" to a 64-bit unsigned integer of
			// base "base".
		static	uint64_t	toUnsignedInteger(const char *string,
							char **endptr,
							int base);
			// Converts "string" to a 64-bit unsigned integer of
			// base "base".
			// If non-NULL, endptr will be set to the first
			// character in the string after the number.


		static	long double	toFloat(const char *string);
			// Converts "string" to a floating point number.
		static	long double	toFloat(const char *string,
							char **endptr);
			// Converts "string" to a floating point number.  If
			// non-NULL, endptr will be set to the first character
			// in the string after the number.


		static	char	*httpEscape(const char *input);
			// http escapes "input" and returns it in a buffer
			// allocated inside the function.  This buffer must be
			// deleted by the calling program.
		static	char	*httpUnescape(const char *input);
			// http unescapes "input" and returns it in a buffer
			// allocated inside the function.  This buffer must be
			// deleted by the calling program.

		static	char	*escape(const char *input,
					const char *characters);
			// escapes all characters in "characters" found in
			// "input" using \'s and returns it in a buffer
			// allocated inside the function.  This buffer must be
			// deleted by the calling program.
		static	char	*unescape(const char *input);
			// unescapes all \-escaped characters found in
			// "input" and returns the result in a buffer
			// allocated inside the function.  This buffer must be
			// deleted by the calling program.

		static	void	escape(const char *input, uint64_t inputsize,
					char **output, uint64_t *outputsize,
					const char *characters);
			// similar to escape() above, but takes an "inputsize"
			// parameter and returns the result in "output" and
			// "outputsize" rather than in a return value
		static	void	unescape(const char *input, uint64_t inputsize,
					char **output, uint64_t *outputsize);
			// similar to unescape() above, but takes an "inputsize"
			// parameter and returns the result in "output" and
			// "outputsize" rather than in a return value

		static	char	*base64Encode(const unsigned char *input);
			// base64-encodes "input" and returns it in a buffer
			// allocated inside the function.  This buffer must be
			// deleted by the calling program.
		static	char	*base64Encode(const unsigned char *input,
						uint64_t inputsize);
			// similar to base64Encode above but only encodes
			// the first "inputsize" characters of "input"
		static	void	base64Encode(const unsigned char *input,
						uint64_t inputsize,
						char **output,
						uint64_t *outputsize);
			// similar to base64Encode() above, but returns the
			// result in "output" and "outputsize" rather than in a
			// return value

		static	unsigned char	*base64Decode(const char *input);
			// base64-decodes "input" and returns it in a buffer
			// allocated inside the function.  This buffer must be
			// deleted by the calling program.
		static	unsigned char	*base64Decode(const char *input,
							uint64_t inputsize);
			// similar to base64Decode above but only decodes
			// the first "inputsize" characters of "input"
		static	void	base64Decode(const char *input,
						uint64_t inputsize,
						unsigned char **output,
						uint64_t *outputsize);
			// similar to base64Decode() above, but returns the
			// result in "output" and "outputsize" rather than in a
			// return value

		static	void	leftJustify(char *str, int length);
			// Moves leading spaces to the end of "str" for
			// "length" characters.
			// 
			// Example:
			//	"   hello   " -> "hello      "
		static  void    rightPad(char *str, int lngth,
					char padchar, bool fill);
			// Moves trailing spaces to the beginning of "str" for
			// "length" characters and replaces them with the
			// padchar.
			//
			// Example when padchar is X:
			//      "hello      " -> "XXXXXXhello"
			// Example when padchar is X and lngth is 10 and fill
			// is true
			//	"hello" -> "XXXXhello"
			// Example when padchar is X and lngth is 10 and fill
			// is false
			//      "hello " -> " hello"
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


		static void	safePrint(const char *str);
			// Prints "str", however all non-printing characters
			// are printed as hex values of the format: (0x0a) and
			// carriage returns, line feeds and tabs are printed
			// as \n, \r and \t.
		static void	safePrint(const char *str, int length);
			// safePrint()'s "length" characters of "str"


		// these methods print out the bits in each value
		static void	printBits(unsigned char value);
		static void	printBits(uint16_t value);
		static void	printBits(uint32_t value);
		static void	printBits(uint64_t value);
		static void	printBits(char value);
		static void	printBits(int16_t value);
		static void	printBits(int32_t value);
		static void	printBits(int64_t value);
		static void	printBits(unsigned char *bits, uint64_t size);


		// These methods parse "string" delimited by "delimiter"
		// and allocate "listlength" elements of "list" to return the
		// parts.  Each member of "list" and "list" must be deallocated
		// by the calling program.  In the versions of the method
		// where "stringlength" or "delimiterlength" are not
		// supplied, string and/or delimiter are presumed to be
		// NULL terminated strings.
		static void	split(const char *string,
					ssize_t stringlength,
					const char *delimiter,
					ssize_t delimiterlength,
					bool collapse,
					char ***list,
					uint64_t *listlength);
		static void	split(const char *string,
					const char *delimiter,
					ssize_t delimiterlength,
					bool collapse,
					char ***list,
					uint64_t *listlength);
		static void	split(const char *string,
					ssize_t stringlength,
					const char *delimiter,
					bool collapse,
					char ***list,
					uint64_t *listlength);
		static void	split(const char *string,
					const char *delimiter,
					bool collapse,
					char ***list,
					uint64_t *listlength);

		static char	*subString(const char *str,
						size_t start, size_t end);
				// Returns a copy of the segment of "str"
				// between string indices "start" and "end".

	#include <rudiments/private/charstring.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
