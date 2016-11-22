// Copyright (c) 2016 David Muse
// See the COPYING file for more information

		void	insert(avltreenode<valuetype> *node,
				avltreenode<valuetype> **treetop);
		void	detach(avltreenode<valuetype> **treetop);

		void	adjustParentHeights(avltreenode<valuetype> *node);

		void	balance(avltreenode<valuetype> **treetop);

		avltreenode<valuetype> *leftRightRotate(
					avltreenode<valuetype> **treetop);
		avltreenode<valuetype> *rightLeftRotate(
					avltreenode<valuetype> **treetop);
		avltreenode<valuetype> *leftRotate(
					avltreenode<valuetype> **treetop);
		avltreenode<valuetype> *rightRotate(
					avltreenode<valuetype> **treetop);

		void	sort();

		void	setValue(valuetype value);
		void	setParent(avltreenode<valuetype> *node);
		void	setLeftChild(avltreenode<valuetype> *node);
		void	setRightChild(avltreenode<valuetype> *node);

	protected:
		valuetype		value;
		avltreenode<valuetype>	*parent;
		avltreenode<valuetype>	*left;
		avltreenode<valuetype>	*right;
		uint64_t		leftheight;
		uint64_t		rightheight;

	private:
		void	print(const char *name, uint16_t *indentlevel) const;
