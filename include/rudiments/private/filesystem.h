// Copyright (c) 2003 David Muse
// See the COPYING file for more information

	private:
		void	close();
#if defined(HAVE_LINUX_STATFS)
		static	const char	*getFsTypeName(long type);
#endif

#ifdef HAVE_STATVFS
		struct	statvfs	st;
#else
		struct	statfs	st;
#endif

		int	fd;
		bool	closeflag;

		void	filesystemClone(const filesystem &f);
