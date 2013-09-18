// Copyright (c) 2003 David Muse
// See the COPYING file for more information

	protected:
		linkedlistnode< dictionarynode<keytype,datatype> *>
						*findNode(keytype key);
		linkedlist< dictionarynode<keytype,datatype> *>	dict;
