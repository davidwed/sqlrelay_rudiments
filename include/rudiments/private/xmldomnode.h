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
		void		xml(stringbuffer *strb,
					filedescriptor *fd,
					bool indent,
					uint16_t *indentlevel) const;
		void		append(stringbuffer *strb,
						filedescriptor *fd,
						const char *str) const;
		void		append(stringbuffer *strb,
						filedescriptor *fd,
						const char *str,
						size_t len) const;
		void		append(stringbuffer *strb,
						filedescriptor *fd,
						char ch) const;
		void		safeAppend(stringbuffer *strb,
						filedescriptor *fd,
						const char *str) const;

				xmldomnode(const xmldomnode &x);
		xmldomnode	&operator=(const xmldomnode &x);

		xmldomnodeprivate	*pvt;
