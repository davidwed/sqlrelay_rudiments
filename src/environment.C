// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/environment.h>
#ifndef ENABLE_RUDIMENTS_INLINES
	#define RUDIMENTS_INLINE
	#include <rudiments/private/environmentinlines.h>
#endif

#if defined(HAVE_PUTENV) && !defined(HAVE_SETENV)

#include <stdio.h>

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
	pestr=new char[strlen(variable)+strlen(value)+2];
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
