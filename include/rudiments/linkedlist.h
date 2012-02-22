// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LINKEDLIST_H
#define RUDIMENTS_LINKEDLIST_H

#include <rudiments/private/linkedlistincludes.h>

/** The linkedlist class allows you to store arbitrary amounts of data in a
 *  doubly-linked list.  Since the linkedlist class is template-based, you can
 *  store arbitrary types of data as well.
 * 
 *  Each linkedlist is composed of a series of linkedlistnode's.  Each
 *  linkedlistnode contains the data. */

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

template <class datatype>
class linkedlistnode {
	public:
		/** Creates an empty instance of the linkedlistnode class. */
		linkedlistnode();

		/** Deletes this instance of the linkedlistnode class.
		 *  Note however, that the data stored in the linkedlistnode
		 *  is not deleted by this call. */
		virtual	~linkedlistnode();

		/** Set the data stored in the node to "data". */
		void		setData(datatype data);

		/** Return the data stored in the node. */
		datatype	getData() const;

		/** Returns a negative number,0 or a positive number depending
		 *  on whether the key stored in the node is respectively
		 *  less than, equal to or greater than "testkey". */
		int32_t	compare(datatype data) const;

		/** Sets the pointer to the previous node to "previous". */
		void	setPrevious(linkedlistnode<datatype> *previous);

		/** Sets the pointer to the next node to "next". */
		void	setNext(linkedlistnode<datatype> *next);

		/** Returns the previous node in the linkedlist. */
		linkedlistnode<datatype>	*getPrevious();

		/** Returns the next node in the linkedlist. */
		linkedlistnode<datatype>	*getNext();

		/** Prints the data stored in the node. */
		void	print() const;

	#include <rudiments/private/linkedlistnode.h>
};

template < class datatype, class linkedlistnodetype=linkedlistnode<datatype> >
class linkedlist {
	public:
		/** Creates an empty instance of the linkedlist class. */
		linkedlist();

		/** Deletes this instance of the linkedlist class and all of
		 *  its linkedlistnodes.  Note however, that the daata stored
		 *  in each linkedlistnode is not deleted by this call. */
		virtual	~linkedlist();

		/** Creates a new linkedlistnode containing "data" and
		 *  appends it to the linkedlist. */
		void	append(datatype data);

		/** Appends already created linkedlistnode "node" to the
		 *  linkedlist. */
		void	append(linkedlistnodetype *node);

		/** Creates a new linkedlistnode containing "data" and
		 *  inserts it into the linkedlist at "index".
		 * 
		 *  Returns true on success and false on failure. */
		bool	insert(uint64_t index, datatype data);

		/** Inserts already created linkedlistnode "node" into
		 *  the linkedlist at "index".
		 * 
		 *  Returns true on success and false on failure. */
		bool	insert(uint64_t index, linkedlistnodetype *node);

		/** Deletes the linkedlistnode at "index".
		 * 
		 *  Returns true on success and false on failure. */
		bool	removeByIndex(uint64_t index);

		/** Deletes the first linkedlistnode containing "data".
		 * 
		 *  Returns true on success and false on failure. */
		bool	removeByData(datatype data);

		/** Deletes all linkedlistnodes containing "data".
		 * 
		 *  Returns true on success and false on failure. */
		bool	removeAllByData(datatype data);

		/** Removed linkedlistnode "node" from the linkedlist.
		 * 
		 *  Returns true on success and false on failure. */
		bool	removeNode(linkedlistnodetype *node);

		/** Sets the data contained in the linkedlistnode at
		 *  "index" to "data".
		 * 
		 *  Returns true on success and false on failure. */
		bool	setDataByIndex(uint64_t index, datatype data);

		/** Returns the data contained in the linkedlistnode at
		 *  "index".
		 * 
		 *  Returns true on success and false on failure. */
		bool	getDataByIndex(uint64_t index, datatype *data);

		/** Returns the number of nodes in the linkedlist. */
		uint64_t	getLength() const;

		/** Returns the first node in the linkedlist. */
		linkedlistnodetype	*getFirstNode();

		/** Returns the last node in the linkedlist. */
		linkedlistnodetype	*getLastNode();

		/** Returns a pointer to the linkedlistnode at "index". */
		linkedlistnodetype	*getNodeByIndex(uint64_t index);

		/** Returns a pointer to the first
		 *  linkedlistnode containing "data". */
		linkedlistnodetype	*getNodeByData(datatype data);

		/** Returns a pointer to the firs linkedlistnode
		 *  after "startnode" containing "data". */
		linkedlistnodetype	*getNodeByData(
						linkedlistnodetype *startnode,
						datatype data);

		/** Deletes all linkedlistnodes currently in the linkedlist.
		 *  Note however, that the daata stored in each linkedlistnode
		 *  is not deleted by this call. */
		void	clear();

		/** Prints out a representation of the linkedlist. */
		void	print() const;

	#include <rudiments/private/linkedlist.h>
};

// A class for containing linkedlists of strings is defined here for
// convenience.
typedef linkedlistnode< char * >		stringlistnode;
typedef linkedlist< char *, stringlistnode >	stringlist;

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#include <rudiments/private/linkedlistnodeinlines.h>
#include <rudiments/private/linkedlistinlines.h>

#endif
