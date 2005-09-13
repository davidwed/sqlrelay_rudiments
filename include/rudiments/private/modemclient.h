	protected:
		const char	*_connectscript();
		const char	*_disconnectscript();
		const char	*_phonenumber();

		void	modemclientClone(const modemclient &m);
	private:
		modemclientprivate	*pvt;
