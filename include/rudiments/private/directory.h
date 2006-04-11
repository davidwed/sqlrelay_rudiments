// Copyright (c) 2004 David Muse
// See the COPYING file for more information

	private:
		// how do we effectively make a copy of a DIR?
		// since I don't know how, for now, these are private
				directory(const directory &d);
		directory	&operator=(const directory &d);

		static	int64_t	pathConf(const char *pathname, int name);

		int64_t		fpathConf(int name);

		directoryprivate	*pvt;

		#ifdef HAVE_READDIR_R
		int64_t	bufferSize(DIR *dirp);
		#endif
