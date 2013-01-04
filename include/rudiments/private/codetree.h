// Copyright (c) 2012 David Muse
// See the COPYING file for more information.

	private:
		bool	parseNonTerminal(const char *name,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *tokenbuffer);
		bool	parseChild(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *tokenbuffer);
		bool	parseConcatenation(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *tokenbuffer);
		bool	parseAlternation(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *tokenbuffer);
		bool	parseOption(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *tokenbuffer);
		bool	parseRepetition(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *tokenbuffer);
		bool	parseException(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *tokenbuffer);
		bool	parseTerminal(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *tokenbuffer);
		bool	parseNonTerminal(rudiments::xmldomnode *grammarnode,
					rudiments::xmldomnode *treeparent,
					const char **codeposition,
					rudiments::stringbuffer *tokenbuffer);

		bool	writeNode(rudiments::xmldomnode *node,
					rudiments::stringbuffer *output);

		rudiments::xmldom	grammar;
		rudiments::xmldomnode	*grammartag;
		bool			error;
		uint32_t		indent;
		bool			previousparsechildretval;
		const char		*finalcodeposition;
