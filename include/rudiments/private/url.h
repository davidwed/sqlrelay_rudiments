// Copyright (c) 2015 David Muse
// See the COPYING file for more information

	protected:
		void	lowLevelOpen(const char *name, int32_t flags,
						mode_t perms, bool useperms);
		int32_t	lowLevelClose();

	private:
		void	urlClone(const url &u);

		static	bool	initUrl();
		static	void	shutDownUrl();

		static	size_t	writeData(void *buffer, size_t size,
						size_t nmemb, void *userp);

		urlprivate	*pvt;
