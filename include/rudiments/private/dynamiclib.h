// Copyright (c) 2004 David Muse
// See the COPYING file for more information

	private:
			void	*handle;

			#ifdef RUDIMENTS_HAS_THREADS
			static pthread_mutex_t	*errormutex;
			#endif

					dynamiclib(const dynamiclib &d);
			dynamiclib	&operator=(const dynamiclib &d);
