// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

		void	setPackageSpecificData(void *psd);

	private:
		bool	acquire(const void *name,
					size_t namelength,
					const void *nametype);
		void	setStatus(uint32_t status, int32_t type);

		gsscredentialsprivate	*pvt;
