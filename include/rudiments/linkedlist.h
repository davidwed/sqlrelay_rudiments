// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LINKEDLIST_H
#define RUDIMENTS_LINKEDLIST_H

#include <rudiments/private/linkedlistincludes.h>

/** The linkedlistnode class stores the values that compose a linkedlist. */
template <class valuetype>
class linkedlistnode {
	public:
		/** Creates an instance of the linkedlistnode class that
		 *  stores value "value". */
		linkedlistnode(valuetype value);

		/** Deletes this instance of the linkedlistnode class.
		 *  Note however, that the value stored in the linkedlistnode
		 *  is not deleted by this call. */
		virtual	~linkedlistnode();

		/** Set the value stored in the node to "value". */
		void		setValue(valuetype value);

		/** Return the value stored in the node. */
		valuetype	getValue() const;

		/** Returns a negative number, 0, or a positive number depending
		 *  on whether the value stored in the node is respectively
		 *  less than, equal to or greater than "value". */
		int32_t	compare(valuetype value) const;

		/** Returns the previous node in the linkedlist or NULL
		 *  if this node is the first node in the list. */
		linkedlistnode<valuetype>	*getPrevious();

		/** Returns the next node in the linkedlist or NULL
		 * if this node is the last node in the list. */
		linkedlistnode<valuetype>	*getNext();

		/** Prints the value stored in the node. */
		void	print() const;

	#include <rudiments/private/linkedlistnode.h>
};

/** The linkedlist class allows you to store an arbitrary number of values in a
 *  doubly-linked list.  Since the linkedlist class is template-based, you can
 *  store arbitrary types of values.
 * 
 *  Each linkedlist is composed of a series of linkedlistnodes.  Each
 *  linkedlistnode contains the value. */
template < class valuetype >
class linkedlist {
	public:
		/** Creates an empty instance of the linkedlist class. */
		linkedlist();

		/** Deletes this instance of the linkedlist class and all of
		 *  its linkedlistnodes.  Note however, that the data stored
		 *  in each linkedlistnode is not deleted by this call. */
		virtual	~linkedlist();

		/** Creates a new linkedlistnode containing "value" and
		 *  prepends it to the linkedlist. */
		void	prepend(valuetype value);

		/** Prepends already created linkedlistnode "node" to the
		 *  linkedlist. */
		void	prepend(linkedlistnode<valuetype> *node);

		/** Creates a new linkedlistnode containing "value" and
		 *  appends it to the linkedlist. */
		void	append(valuetype value);

		/** Appends already created linkedlistnode "node" to the
		 *  linkedlist. */
		void	append(linkedlistnode<valuetype> *node);

		/** Creates a new linkedlistnode containing "value" and
		 *  inserts it into the linkedlist before "node". */
		void	insertBefore(linkedlistnode<valuetype> *node,
							valuetype value);

		/** Inserts already created linkedlistnode "newnode" into the
		 *  linkedlist before "node". */
		void	insertBefore(linkedlistnode<valuetype> *node,
					linkedlistnode<valuetype> *newnode);

		/** Creates a new linkedlistnode containing "value" and
		 *  inserts it into the linkedlist after "node". */
		void	insertAfter(linkedlistnode<valuetype> *node,
							valuetype value);

		/** Inserts already created linkedlistnode "node" into the
		 *  linkedlist after "node". */
		void	insertAfter(linkedlistnode<valuetype> *node,
					linkedlistnode<valuetype> *newnode);

		/** Deletes the first linkedlistnode containing "value".
		 * 
		 *  Note that this operation requires a search and is expensive
		 *  in both execution time and code size.
		 *
		 *  Returns true on success and false on failure. */
		bool	remove(valuetype value);

		/** Deletes all linkedlistnodes containing "value".
		 * 
		 *  Note that this operation requires a search and is expensive
		 *  in both execution time and code size.
		 * 
		 *  Returns true on success and false on failure. */
		bool	removeAll(valuetype value);

		/** Removed linkedlistnode "node" from the linkedlist.
		 * 
		 *  Note that this operation does not require a search and is
		 *  far less expensive than the remove(value) operation and
		 *  removeAll().
		 *
		 *  Returns true on success and false on failure. */
		bool	remove(linkedlistnode<valuetype> *node);

		/** Returns the number of nodes in the linkedlist. */
		uint64_t	getLength() const;

		/** Returns the first node in the linkedlist. */
		linkedlistnode<valuetype>	*getFirst();

		/** Returns the last node in the linkedlist. */
		linkedlistnode<valuetype>	*getLast();

		/** Returns the node prior to "node" or NULL if this node is
		 *  the first node in the list.  "node" is presumed to be in
		 *  the list. */
		linkedlistnode<valuetype>	*getPrevious(
					linkedlistnode<valuetype> *node);

		/** Returns the node after "node" or NULL if this node is the
		 *  last node in the list. "node" is presumed to be in the
		 *  list. */
		linkedlistnode<valuetype>	*getNext(
					linkedlistnode<valuetype> *node);

		/** Returns a pointer to the first linkedlistnode
		 *  containing "value" or NULL if "value" was not found. */
		linkedlistnode<valuetype>	*find(valuetype value);

		/** Returns a pointer to the first linkedlistnode
		 *  after "startnode" containing "value" or NULL
		 *  if "value" was not found. */
		linkedlistnode<valuetype>
			*find(linkedlistnode<valuetype> *startnode,
						valuetype value);

		/** Deletes all linkedlistnodes currently in the linkedlist.
		 *  Note however, that the data stored in each linkedlistnode
		 *  is not deleted by this call. */
		void	clear();

		/** Prints out a representation of the linkedlist. */
		void	print() const;

	#include <rudiments/private/linkedlist.h>
};


#include <rudiments/private/linkedlistinlines.h>

#endif
