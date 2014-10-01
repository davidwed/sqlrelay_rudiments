// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	protected:
		char		**_connecterror();
		int32_t		_timeoutsec();
		int32_t		_timeoutusec();
		uint32_t	_retrywait();
		uint32_t	_tries();

		void	initialize(char *connecterror,
					int32_t timeoutsec,
					int32_t timeoutusec,
					uint32_t retrywait,
					uint32_t tries);
		void	clientClone(const client &c);

	private:
		clientprivate	*pvt;
