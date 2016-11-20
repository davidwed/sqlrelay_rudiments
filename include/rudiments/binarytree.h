// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_BINARYTREE_H
#define RUDIMENTS_BINARYTREE_H

#include <rudiments/private/binarytreeincludes.h>

/** The binarytreenode class stores the values that compose a binarytree. */
template <class valuetype>
class binarytreenode {
	public:
		/** Creates an instance of the binarytreenode class that
		 *  stores value "value". */
		binarytreenode(valuetype value);

		/** Deletes this instance of the binarytreenode class.
		 *  Note however, that the value stored in the binarytreenode
		 *  is not deleted by this call. */
		~binarytreenode();

		/** Return the value stored in the node. */
		valuetype	getValue() const;

		/** Returns a negative number, 0, or a positive number depending
		 *  on whether the value stored in the node is respectively
		 *  less than, equal to or greater than "value". */
		int32_t	compare(valuetype value) const;

		/** Returns a negative number, 0, or a positive number depending
		 *  on whether the value stored in the node is respectively
		 *  less than, equal to or greater than the value stored in
		 *  "peer". */
		int32_t	compare(binarytreenode<valuetype> *peer) const;

		/** Returns the parent node in the binarytree or NULL
		 *  if this node is the top-most node in the tree. */
		binarytreenode<valuetype>	*getParent();

		/** Returns the left child of this node in the binarytree
		 *  or NULL if this node has no left child. */
		binarytreenode<valuetype>	*getLeftChild();

		/** Returns the right child of this node in the binarytree
		 *  or NULL if this node has no right child. */
		binarytreenode<valuetype>	*getRightChild();

		/** Returns the left height of this node in the binarytree. */
		uint64_t	getLeftHeight();

		/** Returns the right height of this node in the binarytree. */
		uint64_t	getRightHeight();

		/** Returns the previous node in the binarytree (in a
 		 *  depth-first traversal) or NULL if this node is the first
 		 *  node in the tree. */
		binarytreenode<valuetype>	*getPrevious();

		/** Returns the next node in the binarytree (in a
		 *  depth-first traversal) or NULL if this node is the last
		 *  node in the tree. */
		binarytreenode<valuetype>	*getNext();

		/** Prints the value stored in the node. */
		void	print() const;

	#include <rudiments/private/binarytreenode.h>
};

/** The binarytree class allows you to store an arbitrary number of values in a
 *  binary tree.  Since the binarytree class is template-based, you can store
 *  arbitrary types of values.
 * 
 *  Each binarytree is composed of a set of binarytreenodes.  Each
 *  binarytreenode contains a value. */
template < class valuetype >
class binarytree {
	public:
		/** Creates an empty instance of the binarytree class. */
		binarytree();

		/** Deletes this instance of the binarytree class and all of
		 *  its binarytreenodes.  Note however, that the data stored
		 *  in each binarytreenode is not deleted by this call. */
		~binarytree();

		/** Creates a new binarytreenode containing "value" and
		 *  prepends it to the binarytree. */
		void	insert(valuetype value);

		/** Inserts already created binarytreenode "node" to the
		 *  binarytree. */
		void	insert(binarytreenode<valuetype> *node);

		/** Detaches "node" from the tree. */
		binarytreenode<valuetype>
			*detach(binarytreenode<valuetype> *node);

		/** Deletes the first binarytreenode containing "value".
		 * 
		 *  Note that this operation requires a search and is expensive
		 *  in both execution time and code size.
		 *
		 *  Returns true on success and false on failure. */
		bool	remove(valuetype value);

		/** Deletes all binarytreenodes containing "value".
		 * 
		 *  Note that this operation requires a search and is expensive
		 *  in both execution time and code size.
		 * 
		 *  Returns true on success and false on failure. */
		bool	removeAll(valuetype value);

		/** Removed binarytreenode "node" from the binarytree.
		 * 
		 *  Note that this operation does not require a search and is
		 *  far less expensive than the remove(value) operation and
		 *  removeAll().
		 *
		 *  Returns true on success and false on failure. */
		bool	remove(binarytreenode<valuetype> *node);

		/** Returns the number of nodes in the binarytree. */
		uint64_t	getLength() const;

		/** Returns the top-most node in the binarytree. */
		binarytreenode<valuetype>	*getTop();

		/** Returns the first node in the binarytree (in a depth-first
		 *  traversal). */
		binarytreenode<valuetype>	*getFirst();

		/** Returns the last node in the binarytree (in a depth-first
		 *  traversal). */
		binarytreenode<valuetype>	*getLast();

		/** Returns the node prior to "node" or NULL if this node is
		 *  the first node in the tree (in a depth-first traversal).
		 *  "node" is presumed to be in the tree. */
		binarytreenode<valuetype>	*getPrevious(
					binarytreenode<valuetype> *node);

		/** Returns the node after "node" or NULL if this node is the
		 *  last node in the tree (in a depth-first traversal). "node"
		 *  is presumed to be in the tree. */
		binarytreenode<valuetype>	*getNext(
					binarytreenode<valuetype> *node);

		/** Returns a pointer to the first binarytreenode
		 *  containing "value" (in a depth-first traversal) or NULL if
		 *  "value" was not found. */
		binarytreenode<valuetype>	*find(valuetype value);

		/** Returns a pointer to the first binarytreenode
		 *  after "startnode" containing "value" (in a depth-first
		 *  traversal) or NULL if "value" was not found. */
		binarytreenode<valuetype>
			*find(binarytreenode<valuetype> *startnode,
							valuetype value);

		/** Deletes all binarytreenodes currently in the binarytree.
		 *  Note however, that the data stored in each binarytreenode
		 *  is not deleted by this call. */
		void	clear();

		/** Prints out a representation of the binarytree. */
		void	print() const;

	#include <rudiments/private/binarytree.h>
};

#include <rudiments/private/binarytreeinlines.h>

#endif
