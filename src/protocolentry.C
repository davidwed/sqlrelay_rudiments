// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/protocolentry.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/protocolentryinlines.h>
#endif

#include <stdio.h>

int protocolentry::initialize(const char *protocolname) {
	if (pe) {
		delete pe;
	}
	#ifdef HAVE_GETPROTOBYNAME_R
		pe=new protoent;
		return !getprotobyname_r(protocolname,pe,buffer,1024,&pe);
	#else
		pe=NULL;
		return ((pe=getprotobyname(protocolname))!=NULL);
	#endif
}

int protocolentry::initialize(int number) {
	if (pe) {
		delete pe;
	}
	#ifdef HAVE_GETPROTOBYNUMBER_R
		pe=new protoent;
		return !getprotobynumber_r(number,pe,buffer,1024,&pe);
	#else
		pe=NULL;
		return ((pe=getprotobynumber(number))!=NULL);
	#endif
}

void protocolentry::print() const {

	if (!pe) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Alias list:\n");
	for (int i=0; getAliasList()[i]; i++) {
		printf("	%s\n",getAliasList()[i]);
	}
	printf("Number: %d\n",getNumber());
}
