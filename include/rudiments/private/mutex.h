// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

	private:
		pthread_mutex_t	*mut;

		// FIXME: should be possible to copy a mutex
			mutex(const mutex &m);
		mutex	&operator=(const mutex &m);
