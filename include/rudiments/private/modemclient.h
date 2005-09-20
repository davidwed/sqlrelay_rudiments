// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

	protected:
		const char	*_connectscript();
		const char	*_disconnectscript();
		const char	*_phonenumber();

		void	modemclientClone(const modemclient &m);
	private:
		modemclientprivate	*pvt;
