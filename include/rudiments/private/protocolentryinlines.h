// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

inline protocolentry::protocolentry() {
	pe=NULL;
	buffer=NULL;
}

inline protocolentry::~protocolentry() {
	delete pe;
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
