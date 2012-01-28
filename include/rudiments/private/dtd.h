// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		bool	parseDtd();
		bool	newElement(xmldomnode *node);
		bool	parseList(const char *attributelist,
					xmldomnode *element,
					int32_t checkcount, int32_t indent,
					char delimiter, const char *name);
		bool	newAttribute(xmldomnode *node);
		void	nodeError(xmldomnode *node);


			dtd(const dtd &d);
		dtd	&operator=(const dtd &d);

		dtdprivate	*pvt;
