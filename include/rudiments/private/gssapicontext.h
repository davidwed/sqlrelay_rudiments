// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

	private:
		bool	inquire();
		bool	initiate(const void *name,
					size_t size,
					const void *nametype);
		void	getStatus(uint32_t status, int32_t type);

		gssapicontextprivate	*pvt;
