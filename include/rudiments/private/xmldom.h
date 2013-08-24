// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	friend class xmldomnode;
	protected:
			void	init(bool stringcacheenabled);
			void	reset();

			virtual bool	tagStart(const char *name);
			virtual bool	attributeName(const char *name);
			virtual bool	attributeValue(const char *value);
			virtual bool	text(const char *string);
			virtual bool	tagEnd(const char *name);
			virtual bool	comment(const char *string);
			virtual bool	cdata(const char *string);

				xmldom(const xmldom &x);
			xmldom	&operator=(const xmldom &x);

			const char	*cacheString(const char *string);
			void		unCacheString(const char *string);

			xmldomprivate	*pvt;
