// Copyright (c) 2012 David Muse
// See the COPYING file for more information.

	private:
		void	buildNonTerminalNodeAssociations(
					xmldomnode *node);
		char	getSymbolType(xmldomnode *nt);
		bool	isTag(xmldomnode *nt);
		bool	parseChild(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseConcatenation(xmldomnode *grammarnode,
					xmldomnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	endOfStringOk(xmldomnode *node);
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
		bool	compareValue(const char *code,
					const char *value,
					size_t *valuelength,
					const char *casesensitive);
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
		bool	parseNonPrintableCharacter(
					xmldomnode *grammarnode,
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

		void	pushBreakStack();
		void	popBreakStack();
		bool	parseBreakStack(const char **codeposition);

		bool	writeNode(xmldomnode *node,
					stringbuffer *output);
		void	indent(stringbuffer *output);
		void	writeStartEnd(stringbuffer *output,
					const char *string);

		codetreeprivate		*pvt;
