// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		xmldom	*xmld;
		xmldom	*xmldtd;

		int	parseDtd();
		int	newEntity(xmldomnode *node);
		int	parseList(const char *attributelist,
					xmldomnode *entity,
					int checkcount, int indent,
					const char *name);
		int	newAttribute(xmldomnode *node);
