// Copyright (c) 2003 David Muse
// See the COPYING file for more information

	private:
		namevaluepairs	nvp;
		char		delim;

		int		countPairs(const char *paramstring);
		const char	*parseName(const char *data, char **outbuffer);
		const char	*parseValue(const char *data, char **outbuffer);
		const char	*parsePart(int len, char delimiter,
						const char *data,
						char **outbuffer,
						int quotes, int escapedchars);
		int	parseNameLength(const char *data);
		int	parseValueLength(const char *data);
		int	parsePartLength(const char *data, char delimiter,
						int quotes, int escapedchars);
		
