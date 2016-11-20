// Copyright (c) 2016 David Muse
// See the COPYING file for more information

//#define DEBUG_BINARYTREE 1

#include <rudiments/stdio.h>
#include <rudiments/snooze.h>
#include <rudiments/private/rudimentsinlines.h>
#include <rudiments/private/binarytreeutilinlines.h>

#define BINARYTREE_TEMPLATE template <class valuetype>

#define BINARYTREE_CLASS binarytree<valuetype>

#define BINARYTREENODE_TEMPLATE template <class valuetype>

#define BINARYTREENODE_CLASS binarytreenode<valuetype>

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREE_CLASS::binarytree() {
	top=NULL;
	first=NULL;
	last=NULL;
	length=0;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREE_CLASS::~binarytree() {
	clear();
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREE_CLASS::insert(valuetype value) {
	insert(new binarytreenode<valuetype>(value));
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREE_CLASS::insert(binarytreenode<valuetype> *node) {

	// degenerate case
	if (!node) {
		return;
	}

	#ifdef DEBUG_BINARYTREE
	stdoutput.printf("----------------------------------------"
				"---------------------------------------\n");
	#endif

	if (top) {

		// insert the node, optionally replacing the top of the tree
		binarytreenode<valuetype>	*newtop=top->insert(node);
		if (newtop) {
			top=newtop;
		}

		// update first
		for (first=top;
			first->getLeftChild();
			first=first->getLeftChild()) {}

		// update last
		for (last=top;
			last->getRightChild();
			last=last->getRightChild()) {}
	} else {

		// if there was no top node, then this is the
		// first node inserted into the entire tree
		top=node;
		first=node;
		last=node;
	}

	// increment length
	length++;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREE_CLASS::detach(
					binarytreenode<valuetype> *node) {

	// degenerate case
	if (!node) {
		return NULL;
	}

	#ifdef DEBUG_BINARYTREE
	stdoutput.printf("----------------------------------------"
				"---------------------------------------\n");
	#endif

	// update first
	if (first==node) {
		first=node->getNext();
	}

	// update last
	if (last==node) {
		last=node->getPrevious();
	}

	// was this node the top?
	bool	nodewastop=(node==top);

	// detach the node
	binarytreenode<valuetype>	*newtop=node->detach();

	// reset the top if necessary
	if (newtop || nodewastop) {
		top=newtop;
	}

	// decrement length
	length--;

	return node;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool BINARYTREE_CLASS::remove(valuetype value) {
	binarytreenode<valuetype>	*current=find(value);
	return (current)?remove(current):false;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool BINARYTREE_CLASS::removeAll(valuetype value) {
	bool	removed=false;
	while (remove(value)) {
		removed=true;
	}
	return removed;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool BINARYTREE_CLASS::remove(binarytreenode<valuetype> *node) {
	delete detach(node);
	return true;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
uint64_t BINARYTREE_CLASS::getLength() const {
	return length;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREE_CLASS::getTop() {
	return top;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREE_CLASS::getFirst() {
	return first;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREE_CLASS::getLast() {
	return last;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREE_CLASS::getPrevious(
					binarytreenode<valuetype> *node) {
	return (node)?node->getPrevious():NULL;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREE_CLASS::getNext(
					binarytreenode<valuetype> *node) {
	return (node)?node->getNext():NULL;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREE_CLASS::find(valuetype value) {
	return find((binarytreenode<valuetype> *)top,value);
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREE_CLASS::find(
					binarytreenode<valuetype> *startnode,
					valuetype value) {

	#ifdef DEBUG_BINARYTREE
	stdoutput.printf("find ");
	_binarytreeutil_print(value);
	stdoutput.printf(" from ");
	_binarytreeutil_print(startnode->getValue());
	stdoutput.printf(" {\n",length);
	#endif

	// descend the tree until we find the value or run off of the bottom
	binarytreenode<valuetype> *current=startnode;
	while (current) {

		#ifdef DEBUG_BINARYTREE
		stdoutput.printf("  ");
		_binarytreeutil_print(current->getValue());
		stdoutput.printf("\n",length);
		#endif

		int32_t	result=current->compare(value);
		if (result==-1) {
			current=current->getRightChild();
		} else if (result==0) {
			break;
		} else if (result==1) {
			current=current->getLeftChild();
		}
	}

	#ifdef DEBUG_BINARYTREE
	if (current) {
		stdoutput.printf("  success!\n");
	} else {
		stdoutput.printf("  failed\n");
	}
	stdoutput.printf("} find\n\n");
	#endif

	return current;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREE_CLASS::clear() {

	#ifdef DEBUG_BINARYTREE
	uint64_t	i=0;
	stdoutput.printf("clearing %d nodes {\n",length);
	#endif

	// start at the top
	BINARYTREENODE_CLASS	*node=top;
	while (node) {

		// go right one, then go left as far as possible
		if (node->getRightChild()) {
			node=node->getRightChild();
		}
		while (node->getLeftChild()) {
			node=node->getLeftChild();
		}

		// get the parent
		BINARYTREENODE_CLASS	*p=node->getParent();
		if (p) {
			if (p->getLeftChild()==node) {
				p->setLeftChild(NULL);
			} else {
				p->setRightChild(NULL);
			}
		}

		// delete the node
		#ifdef DEBUG_BINARYTREE
		stdoutput.printf("	clearing %lld: ",i);
		_binarytreeutil_print(node->getValue());
		stdoutput.printf("\n");
		i++;
		#endif
		delete node;

		// continue with parent...
		node=p;
	}

	#ifdef DEBUG_BINARYTREE
	stdoutput.printf("} cleared %d nodes\n\n",i);
	#endif

	top=NULL;
	first=NULL;
	last=NULL;
	length=0;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREE_CLASS::print() const {
	if (top) {
		top->print();
	}
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREENODE_CLASS::binarytreenode(valuetype value) {
	this->value=value;
	parent=NULL;
	left=NULL;
	right=NULL;
	leftheight=0;
	rightheight=0;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREENODE_CLASS::~binarytreenode() {
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREENODE_CLASS::setValue(valuetype value) {
	this->value=value;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
valuetype BINARYTREENODE_CLASS::getValue() const {
	return value;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREENODE_CLASS *BINARYTREENODE_CLASS::getParent() {
	return parent;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREENODE_CLASS *BINARYTREENODE_CLASS::getLeftChild() {
	return left;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREENODE_CLASS *BINARYTREENODE_CLASS::getRightChild() {
	return right;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
uint64_t BINARYTREENODE_CLASS::getLeftHeight() {
	return leftheight;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
uint64_t BINARYTREENODE_CLASS::getRightHeight() {
	return rightheight;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREENODE_CLASS *BINARYTREENODE_CLASS::getPrevious() {
	if (left) {
		BINARYTREENODE_CLASS	*node=left;
		while (node->right) {
			node=node->right;
		}
		return node;
	} else if (parent) {
		if (parent->right==this) {
			return parent;
		} else {
			BINARYTREENODE_CLASS	*node=parent;
			while (node) {
				if (!node->parent) {
					break;
				}
				if (node->parent->right==node) {
					return node->parent;
				}
				node=node->parent;
			}
		}
	}
	return NULL;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
BINARYTREENODE_CLASS *BINARYTREENODE_CLASS::getNext() {
	if (right) {
		BINARYTREENODE_CLASS	*node=right;
		while (node->left) {
			node=node->left;
		}
		return node;
	} else if (parent) {
		if (parent->left==this) {
			return parent;
		} else {
			BINARYTREENODE_CLASS	*node=parent;
			while (node) {
				if (!node->parent) {
					break;
				}
				if (node->parent->left==node) {
					return node->parent;
				}
				node=node->parent;
			}
		}
	}
	return NULL;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
int32_t BINARYTREENODE_CLASS::compare(valuetype value) const {
	return _binarytreeutil_compare(this->value,value);
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
int32_t BINARYTREENODE_CLASS::compare(binarytreenode<valuetype> *peer) const {
	return _binarytreeutil_compare(this->value,peer->value);
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREENODE_CLASS::print() const {
	uint16_t	indentlevel=0;
	print("top",&indentlevel);
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREENODE_CLASS::print(const char *name,
				uint16_t *indentlevel) const {
	for (uint16_t i=0; i<*indentlevel; i++) {
		stdoutput.printf(" ");
	}
	stdoutput.printf("<%s value=\"",name);
	_binarytreeutil_print(value);
	stdoutput.printf("\" lh=\"%lld\" rh=\"%lld\" bf=\"%lld\"",
			leftheight,rightheight,leftheight-rightheight);
	if (!left && !right) {
		stdoutput.printf("/>\n");
	} else {
		stdoutput.printf(">\n");
		(*indentlevel)++;
		if (left) {
			left->print("left ",indentlevel);
		}
		if (right) {
			right->print("right",indentlevel);
		}
		(*indentlevel)--;
		for (uint16_t i=0; i<*indentlevel; i++) {
			stdoutput.printf(" ");
		}
		stdoutput.printf("</%s>\n",name);
	}
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREENODE_CLASS::setParent(BINARYTREENODE_CLASS *node) {
	parent=node;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREENODE_CLASS::setLeftChild(BINARYTREENODE_CLASS *node) {
	left=node;
}

BINARYTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREENODE_CLASS::setRightChild(BINARYTREENODE_CLASS *node) {
	right=node;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREENODE_CLASS::insert(
					binarytreenode<valuetype> *node) {

	// degenerate case
	if (!node) {
		return NULL;
	}

	// find a location to insert the node (should always be a leaf node)
	binarytreenode<valuetype>	*location=this;
	for (;;) {

		if (node->value<=location->value) {

			if (location->left) {
				location=location->left;
			} else {

				#ifdef DEBUG_BINARYTREE
				stdoutput.printf("insert ");
				_binarytreeutil_print(node->value);
				stdoutput.printf(" to left of ");
				_binarytreeutil_print(location->value);
				stdoutput.printf(" {\n\n");
				#endif

				location->setLeftChild(node);
				break;
			}

		} else if (node->value>location->value) {

			if (location->right) {
				location=location->right;
			} else {

				#ifdef DEBUG_BINARYTREE
				stdoutput.printf("insert ");
				_binarytreeutil_print(node->value);
				stdoutput.printf(" to right of ");
				_binarytreeutil_print(location->value);
				stdoutput.printf(" {\n\n");
				#endif

				location->setRightChild(node);
				break;
			}
		}
	}

	node->setParent(location);

	// update heights up the tree
	adjustParentHeights(node);

	#ifdef DEBUG_BINARYTREE
	binarytreenode<valuetype>	*top=this;
	while (top->parent) { top=top->parent; }
	top->print(); stdoutput.printf("\n\n");
	#endif

	// balance the tree
	binarytreenode<valuetype>	*result=node->balanceUp();

	#ifdef DEBUG_BINARYTREE
	stdoutput.printf("} insert\n\n");
	#endif

	return result;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREENODE_CLASS::detach() {

	#ifdef DEBUG_BINARYTREE
	stdoutput.printf("detach ");
	_binarytreeutil_print(value);
	stdoutput.printf(" {\n\n");

	binarytreenode<valuetype>	*top=this;
	while (top->parent) { top=top->parent; }
	top->print(); stdoutput.printf("\n");
	#endif

	binarytreenode<valuetype>	*newtreetop=NULL;

	if ((left && !right) || (!left && right) || (!left && !right)) {

		// node with one or zero children...

		#ifdef DEBUG_BINARYTREE
		stdoutput.printf("simple case: 1 or 0 children\n\n");
		#endif

		// decide which child the node has
		// NOTE: If the node has no children then this will implicitly
		// set child=NULL (which is what we want in that case) because
		// right=NULL.
		binarytreenode<valuetype>	*child=(left)?left:right;

		if (parent) {

			// disconnect this node from its children
			left=NULL;
			right=NULL;

			// connect the parent to the child
			// (or to NULL if the node has no children)
			// decrement the appropriate height of parent
			if (parent->left==this) {
				parent->left=child;
				parent->leftheight--;
			} else {
				parent->right=child;
				parent->rightheight--;
			}

			// connect the child to the parent
			if (child) {
				child->parent=parent;
			}

			// update heights up the tree
			adjustParentHeights(parent);

			#ifdef DEBUG_BINARYTREE
			binarytreenode<valuetype>	*top=this;
			while (top->parent) { top=top->parent; }
			top->print(); stdoutput.printf("\n");
			#endif

			// disconnect this node from its parent
			// (but keep track of the parent so we
			// can use it to balance up)
			binarytreenode<valuetype>	*p=parent;
			parent=NULL;

			// balance the tree
			newtreetop=p->balanceUp();

		} else {

			// disconnect this node's child from it
			if (child) {
				child->parent=NULL;
			}

			// disconnect this node from its children
			left=NULL;
			right=NULL;

			// NOTE: If the node has no children, then this will
			// implicitly (re)set newtreetop=NULL, which is what
			// we want in that case.
			newtreetop=child;

			#ifdef DEBUG_BINARYTREE
			binarytreenode<valuetype>	*top=this;
			while (top->parent) { top=top->parent; }
			top->print(); stdoutput.printf("\n");
			#endif
		}

		#ifdef DEBUG_BINARYTREE
		stdoutput.printf("} detach\n\n");
		#endif

		return newtreetop;

	} else {

		// node with left and right children...

		#ifdef DEBUG_BINARYTREE
		stdoutput.printf("less simple case: 2 children\n\n");
		#endif

		// get this node's successor...
		//
		// (eg. if the tree contains values 5, 7, 10, 12, 15, and 18,
		// and this node is 10, then find the node with 12 in it)
		//
		// go right one, then go left as far as possible
		binarytreenode<valuetype>	*successor=right;
		while (successor->left) {
			successor=successor->left;
		}

		#ifdef DEBUG_BINARYTREE
		stdoutput.printf("swap ");
		_binarytreeutil_print(value);
		stdoutput.printf(" and ");
		_binarytreeutil_print(successor->value);
		stdoutput.printf("\n\n");
		#endif


		// if the successor was the immediate right child of this node
		// then we need to handle a few things differently later
		bool	successorwasimmediaterightchild=(right==successor);


		// swap this node with the successor...

		// get a copy of the successor
		binarytreenode<valuetype>	temp(*successor);

		// re-parent the successor
		successor->parent=parent;
		if (successor->parent) {
			if (successor->parent->left==this) {
				successor->parent->left=successor;
			} else {
				successor->parent->right=successor;
			}
		} else {
			newtreetop=successor;
		}

		// replace the successor's children
		// with those of this node
		successor->left=left;
		if (successor->left) {
			successor->left->parent=successor;
		}
		if (successorwasimmediaterightchild) {
			successor->right=this;
			successor->right->parent=successor;
		} else {
			successor->right=right;
			if (successor->right) {
				successor->right->parent=successor;
			}

			// re-parent this node
			parent=temp.parent;
			if (parent->left==successor) {
				parent->left=this;
			} else {
				parent->right=this;
			}
		}

		// replace the successor's heights
		// with those of this node
		successor->leftheight=leftheight;
		successor->rightheight=rightheight;


		// replace this node's children
		// with those of the successor
		left=temp.left;
		if (left) {
			left->parent=this;
		}
		right=temp.right;
		if (right) {
			right->parent=this;
		}

		// replace this node's heights
		// with those of the successor
		leftheight=temp.leftheight;
		rightheight=temp.rightheight;

		#ifdef DEBUG_BINARYTREE
		binarytreenode<valuetype>	*top=this;
		while (top->parent) { top=top->parent; }
		top->print(); stdoutput.printf("\n");
		#endif

		// Call detach on this node again.
		// This time, the node should have 1 or 0 children.
		binarytreenode<valuetype>	*result=detach();
		if (result) {
			newtreetop=result;
		}

		#ifdef DEBUG_BINARYTREE
		stdoutput.printf("} detach\n\n");
		#endif

		return newtreetop;
	}
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREENODE_CLASS::adjustParentHeights(
					binarytreenode<valuetype> *node) {
	while (node->parent) {

		uint64_t	height=
				((node->leftheight>node->rightheight)?
							node->leftheight:
							node->rightheight)+1;

		if (node->parent->left==node) {
			if (node->parent->leftheight==height) {
				break;
			}
			node->parent->leftheight=height;
		} else {
			if (node->parent->rightheight==height) {
				break;
			}
			node->parent->rightheight=height;
		}

		node=node->parent;
	}
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREENODE_CLASS::balanceUp() {

	#ifdef DEBUG_BINARYTREE
	stdoutput.printf("balanceUp at ");
	_binarytreeutil_print(value);
	stdoutput.printf(" {\n\n");
	#endif

	binarytreenode<valuetype>	*newtreetop=NULL;
	for (binarytreenode<valuetype> *node=this; node; node=node->parent) {
		binarytreenode<valuetype>	*result=node->balance();
		if (result) {
			newtreetop=result;
		}
	}

	#ifdef DEBUG_BINARYTREE
	stdoutput.printf("} balanceUp\n\n",value);
	#endif

	return newtreetop;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREENODE_CLASS::balance() {

	// AVL balance...

	// there's an imbalance if the left and right
	// tree heights differ by more than 1
	if ((leftheight>rightheight && leftheight-rightheight>1) ||
		(rightheight>leftheight && rightheight-leftheight>1)) {

		#ifdef DEBUG_BINARYTREE
		stdoutput.printf("imbalance at ");
		_binarytreeutil_print(value);
		stdoutput.printf("\n\n");
		#endif

		// apply the appropriate rotation to restore balance
		if (rightheight>leftheight) {
			if (right->rightheight>right->leftheight) {
				return leftRotate();
			} else {
				return rightLeftRotate();
			}
		} else if (leftheight>rightheight) {
			if (left->leftheight>left->rightheight) {
				return rightRotate();
			} else {
				return leftRightRotate();
			}
		}

		// It's not impossible that we encountered and imbalance that
		// we can't fix at this node.  It should be fixable at an
		// ancestor.
		#ifdef DEBUG_BINARYTREE
		stdoutput.printf("no rotation\n\n");
		#endif
	}
	return NULL;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREENODE_CLASS::leftRotate() {

	/* one of these: (eg: insert order a,b,c)
 	 *
 	 *  \
 	 *   a
 	 *  / \            \
 	 *     b    ->      b
 	 *    / \         /   \
 	 *   *   c       a     c
 	 *      / \     / \   / \
 	 *                 *
 	 * needs left rotation */

	#ifdef DEBUG_BINARYTREE
	stdoutput.printf("left rotation at ");
	_binarytreeutil_print(value);
	stdoutput.printf(" {\n\n");
	#endif

	// we may need to "re-top" the whole tree
	binarytreenode<valuetype>	*newtreetop=NULL;

	// get a, b, and "star"
	binarytreenode<valuetype>	*a=this;
	binarytreenode<valuetype>	*b=a->right;
	binarytreenode<valuetype>	*star=b->left;
	uint64_t			starheight=b->leftheight;

	// move b
	binarytreenode<valuetype>	*p=a->parent;
	if (p) {
		if (p->right==a) {
			p->right=b;
		} else {
			p->left=b;
		}
	} else {
		#ifdef DEBUG_BINARYTREE
		stdoutput.printf("(new tree top)\n\n");
		#endif
		newtreetop=b;
	}
	b->parent=a->parent;
	b->left=a;

	// move a
	a->parent=b;
	a->right=star;
	a->rightheight=starheight;

	// move "star"
	if (star) {
		star->parent=a;
	}

	// update heights up the tree
	adjustParentHeights(a);

	#ifdef DEBUG_BINARYTREE
	binarytreenode<valuetype>	*top=this;
	while (top->parent) { top=top->parent; }
	top->print(); stdoutput.printf("\n\n");
	#endif

	// Since a was moved into a location in the tree that may not have
	// prevoiusly existed, it may have unbalanced the tree.  Re-balance,
	// starting with a.
	binarytreenode<valuetype>	*result=a->balanceUp();
	if (result) {
		newtreetop=result;
	}

	#ifdef DEBUG_BINARYTREE
	stdoutput.printf("} left\n\n");
	#endif

	return newtreetop;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREENODE_CLASS::rightLeftRotate() {

	/* one of these: (eg: insert order a,c,b)
	 *
	 *  \         \
	 *   a         a
	 *  / \       / \          \
	 *     c  ->     b    ->    b
	 *    / \       / \        / \
	 *   b             c      a   c
	 *  / \           / \    / \ / \
	 *     *         *
	 *
	 * needs right-left rotation */

	#ifdef DEBUG_BINARYTREE
	stdoutput.printf("right-left rotation at ");
	_binarytreeutil_print(value);
	stdoutput.printf(" {\n\n");
	stdoutput.printf("right part {\n\n");
	#endif

	// we may need to "re-top" the whole tree
	binarytreenode<valuetype>	*newtreetop=NULL;

	// do the right part of the right-left rotation...

	// get a, c, b, and "star"
	binarytreenode<valuetype>	*a=this;
	binarytreenode<valuetype>	*c=a->right;
	binarytreenode<valuetype>	*b=c->left;
	binarytreenode<valuetype>	*star=b->right;
	uint64_t			starheight=b->rightheight;

	// move b
	a->right=b;
	b->parent=a;
	b->right=c;

	// move c
	c->parent=b;
	c->left=star;
	c->leftheight=starheight;

	// move "star"
	if (star) {
		star->parent=c;
	}

	// update heights up the tree
	adjustParentHeights(c);

	#ifdef DEBUG_BINARYTREE
	binarytreenode<valuetype>	*top=this;
	while (top->parent) { top=top->parent; }
	top->print(); stdoutput.printf("\n} right part\n\n");
	#endif

	// do the left part of the right-left rotation
	newtreetop=leftRotate();

	// Since c was moved into a location in the tree that may not have
	// prevoiusly existed, it may have unbalanced the tree.  Re-balance,
	// starting with c.
	// FIXME: this may not be necessary...
	binarytreenode<valuetype> *result=c->balanceUp();
	if (result) {
		newtreetop=result;
	}

	#ifdef DEBUG_BINARYTREE
	stdoutput.printf("} right-left\n\n");
	#endif

	return newtreetop;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREENODE_CLASS::rightRotate() {

	/* one of these: (insert order c,b,a)
	 *
	 *     \
	 *      c
	 *     / \       \
	 *    b    ->     b
	 *   / \        /   \
	 *  a   *      a     c 
	 * / \        / \   / \
	 *                 *
	 * needs right rotation */

	#ifdef DEBUG_BINARYTREE
	stdoutput.printf("right rotation at ");
	_binarytreeutil_print(value);
	stdoutput.printf(" {\n\n");
	#endif

	// we may need to "re-top" the whole tree
	binarytreenode<valuetype>	*newtreetop=NULL;

	// get c, b, and "star"
	binarytreenode<valuetype>	*c=this;
	binarytreenode<valuetype>	*b=c->left;
	binarytreenode<valuetype>	*star=b->right;
	uint64_t			starheight=b->rightheight;

	// move b
	binarytreenode<valuetype>	*p=c->parent;
	if (p) {
		if (p->right==c) {
			p->right=b;
		} else {
			p->left=b;
		}
	} else {
		#ifdef DEBUG_BINARYTREE
		stdoutput.printf("(new tree top)\n\n");
		#endif
		newtreetop=b;
	}
	b->parent=c->parent;
	b->right=c;

	// move c
	c->parent=b;
	c->left=star;
	c->leftheight=starheight;

	// move "star"
	if (star) {
		star->parent=c;
	}

	// update heights up the tree
	adjustParentHeights(c);

	#ifdef DEBUG_BINARYTREE
	binarytreenode<valuetype>	*top=this;
	while (top->parent) { top=top->parent; }
	top->print(); stdoutput.printf("\n\n");
	#endif

	// Since c was moved into a location in the tree that may not have
	// prevoiusly existed, it may have unbalanced the tree.  Re-balance,
	// starting with c.
	binarytreenode<valuetype>	*result=c->balanceUp();
	if (result) {
		newtreetop=result;
	}

	#ifdef DEBUG_BINARYTREE
	stdoutput.printf("} right\n\n");
	#endif

	return newtreetop;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREENODE_CLASS::leftRightRotate() {

	/* one of these: (insert order c,a,b)
	 *
	 *   \           \
	 *    c           c
	 *   / \         / \      \
	 *  a     ->    b    ->    b
	 * / \         / \        / \
	 *    b       a          a   c
	 *   / \     / \        /     \
	 *  *           *
	 *
	 * needs left-right rotation */

	#ifdef DEBUG_BINARYTREE
	stdoutput.printf("left-right rotation at ");
	_binarytreeutil_print(value);
	stdoutput.printf(" {\n\n");
	stdoutput.printf("left part {\n\n");
	#endif

	// we may need to "re-top" the whole tree
	binarytreenode<valuetype>	*newtreetop=NULL;

	// do the left part of the left-right rotation...

	// get c, a, b, and "star"
	binarytreenode<valuetype>	*c=this;
	binarytreenode<valuetype>	*a=c->left;
	binarytreenode<valuetype>	*b=a->right;
	binarytreenode<valuetype>	*star=b->left;
	uint64_t			starheight=b->leftheight;

	// move b
	c->left=b;
	b->parent=c;
	b->left=a;

	// move a
	a->parent=b;
	a->right=star;
	a->rightheight=starheight;

	// move "star"
	if (star) {
		star->parent=a;
	}

	// update heights up the tree
	adjustParentHeights(a);

	#ifdef DEBUG_BINARYTREE
	binarytreenode<valuetype>	*top=this;
	while (top->parent) { top=top->parent; }
	top->print(); stdoutput.printf("\n} left part\n\n");
	#endif

	// do the right part of the left-right rotation
	newtreetop=rightRotate();

	// Since a was moved into a location in the tree that may not have
	// prevoiusly existed, it may have unbalanced the tree.  Re-balance,
	// starting with a.
	// FIXME: this may not be necessary...
	binarytreenode<valuetype> *result=a->balanceUp();
	if (result) {
		newtreetop=result;
	}

	#ifdef DEBUG_BINARYTREE
	stdoutput.printf("} left-right\n\n");
	#endif

	return newtreetop;
}
