// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		void	extend(unsigned long size);

		unsigned char	*buffer;
		unsigned long	initialsize;
		unsigned long	increment;
		unsigned long	buffersize;
		unsigned long	position;
		unsigned long	end;
