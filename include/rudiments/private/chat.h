	private:
		filedescriptor	*readfd;
		filedescriptor	*writefd;

		long		timeout;
		stringlist	aborts;

		int	send(const char *string);
		int	expect(const char *string);
