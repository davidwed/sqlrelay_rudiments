// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	protected:
		void	extend(size_t size);

		unsigned char	*buffer;
		size_t		initialsize;
		size_t		increment;
		size_t		buffersize;
		size_t		position;
		size_t		endofbuffer;
