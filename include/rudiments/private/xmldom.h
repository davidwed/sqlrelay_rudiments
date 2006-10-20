// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	friend class xmldomnode;
	protected:
			void	reset();

			bool	tagStart(const char *name);
			bool	attributeName(const char *name);
			bool	attributeValue(const char *value);
			bool	text(const char *string);
			bool	tagEnd(const char *name);
			bool	comment(const char *string);
			bool	cdata(const char *string);

				xmldom(const xmldom &x);
			xmldom	&operator=(const xmldom &x);

			const char	*cacheString(const char *string);
			void		unCacheString(const char *string);

			xmldomprivate	*pvt;
