// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

	protected:
		const char	*_listenscript();
		const char	*_acceptscript();
		const char	*_disconnectscript();

		void	modemserverClone(const modemserver &m);
	private:
		modemserverprivate	*pvt;
