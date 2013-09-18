// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DICTIONARY_H
#define RUDIMENTS_DICTIONARY_H

#include <rudiments/private/dictionaryincludes.h>

/** The dictionarynode class stores the key/value pairs that compose a
 *  dictionary. */
template <class keytype, class datatype>
class dictionarynode {
	public:
		/** Creates an empty instance of the dictionary node class. */
		dictionarynode();

		/** Deletes this instance of the dictionarynode class.
		 *  Note however, that neither the key nor data stored in the
		 *  dictionarynode are deleted by this call. */
		virtual	~dictionarynode();

		/** Sets the key stored in the node to "key". */
		void	setKey(keytype key);

		/** Sets the data stored in the node to "data". */
		void	setData(datatype data);

		/** Returns the key stored in the node. */
		keytype		getKey() const;

		/** Returns the data stored in the node. */
		datatype	getData() const;

		/** Returns a negative number,0 or a positive number depending
		 *  on whether the key stored in the node is respectively
		 *  less than, equal to or greater than "testkey". */
		int32_t	compare(keytype testkey) const;

		/** Prints a representation of the key and
		 *  data stored in the node. */
		void	print() const;

	#include <rudiments/private/dictionarynode.h>
};

/** The dictionary class allows you to store arbitrary numbers of key/value
 *  pairs.
 * 
 *  Each dictionary is composed of a list of dictionarynode's.  Each
 *  dictionarynode contains the key and value.
 *
 *  Internally, the dictionary class uses a linkedlist to store the values
 *  though this is potentially inefficient and may change in a future
 *  version. */
template <class keytype, class datatype>
class dictionary {
	public:
		/** Creates an empty instance of the dictionary class. */
		dictionary();

		/** Deletes this instance of the dictionary class and all
		 *  of its dictionarynodes.  Note however, that neither the
		 *  key nor data stored in each dictionarynode are deleted
		 *  by this call. */
		virtual ~dictionary();

		/** Sets the data associated with "key" to "data".
		 *  If "key" already exists, the data currently
		 *  accociated with it is replaced with "data". */
		void	setData(keytype key, datatype data);

		/** Sets "data" to the data associated with "key".
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	getData(keytype key, datatype *data);

		/** Removes the dictionarynode with "key".
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	removeData(keytype key);

		/** Returns the list used internally. */
		linkedlist< dictionarynode<keytype,datatype> *>	*getList();

		/** Deletes all dictionarynodes currently in the dictionary. */
		void	clear();

		/** Prints out a representation of the dictionary. */
		void	print();

	#include <rudiments/private/dictionary.h>
};

typedef dictionarynode< char *, char * >	namevaluepairsnode;
typedef dictionary< char *, char * >		namevaluepairs;

typedef dictionarynode< const char *, const char * >	constnamevaluepairsnode;
typedef dictionary< const char *, const char * >	constnamevaluepairs;

#include <rudiments/private/dictionarynodeinlines.h>
#include <rudiments/private/dictionaryinlines.h>

#endif
