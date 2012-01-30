// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	protected:
		virtual	int32_t	safeSelect(long sec, long usec,
						bool read, bool write);

		// FIXME: it should be possible to copy a listener
				listener(const listener &l);
		listener	&operator=(const listener &l);

		listenerprivate	*pvt;
