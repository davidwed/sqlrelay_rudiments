// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/hostentry.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/hostentryinlines.h>
#endif

#include <stdio.h>

int hostentry::initialize(const char *hostname) {
	if (he) {
		delete he;
	}
	#ifdef HAVE_GETHOSTBYNAME_R
		he=new hostent;
		int	errnop;
		return !gethostbyname_r(hostname,he,buffer,1024,&he,&errnop);
	#else
		he=NULL;
		return ((he=gethostbyname(hostname))!=NULL);
	#endif
}

int hostentry::initialize(const char *address, int len, int type) {
	if (he) {
		delete he;
	}
	#ifdef HAVE_GETHOSTBYADDR_R
		he=new hostent;
		int	errnop;
		return !gethostbyaddr_r(address,len,type,
					he,buffer,1024,&he,&errnop);
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
