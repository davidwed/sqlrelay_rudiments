// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DICTIONARY_H
#define RUDIMENTS_DICTIONARY_H

#include <rudiments/private/config.h>

template <class keytype, class valuetype>
class dictionarynode {
	public:
			dictionarynode();
			dictionarynode(keytype key, valuetype value);

		void	setKey(keytype key);
		void	setValue(valuetype value);

		keytype		getKey() const;
		valuetype	getValue() const;

		virtual	int	compare(keytype key)=0;

	#include <rudiments/private/dictionarynode.h>
};

#include <rudiments/private/dictionarynodeinlines.h>

template <class keytype, class valuetype>
class primitivedictionarynode : public dictionarynode<keytype,valuetype> {
	public:
			primitivedictionarynode() :
				dictionarynode<keytype,valuetype>() {};
			primitivedictionarynode(keytype key, valuetype value) :
				dictionarynode<keytype,valuetype>(key,value) {};
		int	compare(keytype key);
};

#include <rudiments/private/primitivedictionarynodeinlines.h>

template <class valuetype>
class stringdictionarynode : public dictionarynode<char *,valuetype> {
	public:
			stringdictionarynode() :
				dictionarynode<char *,valuetype>() {};
			stringdictionarynode(char *key, valuetype value) :
				dictionarynode<char *,valuetype>(key,value) {};
		int	compare(char * key);
};

#include <rudiments/private/stringdictionarynodeinlines.h>

template <class keytype, class valuetype>
class objectdictionarynode : public dictionarynode<keytype,valuetype> {
	public:
			objectdictionarynode() :
				dictionarynode<keytype,valuetype>() {};
			objectdictionarynode(keytype key, valuetype value) :
				dictionarynode<keytype,valuetype>(key,value) {};
		int	compare(keytype key);
};

#include <rudiments/private/objectdictionarynodeinlines.h>

#include <rudiments/list.h>

template <class keytype, class valuetype,
	class dictionarynodetype=primitivedictionarynode<keytype,valuetype> >
class dictionary {
	public:
			dictionary();
			~dictionary();

		void	setValue(keytype key, valuetype value);
		int	getValue(keytype key, valuetype *value);
		int	removeValue(keytype key);

		unsigned long	getLength() const;

		int	getKey(unsigned long index, keytype *key) const;
		int	getValue(unsigned long index, valuetype *value) const;

	#include <rudiments/private/dictionary.h>
};

#include <rudiments/private/dictionaryinlines.h>

template <class valuetype>
class stringdictionary : public dictionary< char *, valuetype,
				stringdictionarynode<valuetype> > {};

template <class valuetype>
class numericdictionary : public dictionary< long, valuetype,
				primitivedictionarynode<long, valuetype> > {};

#endif
