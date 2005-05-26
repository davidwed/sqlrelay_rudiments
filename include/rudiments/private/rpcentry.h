// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

	private:
		rpcent	*re;
		#if defined(HAVE_GETRPCBYNAME_R) && \
				defined(HAVE_GETRPCBYNUMBER_R)
			rpcent	rebuffer;
			char	*buffer;
		#endif

		#if defined(RUDIMENTS_HAS_THREADS) && \
			(!defined(HAVE_GETRPCBYNAME_R) || \
				!defined(HAVE_GETRPCBYNUMBER_R))
			static	mutex	*remutex;
		#endif

		bool	initialize(const char *name, int number);
