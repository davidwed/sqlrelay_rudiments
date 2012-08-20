// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

	private:
		void	appendAbortString(const char *string);
		void	clearAbortStrings();
		int32_t	send(const char *string,
				constnamevaluepairs *variables);
		void	flush();
		int32_t	expect(const char *string, char **abort);
		int32_t	substituteVariables(const char **ch,
						constnamevaluepairs *variables);
		void	charUnescape(const char *str,
					char **newstr, bool second);

		chatprivate	*pvt;
