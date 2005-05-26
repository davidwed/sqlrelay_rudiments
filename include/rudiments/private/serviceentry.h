// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

	private:
		servent		*se;
		#if defined(HAVE_GETSERVBYNAME_R) || \
				defined(HAVE_GETSERVBYPORT_R)
			servent		sebuffer;
			char		*buffer;
		#endif

		#if defined(RUDIMENTS_HAS_THREADS) && \
			(!defined(HAVE_GETSERVBYNAME_R) || \
				!defined(HAVE_GETSERVBYPORT_R))
			static	mutex	*semutex;
		#endif

		bool	initialize(const char *servicename, int port,
						const char *protocol);
