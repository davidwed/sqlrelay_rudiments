// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	friend class stringbuffer;
	private:
		unsigned char	*buffer;
		unsigned long	initialsize;
		unsigned long	increment;
		unsigned long	buffersize;
		unsigned long	position;
