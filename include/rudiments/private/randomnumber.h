// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

	private:
		#if defined(RUDIMENTS_HAS_THREADS) && \
			!defined(HAVE_RAND_R)
			static	pthread_mutex_t	*rnmutex;
		#endif
