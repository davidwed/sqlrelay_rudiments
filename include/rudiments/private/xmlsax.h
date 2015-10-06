// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
			void	reset();
			void	close();
			bool	parse();
			bool	parseLocalFile(const char *filename);
			bool	parseRemoteFile(const char *filename);
			void	parseFailed();
			bool	parseTag(char current, char *next);
			void	parseTagFailed();
			bool	parseTagName(char current,
						stringbuffer *ns,
						stringbuffer *name,
						char *next);
			char	parseComment(char current);
			char	parseCData(char current);
			char	parseAttribute(char current, char standalone);
			int32_t	getGeneralEntity(char breakchar);
			void	parseAttributeFailed();
			bool	parseText(char current, char *next);
			void	parseTextFailed();
			char	skipWhitespace(char current);
			char	getCharacter();
			bool	mapFile();

				xmlsax(const xmlsax &x);
			xmlsax	&operator=(const xmlsax &x);

			xmlsaxprivate	*pvt;
