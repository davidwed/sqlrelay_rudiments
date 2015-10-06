// Copyright (c) 2015 David Muse
// See the COPYING file for more information

	protected:
		void	lowLevelOpen(const char *name, int32_t flags,
						mode_t perms, bool useperms);
		int32_t	lowLevelClose();

		ssize_t	lowLevelRead(void *buffer, ssize_t size) const;
	private:
		void	init();

		static	bool	initUrl();
		static	void	shutDownUrl();

		static	size_t	writeData(void *buffer, size_t size,
						size_t nmemb, void *userp);

		urlprivate	*pvt;
