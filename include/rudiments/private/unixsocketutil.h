// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	protected:
		const char	*_filename();
		sockaddr_un	*_sun();
		uint16_t	filenameToPort(const char *filename);
	private:
		unixsocketutilprivate	*pvt;
