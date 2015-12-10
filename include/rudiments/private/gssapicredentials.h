// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

	private:
		bool	acquire(const void *name,
					size_t namelength,
					const char *password,
					const void *nametype);
		void	getStatus(uint32_t status, int32_t type);

		gssapicredentialsprivate	*pvt;
