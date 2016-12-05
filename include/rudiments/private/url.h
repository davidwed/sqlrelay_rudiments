// Copyright (c) 2015 David Muse
// See the COPYING file for more information

		url(const url &u);
		url	&operator=(const url &u);

		bool	getCurrentProperties();

		off64_t	getCurrentPosition() const;
		off64_t	setPositionRelativeToBeginning(off64_t offset) const;
		off64_t	setPositionRelativeToCurrent(off64_t offset) const;
		off64_t	setPositionRelativeToEnd(off64_t offset) const;

		char	*getContents();
		ssize_t	getContents(unsigned char *buffer,size_t buffersize);

		static	char	*getContents(const char *name);
		static	ssize_t	getContents(const char *name,
						unsigned char *buffer,
						size_t buffersize);

	protected:
		bool	lowLevelOpen(const char *name, int32_t flags,
						mode_t perms, bool useperms);
		int32_t	lowLevelClose();

		ssize_t	lowLevelRead(void *buffer, ssize_t size);
	private:
		void	init();

		bool	httpOpen(const char *urlname, const char *userpwd);
		bool	getChunkSize(bool bof);

		bool	curlPerform();
		static	size_t	curlReadData(void *buffer, size_t size,
						size_t nmemb, void *userp);

		static	bool	initUrl();
		static	void	shutDownUrl();

		urlprivate	*pvt;
