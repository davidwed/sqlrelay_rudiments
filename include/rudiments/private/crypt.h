// Copyright (c) 2004 David Muse
// See the COPYING file for more information

	private:
			#ifdef RUDIMENTS_HAS_THREADS
			static pthread_mutex_t	*cryptmutex;
			#endif

		crypt();
		crypt(const crypt &c);
		crypt	&operator=(const crypt &c);
		~crypt();
