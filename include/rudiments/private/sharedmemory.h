// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	private:
		// FIXME: it should be possible to copy a sharedmemory object
				sharedmemory(const sharedmemory &s);
		sharedmemory	&operator=(const sharedmemory &s);

		sharedmemoryprivate	*pvt;
