// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		xmldom	xmld;
		xmldom	xmldtd;

		int	parseDtd();
		int	newElement(xmldomnode *node);
		int	parseList(const char *attributelist,
					xmldomnode *element,
					int checkcount, int indent,
					char delimiter, const char *name);
		int	newAttribute(xmldomnode *node);
