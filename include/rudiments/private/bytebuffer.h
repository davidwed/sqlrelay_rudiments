// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	protected:
		void		init(unsigned char *initialcontents,
					size_t initialsize, size_t increment);
		void		clearExtents();
		void		clear(bool resetpositions);
		bytebuffer	*copy(unsigned char *data,
						size_t size, bool copyin,
						size_t *bytescopied);
		void		extend(size_t size);
		unsigned char	*coalesce(bool replaceinitial);
		void		bytebufferClone(const bytebuffer &v);

		size_t		_end();
		void		_end(size_t e);
		size_t		_position();
		void		_position(size_t pos);

	private:
		bytebufferprivate	*pvt;
