// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
			void	reset();
			void	close();
			int	parse();
			void	parseFailed();
			int	parseTag(char current, char *next);
			void	parseTagFailed();
			int	parseTagName(char current, stringbuffer **name,
						char *next);
			char	parseComment(char current);
			char	parseCData(char current);
			char	parseAttribute(char current, char standalone);
			int	getGeneralEntity(char breakchar, char **buffer);
			void	parseAttributeFailed();
			int	parseText(char current, char *next);
			void	parseTextFailed();
			char	skipWhitespace(char current);
			char	getCharacter();
			char	*string;
			char	*ptr;
			file	fl;
			long	line;
			int	allowtrailingtext;
