// Copyright (c) 2003 David Muse
// See the COPYING file for more information

	private:
		bool	close();
#if defined(HAVE_LINUX_STATFS)
		static	const char	*getFsTypeName(long type);
#endif
		void	filesystemClone(const filesystem &f);
		filesystemprivate	*pvt;
