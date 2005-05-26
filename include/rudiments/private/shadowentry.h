// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

	private:
		spwd	*sp;
		#if defined(HAVE_GETSPNAM_R)
			spwd	spbuffer;
			char	*buffer;
		#endif

		#if defined(RUDIMENTS_HAS_THREADS) && \
			!defined(HAVE_GETSPNAM_R)
			static	mutex	*spmutex;
		#endif
