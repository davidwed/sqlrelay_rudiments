// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

	private:
		rpcent	*re;
		rpcent	rebuffer;
		char	*buffer;

		#if defined(RUDIMENTS_HAS_THREADS) && \
			(!defined(HAVE_GETRPCBYNAME_R) || \
				!defined(HAVE_GETRPCBYNUMBER_R))
			static	pthread_mutex_t	*remutex;
		#endif

		bool	initialize(const char *name, int number);
