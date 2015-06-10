// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DICTIONARY_H
#define RUDIMENTS_DICTIONARY_H

#include <rudiments/private/dictionaryincludes.h>

/** The dictionarynode class stores the key/value pairs that compose a
 *  dictionary. */
template <class keytype, class valuetype>
class dictionarynode {
	public:
		/** Creates an instance of the dictionary node class 
		 *  with key "key" and value "value". */
		dictionarynode(keytype key, valuetype value);

		/** Deletes this instance of the dictionarynode class.
		 *  Note however, that neither the key nor value stored in the
		 *  dictionarynode are deleted by this call. */
		virtual	~dictionarynode();

		/** Sets the key stored in the node to "key". */
		void	setKey(keytype key);

		/** Sets the value stored in the node to "value". */
		void	setValue(valuetype value);

		/** Returns the key stored in the node. */
		keytype		getKey() const;

		/** Returns the value stored in the node. */
		valuetype	getValue() const;

		/** Returns a negative number,0 or a positive number depending
		 *  on whether the key stored in the node is respectively
		 *  less than, equal to or greater than "testkey". */
		int32_t	compare(keytype testkey) const;

		/** Prints a representation of the key and
		 *  value stored in the node. */
		void	print() const;

	#include <rudiments/private/dictionarynode.h>
};

/** The dictionary class allows you to store arbitrary numbers of key/value
 *  pairs.
 * 
 *  Each dictionary is composed of a set of dictionarynodes.  Each
 *  dictionarynode contains the key and value.
 *
 *  Internally, the dictionary class uses a linkedlist to store the values
 *  though this is potentially inefficient though, and may change in a future
 *  version. */
template <class keytype, class valuetype>
class dictionary {
	public:
		/** Creates an empty instance of the dictionary class. */
		dictionary();

		/** Deletes this instance of the dictionary class and all
		 *  of its dictionarynodes.  Note however, that neither the
		 *  key nor value stored in each dictionarynode are deleted
		 *  by this call. */
		virtual ~dictionary();

		/** Sets the value associated with "key" to "value".
		 *  If "key" already exists, the value currently
		 *  accociated with it is replaced with "value". */
		void	setValue(keytype key, valuetype value);

		/** Sets "value" to the value associated with "key".
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	getValue(keytype key, valuetype *value);

		/** Returns the node associated with "key" or NULL
		 *  if "key" wasn't found. */
		dictionarynode<keytype,valuetype> *getNode(keytype key);

		/** Returns the value associated with "key" or NULL
		 *  if "key" wasn't found.  Note that there is no
		 *  way to distinguish between failure to find "key"
		 *  and a valid value of NULL associated with "key". */
		valuetype getValue(keytype key);

		/** Removes the dictionarynode associated with "key".
		 *  Returns true on success or false if "key" wasn't
		 *  found. */
		bool	remove(keytype key);

		/** Detaches the dictionarynode associated with "key".
		 *  Returns the node on success or NULL if "key" wasn't
		 *  found. */
		dictionarynode<keytype,valuetype> *detach(keytype key);

		/** Returns a list of the keys in the dictionary.
		 *
		 *  Note that the linkedlist returned is allocated internally
		 *  and returned.  The calling program must delete the
		 *  linkedlist. */
		linkedlist< keytype > *getKeys();

		/** Returns the list used internally. */
		linkedlist< dictionarynode<keytype,valuetype> *> *getList();

		/** Deletes all dictionarynodes currently in the dictionary. */
		void	clear();

		/** Prints out a representation of the dictionary. */
		void	print();

	#include <rudiments/private/dictionary.h>
};


// ideally I'd use typdefs for these but older compilers can't handle them
#define namevaluepairsnode	dictionarynode< char *, char * >
#define namevaluepairs		dictionary< char *, char * >
#define constnamevaluepairsnode	dictionarynode< const char *, const char * >
#define constnamevaluepairs	dictionary< const char *, const char * >

#include <rudiments/private/dictionaryinlines.h>

#endif
