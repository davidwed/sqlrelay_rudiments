// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

	private:
		#ifdef HAVE_PTHREAD_MUTEX_T
		pthread_mutex_t	*mut;
		#endif
		#ifdef HAVE_CREATE_MUTEX
		HANDLE		mut;
		#endif
		bool		destroy;

		// FIXME: should be possible to copy a mutex
			mutex(const mutex &m);
		mutex	&operator=(const mutex &m);
