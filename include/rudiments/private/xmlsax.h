// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
			void	reset();
			void	close();
			int	parse();
			void	parseFailed();
			char	parseTag(char current);
			void	parseTagFailed();
			char	parseTagName(char current, stringbuffer **name);
			char	parseComment(char current);
			char	parseCData(char current);
			char	parseAttribute(char current, char standalone);
			int	getGeneralEntity(char breakchar, char **buffer);
			void	parseAttributeFailed();
			char	parseText(char current);
			void	parseTextFailed();
			char	skipWhitespace(char current);
			char	getCharacter();
			char	*string;
			char	*ptr;
			file	fl;
			long	line;
