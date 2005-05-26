// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		passwd	*pwd;
		#if defined(HAVE_GETPWNAM_R) && defined(HAVE_GETPWUID_R)
			passwd	pwdbuffer;
			char	*buffer;
		#endif

		#if defined(RUDIMENTS_HAS_THREADS) && \
			(!defined(HAVE_GETPWNAM_R) || !defined(HAVE_GETPWUID_R))
			static	mutex	*pemutex;
		#endif

		bool	initialize(const char *username, uid_t userid);
