// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/connectiondata.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/connectiondatainlines.h>
#endif
	

#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

connectiondata::~connectiondata() {
	connectiondatanode	*current=first;
	while(current) {
		last=current->next;
		delete current;
		current=last;
	}
}

void connectiondata::setValue(const char *key, const char *value) {

	if (!first) {
		first=new connectiondatanode(key,value);
		last=first;
	} else {
		connectiondatanode	*current=findNode(key);
		if (current) {
			current->value=(char *)value;
			return;
		}
		last->next=new connectiondatanode(key,value);
		last=last->next;
	}
	keycount++;
}

void connectiondata::removeValue(const char *key) {

	connectiondatanode	*current=findNode(key);
	if (current) {
		if (current->next) {
			current->next->previous=current->previous;
		}
		if (current->previous) {
			current->previous->next=current->next;
		}
		delete current;
		keycount--;
	}
}

connectiondatanode *connectiondata::findNode(int index) const {
	connectiondatanode	*current=first;
	for (int i=0; i<index && current; i++) {
		current=current->next;
	}
	return current;
}

connectiondatanode *connectiondata::findNode(const char *key) const {
	connectiondatanode	*current=first;
	while(current) {
		if (!strcmp(key,current->key)) {
			return current;
		} else {
			current=current->next;
		}
	}
	return NULL;
}
