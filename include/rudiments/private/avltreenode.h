// Copyright (c) 2016 David Muse
// See the COPYING file for more information

		avltreenode<valuetype>	*insert(avltreenode<valuetype> *node);
		avltreenode<valuetype>	*detach();

		void	adjustParentHeights(avltreenode<valuetype> *node);

		avltreenode<valuetype>	*balanceUp();
		avltreenode<valuetype>	*balance();

		avltreenode<valuetype>	*leftRightRotate();
		avltreenode<valuetype>	*rightLeftRotate();
		avltreenode<valuetype>	*leftRotate();
		avltreenode<valuetype>	*rightRotate();

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
