// Copyright (c) 2016 David Muse
// See the COPYING file for more information

//#define DEBUG_BINARYTREE 1

#include <rudiments/stdio.h>
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
	if (!node) {
		return;
	} else if (top) {
		binarytreenode<valuetype>	*newtop=top->insert(node);
		if (newtop) {
			top=newtop;
		}
		for (first=top;
			first->getLeftChild();
			first=first->getLeftChild()) {}
		for (last=top;
			last->getRightChild();
			last=last->getRightChild()) {}
	} else if (!top) {
		top=node;
		first=node;
		last=node;
	}
	length++;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREE_CLASS::detach(binarytreenode<valuetype> *node) {
	if (!node) {
		return;
	}
	if (node==first) {
		first=node->getNext();
	}
	if (node==last) {
		last=node->getPrevious();
	}
	if (node->getLeftChild()) {
		// FIXME: pull left node up
	}
	if (node->getRightChild()) {
		// FIXME: pull right node up
	}
	if (node->getParent()) {
		// FIXME: set to pulled-up node
	}
	if (node==top) {
		// FIXME: set to pulled-up node
	}
	node->setParent(NULL);
	node->setLeftChild(NULL);
	node->setRightChild(NULL);
	length--;
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
	binarytreenode<valuetype>	*current=first;
	binarytreenode<valuetype>	*next;
	while (current) {
		next=current->getNext();
		if (!current->compare(value) && !remove(current)) {
			return false;
		}
		current=next;
	}
	return true;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
bool BINARYTREE_CLASS::remove(binarytreenode<valuetype> *node) {
	detach(node);
	delete node;
	return true;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
uint64_t BINARYTREE_CLASS::getLength() const {
	return length;
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
	return find((binarytreenode<valuetype> *)first,value);
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
binarytreenode<valuetype> *BINARYTREE_CLASS::find(
					binarytreenode<valuetype> *startnode,
					valuetype value) {
	for (binarytreenode<valuetype> *current=startnode;
			current; current=current->getNext()) {
		if (!current->compare(value)) {
			return current;
		}
	}
	return NULL;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREE_CLASS::sort() {

	// detach nodes and insert them into a new tree
	BINARYTREE_CLASS	newtree;
	while (first) {
		BINARYTREENODE_CLASS	*node=detach(first);
		newtree.insert(node);
	}

	// grab the nodes and length from the new tree
	top=newtree.top;
	first=newtree.first;
	last=newtree.last;
	length=newtree.length;

	// set first to NULL so clear() won't do anything
	// when the new tree is deallocated
	newtree.first=NULL;
}

BINARYTREE_TEMPLATE
RUDIMENTS_TEMPLATE_INLINE
void BINARYTREE_CLASS::clear() {
	while (first) {
		remove(first);
	}
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
	stdoutput.printf("\" lh=\"%d\" rh=\"%d\" bf=\"%d\"",
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

	#ifdef DEBUG_BINARYTREE
	stdoutput.printf("----------------------------------------"
				"---------------------------------------\n");
	#endif

	// find a location to insert the node (should always be a leaf node)
	binarytreenode<valuetype>	*location=this;
	for (;;) {

		if (node->value<=location->value) {

			if (location->left) {
				location=location->left;
			} else {

				#ifdef DEBUG_BINARYTREE
				stdoutput.printf("insert %d "
						"to left of %d {\n\n",
						node->value,
						location->value);
				#endif

				location->setLeftChild(node);
				break;
			}

		} else if (node->value>location->value) {

			if (location->right) {
				location=location->right;
			} else {

				#ifdef DEBUG_BINARYTREE
				stdoutput.printf("insert %d "
						"to right of %d {\n\n",
						node->value,
						location->value);
				#endif

				location->setRightChild(node);
				break;
			}
		}
	}

	node->setParent(location);

	// update heights
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
	stdoutput.printf("balanceUp at %d {\n\n",value);
	#endif

	binarytreenode<valuetype>	*newtreetop=NULL;
	for (binarytreenode<valuetype> *node=this; node; node=node->parent) {
		binarytreenode<valuetype>	*result=node->balance();
		if (result) {
			newtreetop=result;
		};
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
		stdoutput.printf("imbalance at %d\n\n",value);
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
	stdoutput.printf("left rotation at %d {\n\n",value);
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

	// update heights
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
	stdoutput.printf("right-left rotation at %d {\n\n",value);
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

	// update heights
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
	stdoutput.printf("right rotation at %d {\n\n",value);
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

	// update heights
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
	stdoutput.printf("left-right rotation at %d {\n\n",value);
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

	// update heights
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
