// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	private:
		static	void	defaultShutDown();
		static	void	defaultCrash();
		static	void	shutDown();
		static	void	crash();

		static	signalhandler	shutdownhandler;
		static	void		(*shutdownfunc)(int);

		static	signalhandler	crashhandler;
		static	void		(*crashfunc)(int);

		static	signalhandler	deadchildhandler;
		static	void		registerWaitForChildren();

		static	void		waitForChildren();
