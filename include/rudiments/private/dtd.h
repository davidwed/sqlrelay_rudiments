// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		xmldom	xmld;
		xmldom	xmldtd;

		stringbuffer	err;

		bool	parseDtd();
		bool	newElement(xmldomnode *node);
		bool	parseList(const char *attributelist,
					xmldomnode *element,
					int checkcount, int indent,
					char delimiter, const char *name);
		bool	newAttribute(xmldomnode *node);
		void	nodeError(xmldomnode *node);
