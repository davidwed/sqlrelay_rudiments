// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	private:
		static	void	shutDown(int32_t signum);
		static	void	crash(int32_t signum);
		static	void	defaultShutDown(int32_t signum);
		static	void	defaultCrash(int32_t signum);
		static	void	waitForChildrenToExit(int32_t signum);

		static	daemonprocessprivate	*pvt;
