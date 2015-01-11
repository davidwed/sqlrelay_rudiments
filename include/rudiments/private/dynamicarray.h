// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

	private:
		void		init(uint64_t initialsize, uint64_t increment);
		void		dynamicarrayClone(
					const dynamicarray<valuetype> &v);
		void		extend(uint64_t len);
		valuetype	&find(uint64_t index);
		void		clearExtentList();

		linkedlist< valuetype *>	extents;
		linkedlistnode< valuetype *>	*curext;
		uint64_t	curind;
		uint64_t	initial;
		uint64_t	extsize;
		uint64_t	len;
		uint64_t	size;
