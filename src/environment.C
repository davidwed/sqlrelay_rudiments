// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/environment.h>
#include <rudiments/charstring.h>

#if defined(HAVE_PUTENV) && !defined(HAVE_SETENV)

#include <stdio.h>
#include <stdlib.h>

environment::~environment() {
	for (namevaluepairslistnode *node=
			(namevaluepairslistnode *)
				envstrings.getList()->getNodeByIndex(0);
			node;
			node=(namevaluepairslistnode *)node->getNext()) {
		delete[] node->getData()->getData();
	}
}

bool environment::setValue(const char *variable, const char *value) {
	char	*pestr;
	if (envstrings.getData((char *)variable,&pestr)) {
		delete[] pestr;
	}
	pestr=new char[charstring::length(variable)+
			charstring::length(value)+2];
	sprintf(pestr,"%s=%s",variable,value);
	if (putenv(pestr)!=-1) {
		envstrings.setData((char *)variable,pestr);
		return true;
	} else {
		delete[] pestr;
		envstrings.removeData((char *)variable);
		return false;
	}
}
#endif


#ifdef HAVE_SETENV
environment::~environment() {
}
#endif

char *environment::getValue(const char *variable) const {
	return getenv(variable);
}

#ifdef HAVE_SETENV
bool environment::setValue(const char *variable, const char *value) {
	return (setenv(variable,value,1)!=-1);
}
#endif

void environment::remove(const char *variable) {
#ifdef HAVE_UNSETENV
	unsetenv(variable);
#else
	// I know this isn't the same as calling unsetenv, but as far as I
	// know, it's all that can be done.
	setValue(variable,"");
#endif
}
