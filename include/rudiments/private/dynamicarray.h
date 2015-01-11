// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

	private:
		void		init(int initialsize, int increment);
		void		extend(int len);
		valuetype	&find(int index);

		linkedlist< dynamicarrayextent<valuetype> *>		extents;
		linkedlistnode< dynamicarrayextent<valuetype> *>	*curext;
		int	curind;
		int	initsize;
		int	extsize;
		int	len;
		int	size;
