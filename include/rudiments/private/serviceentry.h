// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

	private:
		servent		*se;
		servent		sebuffer;
		char		*buffer;

		#if !defined(HAVE_GETSERVBYNAME_R) || \
				!defined(HAVE_GETSERVBYPORT_R)
			static	pthread_mutex_t	*semutex;
		#endif

		bool	initialize(const char *servicename, int port,
						const char *protocol);
