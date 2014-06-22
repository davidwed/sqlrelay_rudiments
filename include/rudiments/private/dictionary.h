// Copyright (c) 2003 David Muse
// See the COPYING file for more information

	protected:
		linkedlistnode< dictionarynode<keytype,valuetype> *>
							*find(keytype key);
		linkedlist< dictionarynode<keytype,valuetype> *>	dict;
