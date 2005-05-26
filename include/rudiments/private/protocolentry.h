// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		protoent	*pe;
		#if defined(HAVE_GETPROTOBYNAME_R) || \
				defined(HAVE_GETPROTOBYNUMBER_R)
			protoent	pebuffer;
			char		*buffer;
		#endif

		#if defined(RUDIMENTS_HAS_THREADS) && \
			(!defined(HAVE_GETPROTOBYNAME_R) || \
				!defined(HAVE_GETPROTOBYNUMBER_R))
			static	mutex	*pemutex;
		#endif

		bool	initialize(const char *protocolname, int number);

