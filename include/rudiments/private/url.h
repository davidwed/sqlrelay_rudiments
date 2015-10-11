// Copyright (c) 2015 David Muse
// See the COPYING file for more information

		url(const url &u);
		url	&operator=(const url &u);

	protected:
		bool	lowLevelOpen(const char *name, int32_t flags,
						mode_t perms, bool useperms);
		int32_t	lowLevelClose();

		ssize_t	lowLevelRead(void *buffer, ssize_t size);
	private:
		void	init();

		bool	http(const char *host,
				uint16_t port,
				const char *userpwd,
				const char *path);
		bool	getChunkSize(bool bof);
		bool	perform();

		static	bool	initUrl();
		static	void	shutDownUrl();

		static	size_t	readData(void *buffer, size_t size,
						size_t nmemb, void *userp);

		urlprivate	*pvt;
