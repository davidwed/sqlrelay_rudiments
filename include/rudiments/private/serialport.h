// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

	private:

		bool	tcSetAttr(int32_t optional_actions,
					termios *termios_p);
		bool	tcFlush(int32_t queue_selector);
		bool	tcFlow(int32_t action);

		serialportprivate	*pvt;
