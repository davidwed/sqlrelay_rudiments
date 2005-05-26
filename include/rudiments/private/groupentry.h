// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		group		*grp;
		#if defined(HAVE_GETGRNAM_R) && defined(HAVE_GETGRGID_R)
			group		grpbuffer;
			char		*buffer;
		#endif

		#if defined(RUDIMENTS_HAS_THREADS) && \
			(!defined(HAVE_GETGRNAM_R) || \
				!defined(HAVE_GETGRUID_R))
			static	mutex	*gemutex;
		#endif

		bool	initialize(const char *groupname, gid_t groupid);
