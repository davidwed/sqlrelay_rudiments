// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DICTIONARY_H
#define RUDIMENTS_DICTIONARY_H

#include <rudiments/private/config.h>

template <class keytype, class datatype>
class dictionarynode {
	public:
			dictionarynode();
			dictionarynode(keytype key, datatype data);

		void	setKey(keytype key);
		void	setData(datatype data);

		keytype		getKey() const;
		datatype	getData() const;

		virtual	int	compare(keytype key)=0;

	#include <rudiments/private/dictionarynode.h>
};

#include <rudiments/private/dictionarynodeinlines.h>

template <class keytype, class datatype>
class primitivedictionarynode : public dictionarynode<keytype,datatype> {
	public:
			primitivedictionarynode() :
				dictionarynode<keytype,datatype>() {};
			primitivedictionarynode(keytype key, datatype data) :
				dictionarynode<keytype,datatype>(key,data) {};
		int	compare(keytype key);
};

#include <rudiments/private/primitivedictionarynodeinlines.h>

template <class datatype>
class stringdictionarynode : public dictionarynode<char *,datatype> {
	public:
			stringdictionarynode() :
				dictionarynode<char *,datatype>() {};
			stringdictionarynode(char *key, datatype data) :
				dictionarynode<char *,datatype>(key,data) {};
		int	compare(char * key);
};

#include <rudiments/private/stringdictionarynodeinlines.h>

template <class keytype, class datatype>
class objectdictionarynode : public dictionarynode<keytype,datatype> {
	public:
			objectdictionarynode() :
				dictionarynode<keytype,datatype>() {};
			objectdictionarynode(keytype key, datatype data) :
				dictionarynode<keytype,datatype>(key,data) {};
		int	compare(keytype key);
};

#include <rudiments/private/objectdictionarynodeinlines.h>

#include <rudiments/list.h>

template <class keytype, class datatype,
	class dictionarynodetype=primitivedictionarynode<keytype,datatype> >
class dictionary {
	public:
			dictionary();
			~dictionary();

		void	setData(keytype key, datatype data);
		int	getData(keytype key, datatype *data);
		int	removeData(keytype key);

		unsigned long	getLength() const;

		int	getKey(unsigned long index, keytype *key) const;
		int	getData(unsigned long index, datatype *data) const;

	#include <rudiments/private/dictionary.h>
};

#include <rudiments/private/dictionaryinlines.h>

template <class datatype>
class stringdictionary : public dictionary< char *, datatype,
				stringdictionarynode<datatype> > {};

template <class datatype>
class numericdictionary : public dictionary< long, datatype,
				primitivedictionarynode<long, datatype> > {};

class namevaluepairs : public stringdictionary< char * > {};

#endif
