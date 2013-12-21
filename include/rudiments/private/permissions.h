// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	static char	*permStringToSDDL(const char *permstring,
							bool directory);
	static char	*permOctalToSDDL(mode_t permoctal,
							bool directory);

	static char	*sddlToPermString(const char *sddl);
	static mode_t	sddlToPermOctal(const char *sddl);
	static void	processSddlPerm(const char *sddl, const char *sid,
						uint8_t which, mode_t *perms);
	static void	sddlPermToPermOctal(const char *section,
						uint8_t which, mode_t *perms);
