// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRING_H
	#include <strings.h>
#endif

inline protocolentry::protocolentry() {
	pe=NULL;
	buffer=NULL;
}

inline protocolentry::~protocolentry() {
	delete[] buffer;
}

inline char *protocolentry::getName() const {
	return pe->p_name;
}

inline char **protocolentry::getAliasList() const {
	return pe->p_aliases;
}

inline int protocolentry::getNumber() const {
	return pe->p_proto;
}

inline int protocolentry::getAliasList(const char *protocolname,
						char ***aliaslist) {
	protocolentry	pe;
	if (pe.initialize(protocolname)) {
		int	counter;
		for (counter=0; pe.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int i=0; i<counter; i++) {
			alias[i]=strdup(pe.getAliasList()[i]);
		}
		*aliaslist=alias;
		return 1;
	}
	return 0;
}

inline int protocolentry::getNumber(const char *protocolname, int *number) {
	protocolentry	pe;
	if (pe.initialize(protocolname)) {
		*number=pe.getNumber();
		return 1;
	}
	return 0;
}

inline int protocolentry::getName(int number, char **name) {
	protocolentry	pe;
	if (pe.initialize(number)) {
		*name=strdup(pe.getName());
		return 1;
	}
	return 0;
}

inline int protocolentry::getAliasList(int number, char ***aliaslist) {
	protocolentry	pe;
	if (pe.initialize(number)) {
		int	counter;
		for (counter=0; pe.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int i=0; i<counter; i++) {
			alias[i]=strdup(pe.getAliasList()[i]);
		}
		*aliaslist=alias;
		return 1;
	}
	return 0;
}
