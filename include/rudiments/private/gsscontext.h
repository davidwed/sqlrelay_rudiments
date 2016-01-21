// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

	private:
		bool	inquire();
		bool	initiate(const void *name,
					size_t size,
					const void *nametype);
		void	getStatus(uint32_t status, int32_t type,
						stringbuffer *strb);

		ssize_t	receiveToken(uint32_t *tokenflags,
					void **tokendata,
					size_t *tokensize);
		ssize_t sendToken(uint32_t tokenflags,
					const void *tokendata,
					size_t tokensize);

		ssize_t	fullRead(void *data, ssize_t count);
		ssize_t	fullWrite(const void *data, ssize_t count);

		gsscontextprivate	*pvt;
