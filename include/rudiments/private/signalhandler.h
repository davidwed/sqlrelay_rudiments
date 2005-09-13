// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	private:
		// FIXME: should be possible to copy a signalhandler
				signalhandler(const signalhandler &s);
		signalhandler	&operator=(const signalhandler &s);
		signalhandlerprivate	*pvt;
