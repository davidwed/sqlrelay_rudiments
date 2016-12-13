// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		// FIXME: it should be possible to copy a memorypool
				memorypool(const memorypool &m);
		memorypool	&operator=(const memorypool &m);
		memorypoolprivate	*pvt;
