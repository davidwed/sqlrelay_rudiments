// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#include <rudiments/linkedlist.h>

template< class datatype > class dynamicarrayextent {
	public:
		dynamicarrayextent(int size);
		~dynamicarrayextent();
		datatype &operator[](int index);
	private:

		int		size;
		unsigned char	**a;
};
