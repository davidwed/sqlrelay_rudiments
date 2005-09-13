// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	protected:
		char		**_connecterror();
		long		_timeoutsec();
		long		_timeoutusec();
		unsigned long	_retrywait();
		unsigned long	_retrycount();

		void	initialize(char *connecterror,
					long timeoutsec,
					long timeoutusec,
					unsigned long retrywait,
					unsigned long retrycount);
		void	clientClone(const client &c);

	private:
		clientprivate	*pvt;
