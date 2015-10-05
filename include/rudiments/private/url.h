// Copyright (c) 2015 David Muse
// See the COPYING file for more information

	protected:
		virtual	void	lowLevelOpen(const char *name, int32_t flags,
						mode_t perms, bool useperms);

	private:
		void	urlClone(const url &u);

		urlprivate	*pvt;
