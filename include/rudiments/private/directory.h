// Copyright (c) 2004 David Muse
// See the COPYING file for more information

	private:
		DIR		*dir;
		unsigned long	currentindex;

		#if defined(RUDIMENTS_HAS_THREADS) && \
			!defined(HAVE_READDIR_R)
			static	pthread_mutex_t	*rdmutex;
		#endif
