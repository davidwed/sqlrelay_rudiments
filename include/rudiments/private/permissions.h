// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	static char	*permStringToSDDL(const char *permstring,
							bool directory);
	static char	*permOctalToSDDL(mode_t permoctal,
							bool directory);

	static char	*sddlToPermString(const char *sddl);
	static mode_t	sddlToPermOctal(const char *sddl);