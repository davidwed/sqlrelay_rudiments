// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LINKEDLIST_H
#define RUDIMENTS_LINKEDLIST_H

#include <rudiments/private/linkedlistincludes.h>

/** The linkedlistnode class stores the values that compose a linkedlist. */
template <class valuetype>
class linkedlistnode : public baselinkedlistnode {
	public:
		/** Creates an empty instance of the linkedlistnode class. */
		linkedlistnode();

		/** Deletes this instance of the linkedlistnode class.
		 *  Note however, that the value stored in the linkedlistnode
		 *  is not deleted by this call. */
		virtual	~linkedlistnode();

		/** Set the value stored in the node to "value". */
		void		setValue(valuetype value);

		/** Return the value stored in the node. */
		valuetype	getValue() const;

		/** Returns a negative number,0 or a positive number depending
		 *  on whether the key stored in the node is respectively
		 *  less than, equal to or greater than "testkey". */
		int32_t	compare(valuetype value) const;
		int32_t	compareInternal(uint64_t value) const;

		/** Sets the pointer to the previous node to "previous". */
		void	setPrevious(linkedlistnode<valuetype> *previous);

		/** Sets the pointer to the next node to "next". */
		void	setNext(linkedlistnode<valuetype> *next);

		/** Returns the previous node in the linkedlist. */
		linkedlistnode<valuetype>	*getPrevious();

		/** Returns the next node in the linkedlist. */
		linkedlistnode<valuetype>	*getNext();

		/** Prints the value stored in the node. */
		void	print() const;
};

/** The linkedlist class allows you to store an arbitrary number of values in a
 *  doubly-linked list.  Since the linkedlist class is template-based, you can
 *  store arbitrary value types as well.
 * 
 *  Each linkedlist is composed of a series of linkedlistnode's.  Each
 *  linkedlistnode contains the value. */
template < class valuetype >
class linkedlist : public baselinkedlist {
	public:
		/** Creates an empty instance of the linkedlist class. */
		linkedlist();

		/** Deletes this instance of the linkedlist class and all of
		 *  its linkedlistnodes.  Note however, that the daata stored
		 *  in each linkedlistnode is not deleted by this call. */
		virtual	~linkedlist();

		baselinkedlistnode	*newNode();

		/** Creates a new linkedlistnode containing "value" and
		 *  appends it to the linkedlist. */
		void	append(valuetype value);

		/** Appends already created linkedlistnode "node" to the
		 *  linkedlist. */
		void	append(linkedlistnode<valuetype> *node);

		/** Creates a new linkedlistnode containing "value" and
		 *  inserts it into the linkedlist at "index".
		 * 
		 *  Returns true on success and false on failure. */
		bool	insert(uint64_t index, valuetype value);

		/** Inserts already created linkedlistnode "node" into
		 *  the linkedlist at "index".
		 * 
		 *  Returns true on success and false on failure. */
		bool	insert(uint64_t index, linkedlistnode<valuetype> *node);

		/** Deletes the linkedlistnode at "index".
		 * 
		 *  Returns true on success and false on failure. */
		bool	removeByIndex(uint64_t index);

		/** Deletes the first linkedlistnode containing "value".
		 * 
		 *  Returns true on success and false on failure. */
		bool	removeByValue(valuetype value);

		/** Deletes all linkedlistnodes containing "value".
		 * 
		 *  Returns true on success and false on failure. */
		bool	removeAllByValue(valuetype value);

		/** Removed linkedlistnode "node" from the linkedlist.
		 * 
		 *  Returns true on success and false on failure. */
		bool	removeNode(linkedlistnode<valuetype> *node);

		/** Sets the value contained in the linkedlistnode at
		 *  "index" to "value".
		 * 
		 *  Returns true on success and false on failure. */
		bool	setValueByIndex(uint64_t index, valuetype value);

		/** Returns the value contained in the linkedlistnode at
		 *  "index".
		 * 
		 *  Returns true on success and false on failure. */
		bool	getValueByIndex(uint64_t index, valuetype *value);

		/** Returns the number of nodes in the linkedlist. */
		uint64_t	getLength() const;

		/** Returns the first node in the linkedlist. */
		linkedlistnode<valuetype>	*getFirstNode();

		/** Returns the last node in the linkedlist. */
		linkedlistnode<valuetype>	*getLastNode();

		/** Returns a pointer to the linkedlistnode at "index". */
		linkedlistnode<valuetype>
				*getNodeByIndex(uint64_t index);

		/** Returns a pointer to the first
		 *  linkedlistnode containing "value". */
		linkedlistnode<valuetype>
				*getNodeByValue(valuetype value);

		/** Returns a pointer to the firs linkedlistnode
		 *  after "startnode" containing "value". */
		linkedlistnode<valuetype>
			*getNodeByValue(linkedlistnode<valuetype> *startnode,
					valuetype value);

		/** Deletes all linkedlistnodes currently in the linkedlist.
		 *  Note however, that the daata stored in each linkedlistnode
		 *  is not deleted by this call. */
		void	clear();

		/** Prints out a representation of the linkedlist. */
		void	print() const;
};


// ideally I'd use typdefs for these but older compilers can't handle them
#define stringlistnode	linkedlistnode< char * >
#define stringlist	linkedlist< char * >

#include <rudiments/private/linkedlistinlines.h>

#endif
