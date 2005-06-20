	private:
		const filedescriptor	*readfd;
		const filedescriptor	*writefd;

		long		timeout;
		stringlist	aborts;

		void	appendAbortString(const char *string);
		void	clearAbortStrings();
		int	send(const char *string,
				constnamevaluepairs *variables);
		void	flush();
		int	expect(const char *string, char **abort);
		int	substituteVariables(const char **ch,
						constnamevaluepairs *variables);
