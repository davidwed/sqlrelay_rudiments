// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef LIST_H
#define LIST_H

#include <rudiments/private/config.h>

template <class type>
class listnode {
	public:
			listnode();
			listnode(type value);
			listnode(type value,
					listnode<type> *previous,
					listnode<type> *next);

		void	setValue(type value);
		type	getValue() const;

		void	setPrevious(listnode<type> *previous);
		void	setNext(listnode<type> *next);

		listnode<type>	*getPrevious() const;
		listnode<type>	*getNext() const;

	#include <rudiments/private/listnode.h>
};

#include <rudiments/private/listnodeinlines.h>

template <class type>
class list {
	public:
			list();
			~list();

		void	append(type value);
		int	insert(unsigned long index, type value);

		int	removeIndex(unsigned long index);
		int	removeValue(type value);
		int	removeAllValues(type value);
		int	removeNode(listnode<type> *node);

		int	setValue(unsigned long index, type value);
		int	getValue(unsigned long index, type *value) const;

		unsigned long	getLength() const;

		listnode<type>	*getNode(unsigned long index) const;

		void	clear();

	#include <rudiments/private/list.h>
};

#include <rudiments/private/listinlines.h>

#endif
