// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

		void	setOptions(termios *tio);
		termios	*getTermios();

	private:
		baudrate_t	translateBaudString(const char *baud);
		serialportprofileprivate	*pvt;
