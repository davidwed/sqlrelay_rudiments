// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/hostentry.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/hostentryinlines.h>
#endif

#include <stdio.h>
#include <errno.h>

#define MAXBUFFER	(32*1024)

int hostentry::initialize(const char *hostname) {
	if (he) {
		he=NULL;
		delete[] buffer;
	}
	#ifdef HAVE_GETHOSTBYNAME_R
		// gethostbyname_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		int	errnop;
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			if (gethostbyname_r(hostname,&hebuffer,
						buffer,size,&he,&errnop)==0) {
				return 1;
			}
			delete[] buffer;
			he=NULL;
			if (errnop!=ENOMEM) {
				return 0;
			}
		}
	#else
		he=NULL;
		return ((he=gethostbyname(hostname))!=NULL);
	#endif
}

int hostentry::initialize(const char *address, int len, int type) {
	if (he) {
		he=NULL;
		delete[] buffer;
	}
	#ifdef HAVE_GETHOSTBYADDR_R
		// gethostbyaddr_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		int	errnop;
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			if (gethostbyaddr_r(address,len,type,&hebuffer,
						buffer,size,&he,&errnop)==0) {
				return 1;
			}
			delete[] buffer;
			he=NULL;
			if (errnop!=ENOMEM) {
				return 0;
			}
		}
	#else
		he=NULL;
		return ((he=gethostbyaddr(address,len,type))!=NULL);
	#endif
}

char *hostentry::getAddressString(int index) const {
	char	*address=new char[(getAddressLength()*4)+1];
	address[0]=(char)NULL;
	for (int byte=0; byte<getAddressLength(); byte++) {
		sprintf(address,"%s%d",address,getAddressList()[index][byte]);
		if (byte<getAddressLength()-1) {
			sprintf(address,"%s.",address);
		}
	}
	return address;
}

void hostentry::print() const {

	if (!he) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Alias list:\n");
	for (int i=0; getAliasList()[i]; i++) {
		printf("	%s\n",getAliasList()[i]);
	}
	printf("Address type: %d\n",getAddressType());
	printf("Address length: %d\n",getAddressLength());
	printf("Address list:\n");
	for (int i=0; getAddressList()[i]; i++) {
		char	*addr=getAddressString(i);
		printf("	%s\n",addr);
		delete[] addr;
	}
}
