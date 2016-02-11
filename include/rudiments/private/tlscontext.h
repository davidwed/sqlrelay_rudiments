// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

	private:
		void	init();
		void	clear();
		bool	setCertificateAuthority(const char *cafile,
						const char *capath);
		void	clearError();
		void	getError(int32_t ret);

		tlscontextprivate	*pvt;
