// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	private:
		static	void	shutDown();

		static	signalhandler	*sighandler;
		static	void		(*shutdownfunc)(int);

		static	signalhandler	*deadchildhandler;
		static	void		waitOnChildren();
