// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		int		cascade;

		xmldomnodetype	type;
		char		*nodename;
		char		*nodevalue;
		xmldomnode	*parent;
		xmldomnode	*next;
		xmldomnode	*previous;
		int		childcount;
		xmldomnode	*firstchild;
		xmldomnode	*lastchild;
		int		attributecount;
		xmldomnode	*firstattribute;
		xmldomnode	*lastattribute;
		xmldomnode	*nullnode;
		int		isnullnode;

		void		set(char **variable, const char *value);
		xmldomnode	*getNode(xmldomnode *first,
					int position, const char *name,
					int count) const;
		int		insertNode(xmldomnode *node, int position,
					xmldomnodetype type,
					xmldomnode **first, xmldomnode **last,
					int *count);
		int		deleteNode(xmldomnode *node, int position,
					const char *name,
					xmldomnode **first, xmldomnode **last,
					int *count);
		stringbuffer	*xml(stringbuffer *string) const;
