// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LIST_H
#define RUDIMENTS_LIST_H

#include <rudiments/private/config.h>

template <class datatype, class keytype=datatype>
class listnode {
	public:
			listnode();
			listnode(datatype data);
			listnode(datatype data,
					listnode<datatype,keytype> *previous,
					listnode<datatype,keytype> *next);
		virtual	~listnode();

		virtual void		setData(datatype data);
		virtual datatype	getData() const;

		virtual int	compare(keytype key)=0;

		void	setPrevious(listnode<datatype,keytype> *previous);
		void	setNext(listnode<datatype,keytype> *next);

		listnode<datatype,keytype>	*getPrevious() const;
		listnode<datatype,keytype>	*getNext() const;

	#include <rudiments/private/listnode.h>
};

#include <rudiments/private/listnodeinlines.h>

template <class datatype>
class primitivelistnode : public listnode<datatype> {
	public:
			primitivelistnode() :
				listnode<datatype>() {};
			primitivelistnode(datatype data) :
				listnode<datatype>(data) {};
			primitivelistnode(datatype data,
					primitivelistnode<datatype> *previous,
					primitivelistnode<datatype> *next) :
				listnode<datatype>(data,previous,next) {};
		virtual int	compare(datatype key);
};

#include <rudiments/private/primitivelistnodeinlines.h>

class stringlistnode : public listnode<char *> {
	public:
			stringlistnode() :
				listnode<char *>() {}
			stringlistnode(char * data) :
				listnode<char *>(data) {}
			stringlistnode(char * data,
					stringlistnode *previous,
					stringlistnode *next) :
				listnode<char *>(data,previous,next) {}
		virtual int	compare(char *key);
};

#include <rudiments/private/stringlistnodeinlines.h>

template <class datatype, class keytype>
class objectlistnode : public listnode<datatype,keytype> {
	public:
			objectlistnode() :
				listnode<datatype,keytype>() {};
			objectlistnode(datatype data) :
				listnode<datatype,keytype>(data) {};
			objectlistnode(datatype data,
				objectlistnode<datatype,keytype> *previous,
				objectlistnode<datatype,keytype> *next) :
			listnode<datatype,keytype>(data,previous,next) {};
		virtual int	compare(keytype key);
};

#include <rudiments/private/objectlistnodeinlines.h>

template < class datatype, class keytype=datatype,
		class listnodetype=primitivelistnode<datatype> >
class list {
	public:
			list();
			~list();

		void	append(datatype data);
		int	insert(unsigned long index, datatype data);

		int	removeByIndex(unsigned long index);
		int	removeByKey(keytype key);
		int	removeAllByKey(keytype key);
		int	removeNode(listnodetype *node);

		int	setDataByIndex(unsigned long index,
						datatype data);
		int	getDataByIndex(unsigned long index,
						datatype *data) const;

		int	getDataByKey(keytype key, datatype *data) const;

		unsigned long	getLength() const;

		listnodetype	*getNodeByIndex(unsigned long index) const;

		void	clear();

	#include <rudiments/private/list.h>
};

#include <rudiments/private/listinlines.h>

typedef list< char *, char *, stringlistnode >	stringlist;

template <class datatype>
class primitivelist : public list< datatype, datatype,
					primitivelistnode<datatype> > {};

template <class datatype, class keytype>
class objectlist : public list< datatype, keytype,
					objectlistnode<datatype,keytype> > {};

#endif
