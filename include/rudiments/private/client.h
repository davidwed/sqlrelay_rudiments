// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	protected:
		char		*connecterror;
		long		timeoutsec;
		long		timeoutusec;
		unsigned int	retrywait;
		unsigned int	retrycount;

	private:
		void	clientInit(char *connecterror,
					long timeoutsec,
					long timeoutusec,
					unsigned int retrywait,
					unsigned int retrycount);
		void	clientClone(const client &c);
