// Copyright (c) 2003 David Muse
// See the COPYING file for more information

	private:
		char	*buffer;
		size_t	size;

		static	int	countLeadingSpaces(char *string, int length);
		static	int	countTrailingSpaces(char *string, int length);
