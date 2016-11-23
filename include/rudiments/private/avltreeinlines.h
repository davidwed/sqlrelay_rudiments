// Copyright (c) 2016 David Muse
// See the COPYING file for more information

//#define DEBUG_AVLTREE 1

#include <rudiments/stdio.h>
#include <rudiments/private/rudimentsinlines.h>
#include <rudiments/private/containerutilinlines.h>

#define AVLTREE_TEMPLATE template <class valuetype>

#define AVLTREE_CLASS avltree<valuetype>

#define AVLTREENODE_TEMPLATE template <class valuetype>

#define AVLTREENODE_CLASS avltreenode<valuetype>

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
AVLTREE_CLASS::avltree() {
	top=NULL;
	first=NULL;
	last=NULL;
	length=0;
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
AVLTREE_CLASS::~avltree() {
	clear();
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void AVLTREE_CLASS::insert(valuetype value) {
	insert(new avltreenode<valuetype>(value));
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void AVLTREE_CLASS::insert(avltreenode<valuetype> *node) {

	// degenerate case
	if (!node) {
		return;
	}

	#ifdef DEBUG_AVLTREE
	stdoutput.printf("----------------------------------------"
				"---------------------------------------\n");
	#endif

	if (top) {

		// insert the node, optionally replacing the top of the tree
		top->insert(node,&top);

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

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
avltreenode<valuetype> *AVLTREE_CLASS::detach(avltreenode<valuetype> *node) {

	// degenerate case
	if (!node) {
		return NULL;
	}

	#ifdef DEBUG_AVLTREE
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

	// detach the node
	node->detach(&top);

	// decrement length
	length--;

	return node;
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool AVLTREE_CLASS::remove(valuetype value) {
	avltreenode<valuetype>	*current=find(value);
	return (current)?remove(current):false;
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool AVLTREE_CLASS::removeAll(valuetype value) {
	bool	removed=false;
	while (remove(value)) {
		removed=true;
	}
	return removed;
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool AVLTREE_CLASS::remove(avltreenode<valuetype> *node) {
	delete detach(node);
	return true;
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
uint64_t AVLTREE_CLASS::getLength() const {
	return length;
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
avltreenode<valuetype> *AVLTREE_CLASS::getTop() {
	return top;
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
avltreenode<valuetype> *AVLTREE_CLASS::getFirst() {
	return first;
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
avltreenode<valuetype> *AVLTREE_CLASS::getLast() {
	return last;
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
avltreenode<valuetype> *AVLTREE_CLASS::getPrevious(
					avltreenode<valuetype> *node) {
	return (node)?node->getPrevious():NULL;
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
avltreenode<valuetype> *AVLTREE_CLASS::getNext(
					avltreenode<valuetype> *node) {
	return (node)?node->getNext():NULL;
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
avltreenode<valuetype> *AVLTREE_CLASS::find(valuetype value) {
	return find((avltreenode<valuetype> *)top,value);
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
avltreenode<valuetype> *AVLTREE_CLASS::find(
					avltreenode<valuetype> *startnode,
					valuetype value) {

	#ifdef DEBUG_AVLTREE
	stdoutput.printf("find ");
	_containerutil_print(value);
	stdoutput.printf(" from ");
	if (startnode) {
		_containerutil_print(startnode->getValue());
	} else {
		_containerutil_print("(null)");
	}
	stdoutput.printf(" {\n");
	#endif

	// descend the tree until we find the value or run off of the bottom
	avltreenode<valuetype> *current=startnode;
	while (current) {

		int32_t	result=current->compare(value);

		#ifdef DEBUG_AVLTREE
		stdoutput.printf("  ");
		_containerutil_print(current->getValue());
		stdoutput.printf(" - %d\n",result);
		#endif

		if (result<0) {
			current=current->getRightChild();
		} else if (result==0) {
			break;
		} else if (result>0) {
			current=current->getLeftChild();
		}
	}

	#ifdef DEBUG_AVLTREE
	if (current) {
		stdoutput.printf("  success!\n");
	} else {
		stdoutput.printf("  failed\n");
	}
	stdoutput.printf("} find\n\n");
	#endif

	return current;
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void AVLTREE_CLASS::clear() {

	#ifdef DEBUG_AVLTREE
	uint64_t	i=0;
	stdoutput.printf("clearing %d nodes {\n",length);
	#endif

	// start at the top
	AVLTREENODE_CLASS	*node=top;
	while (node) {

		// go right one, then go left as far as possible
		if (node->getRightChild()) {
			node=node->getRightChild();
		}
		while (node->getLeftChild()) {
			node=node->getLeftChild();
		}

		// get the parent
		AVLTREENODE_CLASS	*p=node->getParent();
		if (p) {
			if (p->getLeftChild()==node) {
				p->setLeftChild(NULL);
			} else {
				p->setRightChild(NULL);
			}
		}

		// delete the node
		#ifdef DEBUG_AVLTREE
		stdoutput.printf("	clearing %lld\n",i);
		// It's dangerous to try to print the value of the node here.
		// If the value is a pointer to something, it may have been
		// deleted already.  In fact, it's really common to run through
		// the tree, deleting values, before finally calling clear()
		// on the tree itself.
		i++;
		#endif
		delete node;

		// continue with parent...
		node=p;
	}

	#ifdef DEBUG_AVLTREE
	stdoutput.printf("} cleared %d nodes\n\n",i);
	#endif

	// clear pointers and length
	top=NULL;
	first=NULL;
	last=NULL;
	length=0;
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void AVLTREE_CLASS::print() const {
	if (top) {
		top->print();
	}
}

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
AVLTREENODE_CLASS::avltreenode(valuetype value) {
	this->value=value;
	parent=NULL;
	left=NULL;
	right=NULL;
	leftheight=0;
	rightheight=0;
}

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
AVLTREENODE_CLASS::~avltreenode() {
}

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void AVLTREENODE_CLASS::setValue(valuetype value) {
	this->value=value;
}

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
valuetype AVLTREENODE_CLASS::getValue() const {
	return value;
}

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
AVLTREENODE_CLASS *AVLTREENODE_CLASS::getParent() {
	return parent;
}

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
AVLTREENODE_CLASS *AVLTREENODE_CLASS::getLeftChild() {
	return left;
}

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
AVLTREENODE_CLASS *AVLTREENODE_CLASS::getRightChild() {
	return right;
}

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
uint64_t AVLTREENODE_CLASS::getLeftHeight() {
	return leftheight;
}

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
uint64_t AVLTREENODE_CLASS::getRightHeight() {
	return rightheight;
}

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
AVLTREENODE_CLASS *AVLTREENODE_CLASS::getPrevious() {

	// reverse in-order, depth-first traversal...

	if (left) {

		// if we have a left child then its rightmost descendent
		// contains the next lowest value...

		// go left
		AVLTREENODE_CLASS	*node=left;

		// go as far right as possible
		while (node->right) {
			node=node->right;
		}
		return node;

	} else if (parent) {

		// if we're the right child of our parent,
		// then our parent contains the next lowest value
		if (parent->right==this) {
			return parent;
		}

		// If we're the left child of our parent, then we have to
		// move up until we find an acestor that's the right child of
		// its parent.  That node contains the next lowest value.
		AVLTREENODE_CLASS	*node=parent;
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
	return NULL;
}

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
AVLTREENODE_CLASS *AVLTREENODE_CLASS::getNext() {

	// in-order, depth-first traversal...

	if (right) {

		// if we have a right child then its leftmost descendent
		// contains the next highest value...

		// go right
		AVLTREENODE_CLASS	*node=right;

		// go as far left as possible
		while (node->left) {
			node=node->left;
		}
		return node;

	} else if (parent) {

		// if we're the left child of our parent,
		// then our parent contains the next highest value
		if (parent->left==this) {
			return parent;
		}

		// If we're the right child of our parent, then we have to
		// move up until we find an acestor that's the left child of
		// its parent.  That node contains the next highest value.
		AVLTREENODE_CLASS	*node=parent;
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
	return NULL;
}

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
int32_t AVLTREENODE_CLASS::compare(valuetype value) const {
	return _containerutil_compare(this->value,value);
}

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
int32_t AVLTREENODE_CLASS::compare(avltreenode<valuetype> *peer) const {
	return _containerutil_compare(this->value,peer->value);
}

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void AVLTREENODE_CLASS::print() const {
	uint16_t	indentlevel=0;
	print("top",&indentlevel);
}

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void AVLTREENODE_CLASS::print(const char *name, uint16_t *indentlevel) const {
	// print an xml-style representation of the node and its descendents
	for (uint16_t i=0; i<*indentlevel; i++) {
		stdoutput.printf(" ");
	}
	stdoutput.printf("<%s value=\"",name);
	_containerutil_print(value);
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

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void AVLTREENODE_CLASS::setParent(AVLTREENODE_CLASS *node) {
	parent=node;
}

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void AVLTREENODE_CLASS::setLeftChild(AVLTREENODE_CLASS *node) {
	left=node;
}

AVLTREENODE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void AVLTREENODE_CLASS::setRightChild(AVLTREENODE_CLASS *node) {
	right=node;
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void AVLTREENODE_CLASS::insert(avltreenode<valuetype> *node,
				avltreenode<valuetype> **treetop) {

	// degenerate case
	if (!node) {
		return;
	}

	// find a location to insert the node (should always be a leaf node)
	avltreenode<valuetype>	*location=this;
	for (;;) {

		if (node->compare(location->value)<=0) {

			if (location->left) {
				location=location->left;
			} else {

				#ifdef DEBUG_AVLTREE
				stdoutput.printf("insert ");
				_containerutil_print(node->value);
				stdoutput.printf(" to left of ");
				_containerutil_print(location->value);
				stdoutput.printf(" {\n\n");
				#endif

				location->setLeftChild(node);
				break;
			}

		} else if (node->compare(location->value)>0) {

			if (location->right) {
				location=location->right;
			} else {

				#ifdef DEBUG_AVLTREE
				stdoutput.printf("insert ");
				_containerutil_print(node->value);
				stdoutput.printf(" to right of ");
				_containerutil_print(location->value);
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

	// balance the tree
	node->parent->balance(treetop);

	#ifdef DEBUG_AVLTREE
	stdoutput.printf("} insert\n\n");
	#endif
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void AVLTREENODE_CLASS::detach(avltreenode<valuetype> **treetop) {

	#ifdef DEBUG_AVLTREE
	stdoutput.printf("detach ");
	_containerutil_print(value);
	stdoutput.printf(" {\n\n");

	avltreenode<valuetype>	*top=this;
	while (top->parent) { top=top->parent; }
	top->print(); stdoutput.printf("\n");
	#endif

	if (left && right) {

		// node with left and right children...

		#ifdef DEBUG_AVLTREE
		stdoutput.printf("less simple case: 2 children\n\n");
		#endif

		// get this node's successor...
		//
		// (eg. if the tree contains values 5, 7, 10, 12, 15, and 18,
		// and this node is 10, then find the node with 12 in it)
		//
		// following the rules from our in-order, depth-first traversal
		// above, since we have a right child, we must go right one,
		// then go left as far as possible
		avltreenode<valuetype>	*successor=right;
		while (successor->left) {
			successor=successor->left;
		}

		#ifdef DEBUG_AVLTREE
		stdoutput.printf("swap ");
		_containerutil_print(value);
		stdoutput.printf(" and ");
		_containerutil_print(successor->value);
		stdoutput.printf("\n\n");
		#endif


		// if the successor was the immediate right child of this node
		// then we need to handle a few things differently later
		bool	successorwasimmediaterightchild=(right==successor);


		// swap this node with the successor...

		// get a copy of the successor
		avltreenode<valuetype>	temp(*successor);

		// re-parent the successor
		successor->parent=parent;
		if (successor->parent) {
			if (successor->parent->left==this) {
				successor->parent->left=successor;
			} else {
				successor->parent->right=successor;
			}
		} else {
			*treetop=successor;
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

		#ifdef DEBUG_AVLTREE
		avltreenode<valuetype>	*top=this;
		while (top->parent) { top=top->parent; }
		top->print(); stdoutput.printf("\n");
		#endif

		// fall through to the code below because now
		// the node should have one or zero children...
	}

	// node with one or zero children...

	#ifdef DEBUG_AVLTREE
	stdoutput.printf("simple case: 1 or 0 children\n\n");
	#endif

	// decide which child the node has
	// NOTE: If the node has no children then this will implicitly
	// set child=NULL (which is what we want in that case) because
	// right=NULL.
	avltreenode<valuetype>	*child=(left)?left:right;

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

		// disconnect this node from its parent
		// (but keep track of the parent so we
		// can use it to balance)
		avltreenode<valuetype>	*p=parent;
		parent=NULL;

		// update heights up the tree
		adjustParentHeights(p);

		// balance the tree
		p->balance(treetop);

	} else {

		// disconnect this node's child from it
		if (child) {
			child->parent=NULL;
		}

		// disconnect this node from its children
		left=NULL;
		right=NULL;

		// NOTE: If the node has no children, then this will
		// implicitly (re)set treetop=NULL, which is what
		// we want in that case.
		*treetop=child;

		#ifdef DEBUG_AVLTREE
		avltreenode<valuetype>	*top=this;
		while (top->parent) { top=top->parent; }
		top->print(); stdoutput.printf("\n");
		#endif
	}

	#ifdef DEBUG_AVLTREE
	stdoutput.printf("} detach\n\n");
	#endif
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void AVLTREENODE_CLASS::adjustParentHeights(avltreenode<valuetype> *node) {

	// move up the tree, starting with the parent of "node"...
	while (node->parent) {

		// calculate the new height of the parent
		uint64_t	height=
				((node->leftheight>node->rightheight)?
							node->leftheight:
							node->rightheight)+1;

		// If "node" is the left child of the parent, then adjust the
		// parent's left height.  
		// If "node" is the right child of the parent, then adjust the
		// parent's right height.  
		// In either case, bail if the height is already the same as we
		// calculated.
		if (node->parent->left==node) {
			if (node->parent->leftheight==height) {
				return;
			}
			node->parent->leftheight=height;
		} else {
			if (node->parent->rightheight==height) {
				return;
			}
			node->parent->rightheight=height;
		}

		// move up
		node=node->parent;
	}
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void AVLTREENODE_CLASS::balance(avltreenode<valuetype> **treetop) {

	// AVL balance...

	#ifdef DEBUG_AVLTREE
	stdoutput.printf("balance at ");
	_containerutil_print(value);
	stdoutput.printf(" {\n\n");

	avltreenode<valuetype>	*top=this;
	while (top->parent) { top=top->parent; }
	top->print(); stdoutput.printf("\n");
	#endif

	// start balancing with the current node
	avltreenode<valuetype> *node=this;
	while (node) {

		// there's an imbalance if the left and right
		// tree heights differ by more than 1
		if ((node->leftheight>node->rightheight &&
				node->leftheight-node->rightheight>1) ||
			(node->rightheight>node->leftheight &&
				node->rightheight-node->leftheight>1)) {

			#ifdef DEBUG_AVLTREE
			stdoutput.printf("imbalance at ");
			_containerutil_print(node->value);
			stdoutput.printf("\n\n");
			#endif

			// apply the appropriate rotation to restore balance
			// and let the rotation method tell us whch node to
			// process next
			if (node->rightheight>node->leftheight) {
				if (node->right->rightheight>
						node->right->leftheight) {
					node=node->leftRotate(treetop);
				} else {
					node=node->rightLeftRotate(treetop);
				}
			} else if (node->leftheight>node->rightheight) {
				if (node->left->leftheight>
						node->left->rightheight) {
					node=node->rightRotate(treetop);
				} else {
					node=node->leftRightRotate(treetop);
				}
			}

			#ifdef DEBUG_AVLTREE
			avltreenode<valuetype>	*top=this;
			while (top->parent) { top=top->parent; }
			top->print(); stdoutput.printf("\n");
			#endif

		} else {

			#ifdef DEBUG_AVLTREE
			stdoutput.printf("no imbalance at ");
			_containerutil_print(node->value);
			stdoutput.printf("\n\n");
			#endif

			// if there's no imbalance then the next node we need
			// to process is the parent of the current node
			node=node->parent;
		}

		#ifdef DEBUG_AVLTREE
		if (node) {
			stdoutput.printf("continuing at ");
			_containerutil_print(node->value);
			stdoutput.printf("\n\n");
		}
		#endif
	}


	#ifdef DEBUG_AVLTREE
	stdoutput.printf("} balance\n\n");
	#endif
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
avltreenode<valuetype> *AVLTREENODE_CLASS::leftRotate(
					avltreenode<valuetype> **treetop) {

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

	#ifdef DEBUG_AVLTREE
	stdoutput.printf("left rotation at ");
	_containerutil_print(value);
	stdoutput.printf("\n\n");
	#endif

	// get a, b, and "star"
	avltreenode<valuetype>	*a=this;
	avltreenode<valuetype>	*b=a->right;
	avltreenode<valuetype>	*star=b->left;
	uint64_t		starheight=b->leftheight;

	// move b
	avltreenode<valuetype>	*p=a->parent;
	if (p) {
		if (p->right==a) {
			p->right=b;
		} else {
			p->left=b;
		}
	} else {
		#ifdef DEBUG_AVLTREE
		stdoutput.printf("(new tree top)\n\n");
		#endif
		*treetop=b;
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

	// Since a was moved into a location in the tree that may not have
	// prevoiusly existed, and thus may have unbalanced the tree, we need
	// to continue balancing, starting at a.
	return a;
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
avltreenode<valuetype> *AVLTREENODE_CLASS::rightLeftRotate(
					avltreenode<valuetype> **treetop) {

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

	#ifdef DEBUG_AVLTREE
	stdoutput.printf("right-left rotation at ");
	_containerutil_print(value);
	stdoutput.printf(" {\n\n");
	stdoutput.printf("right part\n\n");
	#endif

	// do the right part of the right-left rotation...

	// get a, c, b, and "star"
	avltreenode<valuetype>	*a=this;
	avltreenode<valuetype>	*c=a->right;
	avltreenode<valuetype>	*b=c->left;
	avltreenode<valuetype>	*star=b->right;
	uint64_t		starheight=b->rightheight;

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

	#ifdef DEBUG_AVLTREE
	avltreenode<valuetype>	*top=this;
	while (top->parent) { top=top->parent; }
	top->print(); stdoutput.printf("\n");
	#endif

	// do the left part of the right-left rotation
	leftRotate(treetop);

	#ifdef DEBUG_AVLTREE
	stdoutput.printf("} right-left\n\n");
	#endif

	// Since c was moved into a location in the tree that may not have
	// prevoiusly existed, and thus may have unbalanced the tree, we need
	// to continue balancing, starting at c.
	return c;
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
avltreenode<valuetype> *AVLTREENODE_CLASS::rightRotate(
					avltreenode<valuetype> **treetop) {

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

	#ifdef DEBUG_AVLTREE
	stdoutput.printf("right rotation at ");
	_containerutil_print(value);
	stdoutput.printf("\n\n");
	#endif

	// get c, b, and "star"
	avltreenode<valuetype>	*c=this;
	avltreenode<valuetype>	*b=c->left;
	avltreenode<valuetype>	*star=b->right;
	uint64_t		starheight=b->rightheight;

	// move b
	avltreenode<valuetype>	*p=c->parent;
	if (p) {
		if (p->right==c) {
			p->right=b;
		} else {
			p->left=b;
		}
	} else {
		#ifdef DEBUG_AVLTREE
		stdoutput.printf("(new tree top)\n\n");
		#endif
		*treetop=b;
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

	// Since c was moved into a location in the tree that may not have
	// prevoiusly existed, and thus may have unbalanced the tree, we need
	// to continue balancing, starting at c.
	return c;
}

AVLTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
avltreenode<valuetype> *AVLTREENODE_CLASS::leftRightRotate(
					avltreenode<valuetype> **treetop) {

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

	#ifdef DEBUG_AVLTREE
	stdoutput.printf("left-right rotation at ");
	_containerutil_print(value);
	stdoutput.printf(" {\n\n");
	stdoutput.printf("left part\n\n");
	#endif

	// do the left part of the left-right rotation...

	// get c, a, b, and "star"
	avltreenode<valuetype>	*c=this;
	avltreenode<valuetype>	*a=c->left;
	avltreenode<valuetype>	*b=a->right;
	avltreenode<valuetype>	*star=b->left;
	uint64_t		starheight=b->leftheight;

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

	#ifdef DEBUG_AVLTREE
	avltreenode<valuetype>	*top=this;
	while (top->parent) { top=top->parent; }
	top->print(); stdoutput.printf("\n");
	#endif

	// do the right part of the left-right rotation
	rightRotate(treetop);

	#ifdef DEBUG_AVLTREE
	stdoutput.printf("} left-right\n\n");
	#endif

	// Since a was moved into a location in the tree that may not have
	// prevoiusly existed, and thus may have unbalanced the tree, we need
	// to continue balancing, starting at a.
	return a;
}
