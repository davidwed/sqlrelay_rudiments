// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
			void	reset();

			bool	tagStart(char *name);
			bool	attributeName(char *name);
			bool	attributeValue(char *value);
			bool	text(char *string);
			bool	tagEnd(char *name);
			bool	comment(char *string);
			bool	cdata(char *string);

			xmldomnode	*nullnode;
			xmldomnode	*rootnode;
			xmldomnode	*currentparent;
			xmldomnode	*currentattribute;
