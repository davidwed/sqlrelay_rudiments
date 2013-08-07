// Copyright (c) 2012 David Muse
// See the COPYING file for more information.

	private:
		const char	*getSymbolType(rudiments::xmldomnode *nt);
		bool	parseNonTerminal(const char *name,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *ntbuffer);
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
		bool	parseNonTerminal(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *ntbuffer);

		bool	writeNode(rudiments::xmldomnode *node,
					rudiments::stringbuffer *output);

		codetreeprivate		*pvt;
