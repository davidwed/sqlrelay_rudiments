// Copyright (c) 2014 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SINGLYLINKEDLIST_H
#define RUDIMENTS_SINGLYLINKEDLIST_H

#include <rudiments/private/singlylinkedlistincludes.h>

/** The singlylinkedlistnode class stores the values that compose a
 *  singlylinkedlist. */
template <class valuetype>
class singlylinkedlistnode {
	public:
		/** Creates an instance of the singlylinkedlistnode class that
		 *  stores value "value". */
		singlylinkedlistnode(valuetype value);

		/** Deletes this instance of the singlylinkedlistnode class.
		 *  Note however, that the value stored in the
		 *  singlylinkedlistnode is not deleted by this call. */
		virtual	~singlylinkedlistnode();

		/** Set the value stored in the node to "value". */
		void		setValue(valuetype value);

		/** Return the value stored in the node. */
		valuetype	getValue() const;

		/** Returns a negative number, 0, or a positive number depending
		 *  on whether the value stored in the node is respectively
		 *  less than, equal to or greater than "value". */
		int32_t	compare(valuetype value) const;

		/** Returns the next node in the singlylinkedlist or NULL
		 * if this node is the last node in the list. */
		singlylinkedlistnode<valuetype>	*getNext();

		/** Prints the value stored in the node. */
		void	print() const;

	#include <rudiments/private/singlylinkedlistnode.h>
};

/** The singlylinkedlist class allows you to store an arbitrary number of
 *  values in a singly-linked list.  Since the singlylinkedlist class is
 *  template-based, you can store arbitrary types of values.
 * 
 *  Each singlylinkedlist is composed of a series of singlylinkedlistnodes.
 *  Each singlylinkedlistnode contains the value. */
template < class valuetype >
class singlylinkedlist {
	public:
		/** Creates an empty instance of the singlylinkedlist class. */
		singlylinkedlist();

		/** Deletes this instance of the singlylinkedlist class and all
		 *  of its singlylinkedlistnodes.  Note however, that the data
		 *  stored in each singlylinkedlistnode is not deleted by this
		 *  call. */
		virtual	~singlylinkedlist();

		/** Creates a new singlylinkedlistnode containing "value" and
		 *  prepends it to the singlylinkedlist. */
		void	prepend(valuetype value);

		/** Prepends already created singlylinkedlistnode "node" to the
		 *  singlylinkedlist. */
		void	prepend(singlylinkedlistnode<valuetype> *node);

		/** Creates a new singlylinkedlistnode containing "value" and
		 *  appends it to the singlylinkedlist. */
		void	append(valuetype value);

		/** Appends already created singlylinkedlistnode "node" to the
		 *  singlylinkedlist. */
		void	append(singlylinkedlistnode<valuetype> *node);

		/** Creates a new singlylinkedlistnode containing "value" and
		 *  inserts it into the singlylinkedlist after "node". */
		void	insertAfter(singlylinkedlistnode<valuetype> *node,
							valuetype value);

		/** Inserts already created singlylinkedlistnode "node" into the
		 *  singlylinkedlist after "node". */
		void	insertAfter(singlylinkedlistnode<valuetype> *node,
				singlylinkedlistnode<valuetype> *newnode);

		/** Deletes the first singlylinkedlistnode containing "value".
		 * 
		 *  Returns true on success and false on failure. */
		bool	remove(valuetype value);

		/** Deletes all singlylinkedlistnodes containing "value".
		 * 
		 *  Returns true on success and false on failure. */
		bool	removeAll(valuetype value);

		/** Removed singlylinkedlistnode "node" from the
		 *  singlylinkedlist.
		 * 
		 *  Returns true on success and false on failure. */
		bool	remove(singlylinkedlistnode<valuetype> *node);

		/** Returns the number of nodes in the singlylinkedlist. */
		uint64_t	getLength() const;

		/** Returns the first node in the singlylinkedlist. */
		singlylinkedlistnode<valuetype>	*getFirst();

		/** Returns the last node in the singlylinkedlist. */
		singlylinkedlistnode<valuetype>	*getLast();

		/** Returns the node after "node" or NULL if this node is the
		 *  last node in the list. "node" is presumed to be in the
		 *  list. */
		singlylinkedlistnode<valuetype>	*getNext(
					singlylinkedlistnode<valuetype> *node);

		/** Returns a pointer to the first singlylinkedlistnode
		 *  containing "value" or NULL if "value" was not found. */
		singlylinkedlistnode<valuetype>	*find(valuetype value);

		/** Returns a pointer to the first singlylinkedlistnode
		 *  after "startnode" containing "value" or NULL
		 *  if "value" was not found. */
		singlylinkedlistnode<valuetype>
			*find(singlylinkedlistnode<valuetype> *startnode,
							valuetype value);

		/** Deletes all singlylinkedlistnodes currently in the
		 *  singlylinkedlist.  Note however, that the data stored in
		 *  each singlylinkedlistnode is not deleted by this call. */
		void	clear();

		/** Prints out a representation of the linkedlist. */
		void	print() const;

	#include <rudiments/private/singlylinkedlist.h>
};


#include <rudiments/private/singlylinkedlistinlines.h>

#endif
