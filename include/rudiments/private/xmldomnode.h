// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

	friend class codetree;
	friend class xmldomevents;
	private:
		void		init(xmldomnode *nullnode);
		xmldomnode	*getNode(xmldomnode *first,
					uint64_t position,
					const char *ns,
					const char *name,
					bool ignorecase,
					uint64_t count) const;
		xmldomnode	*getAttribute(
					const char *name,
					bool ignorecase) const;
		xmldomnode	*getFirstChild(
					const char *ns,
					const char *name,
					bool ignorecase) const;
		xmldomnode	*getFirstChild(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const;
		xmldomnode	*getFirstTagChild(
					const char *ns,
					const char *name,
					bool ignorecase) const;
		xmldomnode	*getFirstTagChild(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const;
		xmldomnode	*getPreviousTagSibling(
					const char *ns,
					const char *name,
					bool ignorecase) const;
		xmldomnode	*getPreviousTagSibling(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const;
		xmldomnode	*getNextTagSibling(
					const char *ns,
					const char *name,
					bool ignorecase) const;
		xmldomnode	*getNextTagSibling(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const;
		xmldomnode	*getFirstTagDescendent(
					const char *ns,
					const char *name,
					bool ignorecase) const;
		xmldomnode	*getFirstTagDescendent(
					const char *ns,
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
					const char *ns,
					const char *name,
					bool ignorecase);
		bool		deleteChildren(
					const char *ns,
					const char *name,
					bool ignorecase);
		bool		deleteChildren(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase);
		bool		deleteDescendents(
					const char *ns,
					const char *name,
					bool ignorecase);
		bool		deleteDescendents(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase);
		bool		unwrapFirstChild(
					const char *ns,
					const char *name,
					bool ignorecase);
		bool		unwrapChildren(
					const char *ns,
					const char *name,
					bool ignorecase);
		bool		unwrapDescendents(
					const char *ns,
					const char *name,
					bool ignorecase);
		bool		renameFirstChild(
					const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname,
					bool ignorecase);
		bool		renameChildren(
					const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname,
					bool ignorecase);
		bool		renameDescendents(
					const char *oldns,
					const char *oldname,
					const char *newns,
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
		bool		match(xmldomnode *node,
						const char *ns,
						const char *name,
						bool ignorecase) const;
		bool		match(xmldomnode *node,
						const char *ns,
						const char * const *set) const;

		void	setPrivateData(void *privatedata);
		void	*getPrivateData();

				xmldomnode(const xmldomnode &x);
		xmldomnode	&operator=(const xmldomnode &x);

		xmldomnodeprivate	*pvt;
