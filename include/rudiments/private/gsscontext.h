// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

		void	*getContext();

	private:
		bool	getMaxMessageSize(const char *mechname);
		bool	inquire();
		bool	initiate(const void *name,
					size_t size,
					const void *nametype);

		ssize_t	receiveToken(uint32_t *tokenflags,
					void **tokendata,
					size_t *tokensize);
		ssize_t sendToken(uint32_t tokenflags,
					const void *tokendata,
					size_t tokensize);

		ssize_t	fullRead(void *data, ssize_t count);
		ssize_t	fullWrite(const void *data, ssize_t count);

		void	setStatus(uint32_t status, int32_t type);

		gsscontextprivate	*pvt;
