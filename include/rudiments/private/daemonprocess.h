// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	private:
		static	void	defaultShutDown();
		static	void	defaultCrash();
		static	void	shutDown();
		static	void	crash();
		static	void	waitForChildrenToExit();

		daemonprocessprivate	*pvt;
