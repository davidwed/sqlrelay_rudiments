// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		bool		cascade;

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
		bool		isnullnode;

		void		init(xmldomnode *nullnode);
		xmldomnode	*getNode(xmldomnode *first,
					int position, const char *name,
					int count) const;
		bool		insertNode(xmldomnode *node, int position,
					xmldomnodetype type,
					xmldomnode **first, xmldomnode **last,
					int *count);
		bool		deleteNode(xmldomnode *node, int position,
					const char *name,
					xmldomnode **first, xmldomnode **last,
					int *count);
		stringbuffer	*xml(stringbuffer *string) const;


				xmldomnode(const xmldomnode &x);
		xmldomnode	&operator=(const xmldomnode &x);
