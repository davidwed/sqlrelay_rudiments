	private:
		filedescriptor	*readfd;
		filedescriptor	*writefd;

		long		timeout;
		stringlist	aborts;

		void	appendAbortString(const char *string);
		void	clearAbortStrings();
		int	send(const char *string,
					namevaluepairs *variables);
		int	expect(const char *string);
		int	substituteVariables(char **ch,
					namevaluepairs *variables);
