// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_STRING_H
#define RUDIMENTS_STRING_H

#include <rudiments/private/config.h>

#include <sys/types.h>

// The string class provides methods for manipulating strings.

class string {
	public:
			string(const char *str);
			string(const char *str, size_t size);
			string(size_t size);
			~string();


		char	*getString() const;
		size_t	getLength() const;
		size_t	getSize() const;


		void	zero();
		static	void	zero(char *str, size_t size);


		char	*append(const string *str);
		char	*append(const char *str);
		char	*append(const string *str, size_t size);
		char	*append(const char *str, size_t size);

		static	char	*append(char *dest, const string *source);
		static	char	*append(char *dest, const char *source);
		static	char	*append(char *dest, const string *source,
							size_t size);
		static	char	*append(char *dest, const char *source,
							size_t size);


		char	*copyInto(const string *str);
		char	*copyInto(const char *str);
		char	*copyInto(const char *str, size_t size);
		char	*copyInto(unsigned long location,
					const string *str);
		char	*copyInto(unsigned long location,
					const char *str);
		char	*copyInto(unsigned long location,
					const char *str, size_t size);

		static	char	*copyInto(char *dest, const string *source);
		static	char	*copyInto(char *dest, const char *source);
		static	char	*copyInto(char *dest,
					const char *source, size_t size);
		static	char	*copyInto(char *dest, unsigned long location,
					const string *source);
		static	char	*copyInto(char *dest, unsigned long location,
					const char *source);
		static	char	*copyInto(char *dest, unsigned long location,
					const char *source, size_t size);


		int	compare(const string *str) const;
		int	compare(const char *str) const;
		int	compare(const string *str, size_t size) const;
		int	compare(const char *str, size_t size) const;
		int	compareIgnoringCase(const string *str) const;
		int	compareIgnoringCase(const char *str) const;
		int	compareIgnoringCase(const string *str,
						size_t size) const;
		int	compareIgnoringCase(const char *str,
						size_t size) const;

		static	int	compare(const char *str1, const string *str2);
		static	int	compare(const char *str1, const char *str2);
		static	int	compare(const char *str1, const string *str2,
							size_t size);
		static	int	compare(const char *str1, const char *str2,
							size_t size);
		static	int	compareIgnoringCase(const char *str1,
							const string *str2);
		static	int	compareIgnoringCase(const char *str1,
							const char *str2);
		static	int	compareIgnoringCase(const char *str1,
							const string *str2,
							size_t size);
		static	int	compareIgnoringCase(const char *str1,
							const char *str2,
							size_t size);


		int	equals(const string *str) const;
		int	equals(const char *str) const;
		int	equals(const string *str, size_t size) const;
		int	equals(const char *str, size_t size) const;
		int	equalsIgnoringCase(const string *str) const;
		int	equalsIgnoringCase(const char *str) const;
		int	equalsIgnoringCase(const string *str,
						size_t size) const;
		int	equalsIgnoringCase(const char *str,
						size_t size) const;

		static	int	equals(const char *str1, const string *str2);
		static	int	equals(const char *str1, const char *str2);
		static	int	equals(const char *str1, const string *str2,
							size_t size);
		static	int	equals(const char *str1, const char *str2,
							size_t size);
		static	int	equalsIgnoringCase(const char *str1,
							const string *str2);
		static	int	equalsIgnoringCase(const char *str1,
							const char *str2);
		static	int	equalsIgnoringCase(const char *str1,
							const string *str2,
							size_t size);
		static	int	equalsIgnoringCase(const char *str1,
							const char *str2,
							size_t size);


		int	greaterThan(const string *str) const;
		int	greaterThan(const char *str) const;
		int	greaterThan(const string *str, size_t size) const;
		int	greaterThan(const char *str, size_t size) const;
		int	greaterThanIgnoringCase(const string *str) const;
		int	greaterThanIgnoringCase(const char *str) const;
		int	greaterThanIgnoringCase(const string *str,
						size_t size) const;
		int	greaterThanIgnoringCase(const char *str,
						size_t size) const;

		static	int	greaterThan(const char *str1,
						const string *str2);
		static	int	greaterThan(const char *str1,
						const char *str2);
		static	int	greaterThan(const char *str1,
							const string *str2,
							size_t size);
		static	int	greaterThan(const char *str1,
							const char *str2,
							size_t size);
		static	int	greaterThanIgnoringCase(const char *str1,
							const string *str2);
		static	int	greaterThanIgnoringCase(const char *str1,
							const char *str2);
		static	int	greaterThanIgnoringCase(const char *str1,
							const string *str2,
							size_t size);
		static	int	greaterThanIgnoringCase(const char *str1,
							const char *str2,
							size_t size);


		int	lessThan(const string *str) const;
		int	lessThan(const char *str) const;
		int	lessThan(const string *str, size_t size) const;
		int	lessThan(const char *str, size_t size) const;
		int	lessThanIgnoringCase(const string *str) const;
		int	lessThanIgnoringCase(const char *str) const;
		int	lessThanIgnoringCase(const string *str,
						size_t size) const;
		int	lessThanIgnoringCase(const char *str,
						size_t size) const;

		static	int	lessThan(const char *str1,
						const string *str2);
		static	int	lessThan(const char *str1,
						const char *str2);
		static	int	lessThan(const char *str1,
						const string *str2,
						size_t size);
		static	int	lessThan(const char *str1,
						const char *str2,
						size_t size);
		static	int	lessThanIgnoringCase(const char *str1,
						const string *str2);
		static	int	lessThanIgnoringCase(const char *str1,
						const char *str2);
		static	int	lessThanIgnoringCase(const char *str1,
						const string *str2,
						size_t size);
		static	int	lessThanIgnoringCase(const char *str1,
						const char *str2,
						size_t size);


		char	*contains(const string *str) const;
		char	*contains(const char *str) const;
		char	*contains(char ch) const;

		static	char	*contains(const char *haystack,
						const string *needle);
		static	char	*contains(const char *haystack,
						const char *str);
		static	char	*contains(const char *haystack,
						char needle);


		char	*duplicate() const;
		static	char	*duplicate(const char *str);

	#include <rudiments/private/string.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/stringinlines.h>
#endif

#endif
