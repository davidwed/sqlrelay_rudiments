// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		hostent	*he;
		#if defined(HAVE_GETHOSTBYNAME_R) && \
				defined(HAVE_GETHOSTBYADDR_R)
			hostent	hebuffer;
			char	*buffer;
		#endif

		#if defined(RUDIMENTS_HAS_THREADS) && \
			(!defined(HAVE_GETHOSTBYNAME_R) || \
				!defined(HAVE_GETHOSTBYADDR_R))
			static	pthread_mutex_t	*hemutex;
		#endif

		bool	initialize(const char *hostname,
					const char *address,
					int len, int type);
