// Copyright (c) 2003 David Muse
// See the COPYING file for more information

	private:
		bool	close();
		static	const char	*getFsTypeName(long type);
		void	filesystemClone(const filesystem &f);
		filesystemprivate	*pvt;
