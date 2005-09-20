// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

	private:
		void	appendAbortString(const char *string);
		void	clearAbortStrings();
		int	send(const char *string,
				constnamevaluepairs *variables);
		void	flush();
		int	expect(const char *string, char **abort);
		int	substituteVariables(const char **ch,
						constnamevaluepairs *variables);

		chatprivate	*pvt;
