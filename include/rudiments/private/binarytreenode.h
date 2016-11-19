// Copyright (c) 2016 David Muse
// See the COPYING file for more information

		binarytreenode<valuetype>	*insert(
					binarytreenode<valuetype> *node);
		void	adjustParentHeights(binarytreenode<valuetype> *node);

		binarytreenode<valuetype>	*balanceUp();
		binarytreenode<valuetype>	*balance();
		binarytreenode<valuetype>	*leftRightRotate();
		binarytreenode<valuetype>	*rightLeftRotate();
		binarytreenode<valuetype>	*leftRotate();
		binarytreenode<valuetype>	*rightRotate();

		void	sort();

		void	setParent(binarytreenode<valuetype> *node);
		void	setLeftChild(binarytreenode<valuetype> *node);
		void	setRightChild(binarytreenode<valuetype> *node);

	protected:
		valuetype			value;
		binarytreenode<valuetype>	*parent;
		binarytreenode<valuetype>	*left;
		binarytreenode<valuetype>	*right;
		uint64_t			leftheight;
		uint64_t			rightheight;

	private:
		void	print(const char *name, uint16_t *indentlevel) const;
