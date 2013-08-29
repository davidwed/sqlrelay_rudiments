// Copyright (c) 2012 David Muse
// See the COPYING file for more information.

	private:
		void	buildNonTerminalNodeAssociations(
					rudiments::xmldomnode *node);
		const char	getSymbolType(rudiments::xmldomnode *nt);
		bool	parseChild(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *ntbuffer);
		bool	parseConcatenation(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *ntbuffer);
		bool	parseAlternation(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *ntbuffer);
		bool	parseOption(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *ntbuffer);
		bool	parseRepetition(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *ntbuffer);
		bool	parseException(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *ntbuffer);
		bool	parseTerminal(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *ntbuffer);
		bool	parseLetter(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *ntbuffer);
		bool	parseLowerCaseLetter(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *ntbuffer);
		bool	parseUpperCaseLetter(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *ntbuffer);
		bool	parseDigit(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *ntbuffer);
		bool	parseSet(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *ntbuffer);
		bool	parseBreak(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *ntbuffer);
		bool	parseNonTerminal(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *ntbuffer);

		bool	writeNode(rudiments::xmldomnode *node,
					rudiments::stringbuffer *output);
		void	indent(rudiments::stringbuffer *output);
		void	writeStartEnd(rudiments::stringbuffer *output,
					const char *string);

		codetreeprivate		*pvt;
