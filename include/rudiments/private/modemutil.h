	protected:
		char	*devicename;
		char	*baud;

		bool	configureSerialPort(int fd, const char *baud);
