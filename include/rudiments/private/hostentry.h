// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		hostent	*he;
		hostent	hebuffer;
		char	*buffer;

		#if !defined(HAVE_GETHOSTBYNAME_R) || \
				!defined(HAVE_GETHOSTBYADDR_R)
			static	pthread_mutex_t	*hemutex;
		#endif

		int	initialize(const char *hostname,
					const char *address,
					int len, int type);
