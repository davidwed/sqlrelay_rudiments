// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	private:
		static	void	shutDown(int signum);
		static	void	crash(int signum);
		static	void	defaultShutDown(int signum);
		static	void	defaultCrash(int signum);
		static	void	waitForChildrenToExit(int signum);

		static	daemonprocessprivate	*pvt;
