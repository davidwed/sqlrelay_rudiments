// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		memorypoollist	nodelist;

		size_t	initialsize;
		size_t	increment;

		size_t	freecounter;
		size_t	resizeinterval;
		size_t	totalusedsize;

		// FIXME: it should be possible to copy a memorypool
				memorypool(const memorypool &m);
		memorypool	&operator=(const memorypool &m);
