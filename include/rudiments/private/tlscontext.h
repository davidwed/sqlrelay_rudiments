// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

	private:
		bool	init(bool client);
		void	clearError();
		void	setError(int32_t ret);
		void	setError(int32_t err, const char *errstr);
		void	setNativeError();

		tlscontextprivate	*pvt;
