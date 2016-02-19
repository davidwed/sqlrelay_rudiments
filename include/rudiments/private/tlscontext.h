// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

	private:
		void	initContext();
		void	freeContext();
		bool	reInit(bool isclient);
		void	clearError();
		void	setError(int32_t ret);
		void	setError(int32_t err, const char *errstr);
		void	setNativeError();

		tlscontextprivate	*pvt;
