// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	protected:
		const char	*_address();
		unsigned short	*_port();
		sockaddr_in	*_sin();
	private:
		inetsocketutilprivate	*pvt;
