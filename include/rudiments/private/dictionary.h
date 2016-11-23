// Copyright (c) 2003 David Muse
// See the COPYING file for more information

	protected:
		avltreenode< dictionarynode<keytype,valuetype> *>
							*find(keytype key);

		avltree< dictionarynode<keytype,valuetype> *>		tree;
		linkedlist< dictionarynode<keytype,valuetype> *>	list;

		bool	trackinsertionorder;
