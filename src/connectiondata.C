// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/connectiondata.h>

#include <stdio.h>

void connectiondata::print() const {
	for (objectlistnode< dictionarynode<char *,char *> *, char * >
					*current=dict.getNodeByIndex(0);
		current;
		current=(objectlistnode<
				dictionarynode<char *,char *> *, char *> *)
					current->getNext()) {
		printf("%s=%s\n",current->getValue()->getKey(),
					current->getValue()->getValue());
	}
}
