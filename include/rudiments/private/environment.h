// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

	private:
		#if defined(HAVE_PUTENV) && !defined(HAVE_SETENV)
			namevaluepairs	envstrings;
		#endif

		#ifdef RUDIMENTS_HAS_THREADS
			static	pthread_mutex_t	*envmutex;
		#endif
