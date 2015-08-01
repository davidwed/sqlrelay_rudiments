// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	private:
		void		init(xmldomnode *nullnode);
		xmldomnode	*getNode(xmldomnode *first,
					uint64_t position,
					const char *name,
					bool ignorecase,
					uint64_t count) const;
		xmldomnode	*getAttribute(
					const char *name,
					bool ignorecase) const;
		xmldomnode	*getFirstChild(const char *name,
					bool ignorecase) const;
		xmldomnode	*getFirstChild(const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const;
		xmldomnode	*getFirstTagChild(
					const char *name,
					bool ignorecase) const;
		xmldomnode	*getFirstTagChild(
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const;
		xmldomnode	*getPreviousTagSibling(
					const char *name,
					bool ignorecase) const;
		xmldomnode	*getPreviousTagSibling(
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const;
		xmldomnode	*getNextTagSibling(
					const char *name,
					bool ignorecase) const;
		xmldomnode	*getNextTagSibling(
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const;
		xmldomnode	*getFirstTagDescendent(
					const char *name,
					bool ignorecase) const;
		xmldomnode	*getFirstTagDescendent(
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const;
		bool		insertNode(xmldomnode *node,
					uint64_t position,
					xmldomnodetype type,
					xmldomnode **first,
					xmldomnode **last,
					uint64_t *count);
		bool		deleteFirstChild(
					const char *name,
					bool ignorecase);
		bool		deleteChildren(
					const char *name,
					bool ignorecase);
		bool		deleteChildren(
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase);
		bool		deleteDescendents(
					const char *name,
					bool ignorecase);
		bool		deleteDescendents(
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase);
		bool		unwrapFirstChild(
					const char *name,
					bool ignorecase);
		bool		unwrapChildren(
					const char *name,
					bool ignorecase);
		bool		unwrapDescendents(
					const char *name,
					bool ignorecase);
		bool		renameFirstChild(
					const char *oldname,
					const char *newname,
					bool ignorecase);
		bool		renameChildren(
					const char *oldname,
					const char *newname,
					bool ignorecase);
		bool		renameDescendents(
					const char *oldname,
					const char *newname,
					bool ignorecase);
		bool		deleteNode(xmldomnode *node,
					uint64_t position,
					const char *name,
					xmldomnode **first,
					xmldomnode **last,
					uint64_t *count);
		xmldomnode	*unlinkNode(xmldomnode *node,
					uint64_t position,
					const char *name,
					xmldomnode **first,
					xmldomnode **last,
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
