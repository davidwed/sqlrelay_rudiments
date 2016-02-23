// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

		void	*getContext();
		void	setTokenFormat(gsstokenformat_t tokenformat);

	private:
		bool	getMaxMessageSize(const char *mechname);
		bool	inquire();
		bool	initiate(const void *name,
					size_t size,
					const void *nametype);

		ssize_t	receiveToken(uint32_t *tokenflags,
					void **tokendata,
					size_t *tokensize);
		ssize_t	receiveKrbToken(uint32_t *tokenflags,
					void **tokendata,
					size_t *tokensize);
		ssize_t	receiveTlsToken(uint32_t *tokenflags,
					void **tokendata,
					size_t *tokensize);

		ssize_t sendToken(uint32_t tokenflags,
					const void *tokendata,
					size_t tokensize);
		ssize_t sendKrbToken(uint32_t tokenflags,
					const void *tokendata,
					size_t tokensize);
		ssize_t sendTlsToken(uint32_t tokenflags,
					const void *tokendata,
					size_t tokensize);

		ssize_t	fullRead(void *data, ssize_t count);
		ssize_t	fullWrite(const void *data, ssize_t count);

		bool	checkFlags(uint32_t actualflags,
					uint32_t desiredflags);

		void	setStatus(uint32_t status, int32_t type);

		gsscontextprivate	*pvt;
