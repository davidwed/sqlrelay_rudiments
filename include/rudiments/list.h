// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LIST_H
#define RUDIMENTS_LIST_H


// The list class allows you to store arbitrary amounts of data in a
// doubly-linked list.  Since the list class is template-based, you can store
// arbitrary types of data as well.
//
// Each list is composed of a series of listnode's.  Each listnode contains
// the data.

#include <rudiments/private/config.h>

#include <rudiments/private/listutil.h>

template <class datatype>
class listnode {
	public:
			listnode();
			// Creates a new list node, initializing the data to
			// 0 and the next/previous pointers to NULL.
		virtual	~listnode();
			// Destroys the list node.  Note that this method
			// does not delete the data contained in the node.

		virtual void		setData(datatype data);
					// Set the data stored in the node
					// to "data".
		virtual datatype	getData() const;
					// Return the data stored in the node.

		virtual int	compare(datatype data) const;
				// Returns -1,0 or 1 if the data stored in the
				// node is less than, equal to or greater than
				// "data".

		void	setPrevious(listnode<datatype> *previous);
			// Sets the pointer to the previous node to "previous".
		void	setNext(listnode<datatype> *next);
			// Sets the pointer to the next node to "next".

		listnode<datatype>	*getPrevious() const;
					// Returns the previous node in the
					// list.
		listnode<datatype>	*getNext() const;
					// Returns the next node in the list.

		virtual	void	print() const;
				// Prints the data stored in the node.

	#include <rudiments/private/listnode.h>
};

#include <rudiments/private/listnodeinlines.h>

template < class datatype, class listnodetype=listnode<datatype> >
class list {
	public:
			list();
			// Creates an empty list.
		virtual	~list();
			// Deletes the list and all of it's listnodes.

		void	append(datatype data);
			// Creates a new listnode containing "data" and
			// appends it to the list.
		int	insert(unsigned long index, datatype data);
			// Creates a new listnode containing "data" and
			// inserts it into the list at "index".

		int	removeByIndex(unsigned long index);
			// Deletes the listnode at "index".
		int	removeByData(datatype data);
			// Deletes the first listnode containing "data".
		int	removeAllByData(datatype data);
			// Deletes all listnodes containing "data".
		int	removeNode(listnodetype *node);
			// Removed listnode "node" from the list.

		int	setDataByIndex(unsigned long index,
						datatype data);
			// Sets the data contained in the listnode at "index"
			// to "data".
		int	getDataByIndex(unsigned long index,
						datatype *data) const;
			// Returns the data contained in the listnode at
			// "index".

		unsigned long	getLength() const;
				// Returns the length of the list.

		listnodetype	*getNodeByIndex(unsigned long index) const;
				// Returns a pointer to the listnode at "index".
		listnodetype	*getNodeByData(datatype data) const;
				// Returns a pointer to the first listnode 
				// containing "data".
		listnodetype	*getNodeByData(listnodetype *startnode,
							datatype data) const;
				// Returns a pointer to the first listnode 
				// after "startnode" containing "data".

		void	clear();
			// Deletes all listnodes currently in the list.
		void	print() const;
			// Prints out a representation of the list.

	#include <rudiments/private/list.h>
};

#include <rudiments/private/listinlines.h>

// string...
typedef listnode< char *>		stringlistnode;
typedef list< char *, stringlistnode >	stringlist;

#endif
