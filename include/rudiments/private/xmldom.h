// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
			void	reset();

			int	tagStart(char *name);
			int	attributeName(char *name);
			int	attributeValue(char *value);
			int	text(char *string);
			int	tagEnd(char *name);
			int	comment(char *string);
			int	cdata(char *string);

			xmldomnode	*nullnode;
			xmldomnode	*rootnode;
			xmldomnode	*currentparent;
			xmldomnode	*currentattribute;
