// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

	private:
		bool	hasRecursiveBreak();
		bool	tagStart(const char *name);
		bool	attributeName(const char *name);
		bool	attributeValue(const char *value);

		codetreegrammarprivate	*pvt;
