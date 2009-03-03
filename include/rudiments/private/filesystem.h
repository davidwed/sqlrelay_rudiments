// Copyright (c) 2003 David Muse
// See the COPYING file for more information

	private:
		bool	close();
		static	const char	*getFsTypeName(int64_t type);
		void	filesystemClone(const filesystem &f);
		static	bool	getCurrentPropertiesInternal(
						int fd, void *st);
		static	bool	getCurrentPropertiesInternal(
					const char *path, void *st);
		filesystemprivate	*pvt;
