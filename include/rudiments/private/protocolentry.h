// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		protoent	*pe;
		protoent	pebuffer;
		char		*buffer;

		#if !defined(HAVE_GETPROTOBYNAME_R) || \
				!defined(HAVE_GETPROTOBYNUMBER_R)
			static	pthread_mutex_t	*pemutex;
		#endif

		bool	initialize(const char *protocolname, int number);

