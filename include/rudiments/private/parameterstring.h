// Copyright (c) 2003 David Muse
// See the COPYING file for more information

	private:
		namevaluepairs	nvp;
		char		delim;

		int	countPairs(const char *paramstring);
		char	*parseName(const char *data, char **outbuffer);
		char	*parseValue(const char *data, char **outbuffer);
		char	*parsePart(int len, char delimiter,
					const char *data, char **outbuffer,
					int quotes, int escapedchars);
		int	parseNameLength(const char *data);
		int	parseValueLength(const char *data);
		int	parsePartLength(const char *data, char delimiter,
						int quotes, int escapedchars);
		
