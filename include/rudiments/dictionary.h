// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DICTIONARY_H
#define RUDIMENTS_DICTIONARY_H

#include <rudiments/private/dictionaryincludes.h>

// The dictionary class allows you to store arbitrary numbers of key/value
// pairs.  Since the dictionary class is template-based, you can store
// arbitrary types of keys and values.
//
// Each dictionary is composed of a list of dictionarynode's.  Each
// dictionarynode contains the key and value.


template <class keytype, class datatype>
class dictionarynode {
	public:
			dictionarynode();
			// Creates a new dictionary node, initializing the
			// key and data to 0.
		virtual	~dictionarynode();

		void	setKey(keytype key);
			// Sets the key stored in the node to "key".
		void	setData(datatype data);
			// Sets the data stored in the node to "data".

		keytype		getKey() const;
				// Returns the key stored in the node.
		datatype	getData() const;
				// Returns the data stored in the node.

		virtual	int	compare(keytype testkey) const;
				// Returns -1,0 or 1 if the key stored in the
				// node is less than, equal to or greater than
				// "testkey".

		virtual void	print() const;
				// Prints the key and data stored in the node.

	#include <rudiments/private/dictionarynode.h>
};

#ifndef DISABLE_RUDIMENTS_TEMPLATE_INLINES
	#include <rudiments/private/dictionarynodeinlines.h>
#endif

template <class keytype, class datatype>
class dictionarylistnode :
		public linkedlistnode< dictionarynode<keytype,datatype> * > {};

template <class keytype, class datatype>
class dictionarylist :
		public linkedlist< dictionarynode<keytype,datatype> *,
				dictionarylistnode<keytype,datatype> > {};

template <class keytype, class datatype,
	class dictionarynodetype=dictionarynode<keytype,datatype>,
	class dictionarylistnodetype=dictionarylistnode<keytype,datatype>,
	class dictionarylisttype=dictionarylist<keytype,datatype> >
class dictionary {
	public:
			dictionary();
			// Creates an empty dictionary
		virtual ~dictionary();
			// Deletes the dictionary and all of it's
			// dictionarynodes.

		void	setData(keytype key, datatype data);
			// Sets the data associated with "key" to "data".
			// If "key" already exists, the data currently
			// accociated with it is replaced with "data".
		bool	getData(keytype key, datatype *data);
			// Sets "data" to the data associated with "key".
			// Returns true on success or false if "key" wasn't
			// found.
		bool	removeData(keytype key);
			// Removes the dictionarynode with "key".
			// Returns true on success or false if "key" wasn't
			// found.

		dictionarylisttype	*getList();
					// Returns the list used internally.

		void	clear();
			// Deletes all dictionarynodes currently in the
			// dictionary.

		void	print() const;
			// Prints out a representation of the dictionary.

	#include <rudiments/private/dictionary.h>
};

#ifndef DISABLE_RUDIMENTS_TEMPLATE_INLINES
	#include <rudiments/private/dictionaryinlines.h>
#endif



// A set of classes for storing dictionaries who's keys are strings are
// provided here for convenience.
template <class datatype>
class stringdictionarynode :
		public dictionarynode< char *,datatype > {};

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



// A set of classes for storing dictionaries who's keys are long integers are
// provided here for convenience.
template <class datatype>
class numericdictionarynode :
		public dictionarynode< long, datatype > {};

template <class datatype>
class numericdictionarylistnode :
		public dictionarylistnode< long, datatype > {};

template <class datatype>
class numericdictionarylist : public dictionarylist< long, datatype > {};

template <class datatype>
class numericdictionary : public dictionary< long, datatype,
				numericdictionarynode<datatype>,
				numericdictionarylistnode<datatype>,
				numericdictionarylist<datatype> > {};



// A set of classes for storing dictionaries who's keys and values are both
// strings are provided here for convenience.
typedef stringdictionarynode< char * >		namevaluepairsnode;
typedef stringdictionarylistnode< char * >	namevaluepairslistnode;
typedef stringdictionarylist< char * >		namevaluepairslist;
typedef stringdictionary< char * >		namevaluepairs;

#endif
