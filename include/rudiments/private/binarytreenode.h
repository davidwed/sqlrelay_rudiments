// Copyright (c) 2016 David Muse
// See the COPYING file for more information

		void	setParent(binarytreenode<valuetype> *node);
		void	setLeftChild(binarytreenode<valuetype> *node);
		void	setRightChild(binarytreenode<valuetype> *node);

	protected:
		valuetype			value;
		binarytreenode<valuetype>	*parent;
		binarytreenode<valuetype>	*left;
		binarytreenode<valuetype>	*right;
