// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		group	*grp;
		group	grpbuffer;
		char	*buffer;

		#if defined(RUDIMENTS_HAS_THREADS) && \
			(!defined(HAVE_GETGRNAM_R) || \
				!defined(HAVE_GETGRUID_R))
			static	pthread_mutex_t	*gemutex;
		#endif

		bool	initialize(const char *groupname, gid_t groupid);
