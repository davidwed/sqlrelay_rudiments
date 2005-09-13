	protected:
		const char	*_listenscript();
		const char	*_acceptscript();
		const char	*_disconnectscript();

		void	modemserverClone(const modemserver &m);
	private:
		modemserverprivate	*pvt;
