// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/protocolentry.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/protocolentryinlines.h>
#endif

#include <stdio.h>
#include <errno.h>

#define MAXBUFFER	(32*1024)

int protocolentry::initialize(const char *protocolname) {
	if (pe) {
		pe=NULL;
		delete[] buffer;
		buffer=NULL;
	}
	#ifdef HAVE_GETPROTOBYNAME_R
		// getprotobyname_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			if (!getprotobyname_r(protocolname,&pebuffer,
							buffer,size,&pe)) {
				return pe!=NULL;
			}
			delete[] buffer;
			buffer=NULL;
			pe=NULL;
			if (errno!=ENOMEM) {
				return 0;
			}
		}
		return 0;
	#else
		pe=NULL;
		return ((pe=getprotobyname(protocolname))!=NULL);
	#endif
}

int protocolentry::initialize(int number) {
	if (pe) {
		pe=NULL;
		delete[] buffer;
		buffer=NULL;
	}
	#ifdef HAVE_GETPROTOBYNUMBER_R
		// getprotobynumber_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			if (!getprotobynumber_r(number,&pebuffer,
							buffer,size,&pe)) {
				return pe!=NULL;
			}
			delete[] buffer;
			buffer=NULL;
			pe=NULL;
			if (errno!=ENOMEM) {
				return 0;
			}
		}
		return 0;
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
