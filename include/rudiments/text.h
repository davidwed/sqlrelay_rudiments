// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_TEXT_H
#define RUDIMENTS_TEXT_H

#include <rudiments/private/config.h>

// The text class provides methods for performing commonly needed transforms
// on strings of text.

class text {
	public:
		static	void	upper(char *string); 
				// converts "string" to uppercase
		static	void	lower(char *string); 
				// converts "string" to lowercase

		static	void	rightTrim(char *string);
				// trims all spaces off of the right hand
				// side of "string"
		static	void	rightTrim(char *string, char character);
				// trims all characters off of the right hand
				// side of "string"

		static	void	leftTrim(char *string);
				// trims all spaces off of the left hand
				// side of "string"
		static	void	leftTrim(char *string, char character);
				// trims all characters off of the left hand
				// side of "string"

		static	void	bothTrim(char *string);
				// trims all spaces off of both 
				// sides of "string"
		static	void	bothTrim(char *string, char character);
				// trims all characters off of both 
				// sides of "string"

		static	void	strip(char *string, char character);
				// strips all instances of "character"
				// from "string"

		static	void	strip(char *string1, char *string2);
				// strips all instances of "string2"
				// from "string1"

		static	int	integerLength(long number);
				// returns the number of characters needed
				// to represent "number" as a string

		static	int	isInteger(const char *val);
				// returns 1 the string "val" is an integer
				// and 0 if it is not an integer

		static	int	isNumber(const char *val);
				// returns 1 the string "val" is a number
				// and 0 if it is not a number
		static	char	*parseNumber(long number);
				// Returns a string representing "number".
				// The string is allocated inside the function
				// and must be deleted by the calling program.
		static	char	*parseNumber(double number);
		static	char	*parseNumber(double number,
						unsigned short scale);
		static	char	*parseNumber(double number,
						unsigned short precision,
						unsigned short scale);
				// Returns a string representing "number".
				// The string is allocated inside the function
				// and must be deleted by the calling program.

		static	char	*httpEscape(const char *input);
				// http escapes "input" and returns it in a
				// buffer allocated inside the function.
				// This buffer must be deleted by the calling
				// program.

		static	void	leftJustify(char *string, int length);
				// Moves leading spaces to the end of "string"
				// for "length" characters.
				// 
				// Example:
				//	"   hello   " -> "hello      "
		static	void	rightJustify(char *string, int length);
				// Moves trailing spaces to the beginning of
				// "string" for "length" characters.
				// 
				// Example:
				//	"   hello   " -> "      hello"
		static	void	center(char *string, int length);
				// Centers the text of "string" for "length"
				// characters.
				// 
				// Example:
				//	"hello      " -> "   hello   "

	#include <rudiments/private/text.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/textinlines.h>
#endif

#endif
