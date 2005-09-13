// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

	private:

		bool	tcSetAttr(int optional_actions,
					termios *termios_p);
		bool	tcFlush(int queue_selector);
		bool	tcFlow(int action);

		serialportprivate	*pvt;
