	protected:
		const char	*connectscript;
		const char	*disconnectscript;
		const char	*phonenumber;
		unsigned long	retrywait;
		unsigned long	retrycount;

		void	modemclientClone(const modemclient &m);
