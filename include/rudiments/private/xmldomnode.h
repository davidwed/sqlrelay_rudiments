// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		void		init(xmldomnode *nullnode);
		xmldomnode	*getNode(xmldomnode *first,
					uint64_t position, const char *name,
					uint64_t count) const;
		bool		insertNode(xmldomnode *node, uint64_t position,
					xmldomnodetype type,
					xmldomnode **first, xmldomnode **last,
					uint64_t *count);
		bool		deleteNode(xmldomnode *node, uint64_t position,
					const char *name,
					xmldomnode **first, xmldomnode **last,
					uint64_t *count);
		xmldomnode	*unlinkNode(xmldomnode *node, uint64_t position,
					const char *name,
					xmldomnode **first, xmldomnode **last,
					uint64_t *count);
		stringbuffer	*xml(stringbuffer *string) const;
		void		safeAppend(stringbuffer *output,
						const char *str) const;


				xmldomnode(const xmldomnode &x);
		xmldomnode	&operator=(const xmldomnode &x);

		xmldomnodeprivate	*pvt;
