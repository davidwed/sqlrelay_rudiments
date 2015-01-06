// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	protected:
		void	init(unsigned char *initialcontents,
				size_t initialsize, size_t increment);
		void	extend(size_t size);
		void	coalesce();

		void	bytebufferClone(const bytebuffer &v);

		unsigned char	*_buffer();
		size_t		_end();
		void		_end(size_t e);
		size_t		_position();
		void		_position(size_t pos);

	private:
		bytebufferprivate	*pvt;
