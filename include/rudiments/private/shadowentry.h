// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

	private:
		spwd	*sp;
		spwd	spbuffer;
		char	*buffer;

		#if defined(RUDIMENTS_HAS_THREADS) && \
			!defined(HAVE_GETSPNAM_R)
			static	pthread_mutex_t	*spmutex;
		#endif
