// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LIST_H
#define RUDIMENTS_LIST_H

#include <rudiments/private/config.h>

template <class datatype>
class listnode {
	public:
			listnode();
		virtual	~listnode();

		virtual void		setData(datatype data);
		virtual datatype	getData() const;

		virtual int	compare(datatype data) const;

		void	setPrevious(listnode<datatype> *previous);
		void	setNext(listnode<datatype> *next);

		listnode<datatype>	*getPrevious() const;
		listnode<datatype>	*getNext() const;

		virtual	void	print() const;

	#include <rudiments/private/listnode.h>
};

#include <rudiments/private/listnodeinlines.h>

class stringlistnode : public listnode<char *> {
	public:
			stringlistnode() : listnode<char *>() {}
		virtual int	compare(char *data) const;
		virtual void	print() const;

};

#include <rudiments/private/stringlistnodeinlines.h>

template < class datatype, class listnodetype=listnode<datatype> >
class list {
	public:
			list();
			~list();

		void	append(datatype data);
		int	insert(unsigned long index, datatype data);

		int	removeByIndex(unsigned long index);
		int	removeByData(datatype data);
		int	removeAllByData(datatype data);
		int	removeNode(listnodetype *node);

		int	setDataByIndex(unsigned long index,
						datatype data);
		int	getDataByIndex(unsigned long index,
						datatype *data) const;

		unsigned long	getLength() const;

		listnodetype	*getNodeByIndex(unsigned long index) const;
		listnodetype	*getNodeByData(datatype data) const;
		listnodetype	*getNodeByData(listnodetype *startnode,
							datatype data) const;

		void	clear();
		void	print() const;

	#include <rudiments/private/list.h>
};

#include <rudiments/private/listinlines.h>

typedef list< char *, stringlistnode >	stringlist;

#endif
