// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	private:
		struct	sigaction	handlerstruct;
			int		signalnumber;

		// FIXME: should be possible to copy a signalhandler
				signalhandler(const signalhandler &s);
		signalhandler	&operator=(const signalhandler &s);
