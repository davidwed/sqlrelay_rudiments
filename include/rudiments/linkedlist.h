// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LINKEDLIST_H
#define RUDIMENTS_LINKEDLIST_H

#include <rudiments/private/linkedlistincludes.h>

// The linkedlist class allows you to store arbitrary amounts of data in a
// doubly-linked list.  Since the linkedlist class is template-based, you can
// store arbitrary types of data as well.
//
// Each linkedlist is composed of a series of linkedlistnode's.  Each
// linkedlistnode contains the data.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

template <class datatype>
class linkedlistnode {
	public:
			linkedlistnode();
			// Creates a new linkedlist node, initializing the data
			// to 0 and the next/previous pointers to NULL.
		virtual	~linkedlistnode();
			// Destroys the linkedlist node.  Note that this method
			// does not delete the data contained in the node.

		void		setData(datatype data);
				// Set the data stored in the node
				// to "data".
		datatype	getData() const;
				// Return the data stored in the node.

		int	compare(datatype data) const;
			// Returns -1,0 or 1 if the data stored in the
			// node is less than, equal to or greater than
			// "data".

		void	setPrevious(linkedlistnode<datatype> *previous);
			// Sets the pointer to the previous node to "previous".
		void	setNext(linkedlistnode<datatype> *next);
			// Sets the pointer to the next node to "next".

		linkedlistnode<datatype>	*getPrevious();
					// Returns the previous node in the
					// linkedlist.
		linkedlistnode<datatype>	*getNext();
					// Returns the next node in the
					// linkedlist.

		void	print() const;
			// Prints the data stored in the node.

	#include <rudiments/private/linkedlistnode.h>
};

template < class datatype, class linkedlistnodetype=linkedlistnode<datatype> >
class linkedlist {
	public:
			linkedlist();
			// Creates an empty linkedlist.
		virtual	~linkedlist();
			// Deletes the linkedlist and all of it's
			// linkedlistnodes.

		void	append(datatype data);
			// Creates a new linkedlistnode containing "data" and
			// appends it to the linkedlist.
		void	append(linkedlistnodetype *node);
			// Appends already created linkedlistnode "node" to the
			// linkedlist.
		bool	insert(uint64_t index, datatype data);
			// Creates a new linkedlistnode containing "data" and
			// inserts it into the linkedlist at "index".
			//
			// Returns true on success and false on failure.
		bool	insert(uint64_t index, linkedlistnodetype *node);
			// Inserts already created linkedlistnode "node" into
			// the linkedlist at "index".
			//
			// Returns true on success and false on failure.

		bool	removeByIndex(uint64_t index);
			// Deletes the linkedlistnode at "index".
			//
			// Returns true on success and false on failure.
		bool	removeByData(datatype data);
			// Deletes the first linkedlistnode containing "data".
			//
			// Returns true on success and false on failure.
		bool	removeAllByData(datatype data);
			// Deletes all linkedlistnodes containing "data".
			//
			// Returns true on success and false on failure.
		bool	removeNode(linkedlistnodetype *node);
			// Removed linkedlistnode "node" from the linkedlist.
			//
			// Returns true on success and false on failure.

		bool	setDataByIndex(uint64_t index,
						datatype data);
			// Sets the data contained in the linkedlistnode at
			// "index" to "data".
			//
			// Returns true on success and false on failure.
		bool	getDataByIndex(uint64_t index,
						datatype *data);
			// Returns the data contained in the linkedlistnode at
			// "index".
			//
			// Returns true on success and false on failure.

		uint64_t	getLength() const;
				// Returns the length of the linkedlist.

		linkedlistnodetype	*getFirstNode();
				// Returns the first node in the linkedlist.
		linkedlistnodetype	*getLastNode();
				// Returns the last node in the linkedlist.

		linkedlistnodetype	*getNodeByIndex(uint64_t index);
				// Returns a pointer to the linkedlistnode at
				// "index".
		linkedlistnodetype	*getNodeByData(datatype data);
				// Returns a pointer to the first
				// linkedlistnode containing "data".
		linkedlistnodetype	*getNodeByData(
						linkedlistnodetype *startnode,
							datatype data);
				// Returns a pointer to the first
				// linkedlistnode after "startnode" containing
				// "data".

		void	clear();
			// Deletes all linkedlistnodes currently in the
			// linkedlist.

		void	print() const;
			// Prints out a representation of the linkedlist.

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
