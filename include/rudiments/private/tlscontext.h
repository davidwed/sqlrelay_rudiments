// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

	private:
		void	initContext();
		void	initSubContext();
		void	freeContext();
		void	freeSubContext();
		bool	reInit(bool isclient);
		bool	isPeerCertValid();
		bool	loadPeerCert();
		void	clearError();
		void	setError(int32_t ret);
		void	setError(int32_t err, const char *errstr);
		void	setNativeError();

		tlscontextprivate	*pvt;
