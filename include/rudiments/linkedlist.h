// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <rudiments/private/config.h>

template <class type>
class linkedlistnode {
	public:
			linkedlistnode();
			linkedlistnode(type value);
			linkedlistnode(type value,
					linkedlistnode<type> *previous,
					linkedlistnode<type> *next);

		void	setValue(type value);
		type	getValue() const;

		void	setPrevious(linkedlistnode<type> *previous);
		void	setNext(linkedlistnode<type> *next);

		linkedlistnode<type>	*getPrevious() const;
		linkedlistnode<type>	*getNext() const;

	#include <rudiments/private/linkedlistnode.h>
};

#include <rudiments/private/linkedlistnodeinlines.h>

template <class type>
class linkedlist {
	public:
			linkedlist();
			~linkedlist();

		void	append(type value);
		int	insert(unsigned long index, type value);

		int	removeIndex(unsigned long index);
		int	removeValue(type value);
		int	removeAllValues(type value);
		int	removeNode(linkedlistnode<type> *node);

		int	setValue(unsigned long index, type value);
		int	getValue(unsigned long index, type *value) const;

		unsigned long	getLength() const;

		linkedlistnode<type>	*getNode(unsigned long index) const;

		void	clear();

	#include <rudiments/private/linkedlist.h>
};

#include <rudiments/private/linkedlistinlines.h>

#endif
