// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#define EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
#include <rudiments/environment.h>
#include <rudiments/charstring.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_ENVIRON
	#include <unistd.h>
#else
	extern	char	**environ;
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#if defined(HAVE_PUTENV) && !defined(HAVE_SETENV)

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
	if (envstrings.getData(const_cast<char *>(variable),&pestr)) {
		delete[] pestr;
	}
	pestr=new char[charstring::length(variable)+
			charstring::length(value)+2];
	sprintf(pestr,"%s=%s",variable,value);
	if (putenv(pestr)!=-1) {
		envstrings.setData(const_cast<char *>(variable),pestr);
		return true;
	} else {
		delete[] pestr;
		envstrings.removeData(const_cast<char *>(variable));
		return false;
	}
}
#endif


#ifdef HAVE_SETENV
environment::~environment() {
}
#endif

const char *environment::getValue(const char *variable) const {
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

const char * const *environment::variables() {
	return environ;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
