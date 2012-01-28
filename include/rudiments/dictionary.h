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

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

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

		int32_t	compare(keytype testkey) const;
			// Returns -1,0 or 1 if the key stored in the
			// node is less than, equal to or greater than
			// "testkey".

		void	print() const;
			// Prints the key and data stored in the node.

	#include <rudiments/private/dictionarynode.h>
};

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

		void	print();
			// Prints out a representation of the dictionary.

	#include <rudiments/private/dictionary.h>
};



// A set of classes for storing dictionaries who's keys are strings are
// provided here for convenience.
template <class datatype>
class stringdictionarynode :
		public dictionarynode< char *,datatype > {
	public:
		virtual	~stringdictionarynode();
};

template <class datatype>
class stringdictionarylistnode :
		public dictionarylistnode< char *, datatype > {
	public:
		virtual	~stringdictionarylistnode();
};

template <class datatype>
class stringdictionarylist : public dictionarylist< char *, datatype > {
	public:
		virtual	~stringdictionarylist();
};

template <class datatype>
class stringdictionary : public dictionary< char *, datatype,
				stringdictionarynode<datatype>,
				stringdictionarylistnode<datatype>,
				stringdictionarylist<datatype> > {
	public:
		virtual	~stringdictionary();
};

// A set of classes for storing dictionaries who's keys are const strings are
// provided here for convenience.
template <class datatype>
class conststringdictionarynode :
		public dictionarynode< const char *,datatype > {
	public:
		virtual	~conststringdictionarynode();
};

template <class datatype>
class conststringdictionarylistnode :
		public dictionarylistnode< const char *, datatype > {
	public:
		virtual	~conststringdictionarylistnode();
};

template <class datatype>
class conststringdictionarylist :
		public dictionarylist< const char *, datatype > {
	public:
		virtual	~conststringdictionarylist();
};

template <class datatype>
class conststringdictionary : public dictionary< const char *, datatype,
				conststringdictionarynode<datatype>,
				conststringdictionarylistnode<datatype>,
				conststringdictionarylist<datatype> > {
	public:
		virtual	~conststringdictionary();
};



// A set of classes for storing dictionaries who's keys are int32_t integers are
// provided here for convenience.
template <class datatype>
class numericdictionarynode :
		public dictionarynode< int32_t, datatype > {
	public:
		virtual	~numericdictionarynode();
};

template <class datatype>
class numericdictionarylistnode :
		public dictionarylistnode< int32_t, datatype > {
	public:
		virtual	~numericdictionarylistnode();
};

template <class datatype>
class numericdictionarylist : public dictionarylist< int32_t, datatype > {
	public:
		virtual	~numericdictionarylist();
};

template <class datatype>
class numericdictionary : public dictionary< int32_t, datatype,
				numericdictionarynode<datatype>,
				numericdictionarylistnode<datatype>,
				numericdictionarylist<datatype> > {
	public:
		virtual	~numericdictionary();
};



// A set of classes for storing dictionaries who's keys and values are both
// strings are provided here for convenience.
typedef stringdictionarynode< char * >		namevaluepairsnode;
typedef stringdictionarylistnode< char * >	namevaluepairslistnode;
typedef stringdictionarylist< char * >		namevaluepairslist;
typedef stringdictionary< char * >		namevaluepairs;

typedef conststringdictionarynode< const char * >
				constnamevaluepairsnode;
typedef conststringdictionarylistnode< const char * >
				constnamevaluepairslistnode;
typedef conststringdictionarylist< const char * >
				constnamevaluepairslist;
typedef conststringdictionary< const char * >
				constnamevaluepairs;

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#include <rudiments/private/dictionarynodeinlines.h>
#include <rudiments/private/dictionaryinlines.h>

#endif
