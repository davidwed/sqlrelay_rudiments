// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		passwd	*pwd;
		passwd	pwdbuffer;
		char	*buffer;

		#if !defined(HAVE_GETPWNAM_R) || !defined(HAVE_GETPWUID_R)
			static	pthread_mutex_t	*pemutex;
		#endif

		int	initialize(const char *username, uid_t userid);
