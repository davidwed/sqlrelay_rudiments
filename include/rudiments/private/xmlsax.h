// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
			void	reset();
			void	close();
			bool	parse();
			void	parseFailed();
			bool	parseTag(char current, char *next);
			void	parseTagFailed();
			bool	parseTagName(char current, stringbuffer *name,
						char *next);
			char	parseComment(char current);
			char	parseCData(char current);
			char	parseAttribute(char current, char standalone);
			int	getGeneralEntity(char breakchar, char **buffer);
			void	parseAttributeFailed();
			bool	parseText(char current, char *next);
			void	parseTextFailed();
			char	skipWhitespace(char current);
			char	getCharacter();

			char		*string;
			char		*ptr;
			file		fl;
			#ifdef HAVE_MMAP
			memorymap	mm;
			#endif
			long		line;
			bool		allowtrailingtext;
