	private:

		bool	tcSetAttr(int optional_actions,
					termios *termios_p);
		bool	tcFlush(int queue_selector);
		bool	tcFlow(int action);
