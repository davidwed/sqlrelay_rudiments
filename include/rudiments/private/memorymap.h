// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

	private:
		void	*data;
		size_t	length;

		// FIXME: it should be possible to copy a memorymap
				memorymap(const memorymap &m);
		memorymap	&operator=(const memorymap &m);
