// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DICTIONARY_H
#define RUDIMENTS_DICTIONARY_H

#include <rudiments/private/config.h>

template <class keytype, class datatype>
class dictionarynode {
	public:
			dictionarynode();

		void	setKey(keytype key);
		void	setData(datatype data);

		keytype		getKey() const;
		datatype	getData() const;

		virtual	int	compare(keytype testkey) const;
		virtual void	print() const;

	#include <rudiments/private/dictionarynode.h>
};

#include <rudiments/private/dictionarynodeinlines.h>

template <class datatype>
class stringdictionarynode : public dictionarynode<char *,datatype> {
	public:
			stringdictionarynode() :
				dictionarynode<char *,datatype>() {};
		int	compare(char * testkey) const;
		void	print() const;
};

#include <rudiments/private/stringdictionarynodeinlines.h>

template <class keytype, class datatype>
class objectdictionarynode : public dictionarynode<keytype,datatype> {
	public:
			objectdictionarynode() :
				dictionarynode<keytype,datatype>() {};
		int	compare(keytype testkey) const;
		void	print() const;
};

#include <rudiments/private/objectdictionarynodeinlines.h>

#include <rudiments/list.h>

template <class keytype, class datatype>
class dictionarylistnode :
		public listnode< dictionarynode<keytype,datatype> * > {};

template <class keytype, class datatype>
class dictionarylist :
		public list< dictionarynode<keytype,datatype> *,
				dictionarylistnode<keytype,datatype> > {};

template <class keytype, class datatype,
	class dictionarynodetype=dictionarynode<keytype,datatype>,
	class dictionarylistnodetype=dictionarylistnode<keytype,datatype>,
	class dictionarylisttype=dictionarylist<keytype,datatype> >
class dictionary {
	public:
			dictionary();
			~dictionary();

		int	setData(keytype key, datatype data);
		int	getData(keytype key, datatype *data);
		int	removeData(keytype key);

		dictionarylisttype	*getList();

		void	print() const;

	#include <rudiments/private/dictionary.h>
};

#include <rudiments/private/dictionaryinlines.h>

template <class datatype>
class stringdictionarylistnode :
		public dictionarylistnode< char *, datatype > {};

template <class datatype>
class stringdictionarylist : public dictionarylist< char *, datatype > {};

template <class datatype>
class stringdictionary : public dictionary< char *, datatype,
				stringdictionarynode<datatype>,
				stringdictionarylistnode<datatype>,
				stringdictionarylist<datatype> > {};

template <class datatype>
class numericdictionarylistnode :
		public dictionarylistnode< long, datatype > {};

template <class datatype>
class numericdictionarylist : public dictionarylist< long, datatype > {};

template <class datatype>
class numericdictionary : public dictionary< long, datatype,
				dictionarynode<long, datatype>,
				numericdictionarylistnode<datatype>,
				numericdictionarylist<datatype> > {};

typedef stringdictionarylistnode<char *> namevaluepairslistnode;
typedef stringdictionarylist<char *> namevaluepairslist;
typedef stringdictionary<char *> namevaluepairs;

#endif
