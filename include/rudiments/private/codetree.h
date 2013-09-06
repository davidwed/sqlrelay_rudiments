// Copyright (c) 2012 David Muse
// See the COPYING file for more information.

	private:
		void	buildNonTerminalNodeAssociations(
					xmldomnode *node);
		const char	getSymbolType(xmldomnode *nt);
		bool	parseChild(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseConcatenation(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseAlternation(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseOption(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseRepetition(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseException(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseTerminal(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseLetter(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseLowerCaseLetter(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseUpperCaseLetter(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseDigit(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseSet(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseBreak(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseNonTerminal(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);

		bool	writeNode(xmldomnode *node,
					stringbuffer *output);
		void	indent(stringbuffer *output);
		void	writeStartEnd(stringbuffer *output,
					const char *string);

		codetreeprivate		*pvt;
