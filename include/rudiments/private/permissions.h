// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	static void	*permStringToDacl(const char *permstring,
							bool directory);
	static void	*permOctalToDacl(mode_t permoctal,
							bool directory);

	static char	*daclToPermString(void *dacl);
	static mode_t	daclToPermOctal(void *dacl);
