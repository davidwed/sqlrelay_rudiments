// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/hostentry.h>
#ifndef ENABLE_INLINES
	#include <rudiments/private/hostentryinlines.h>
#endif

#include <stdio.h>
#include <errno.h>

#define MAXBUFFER	(32*1024)

#if defined(__GNUC__) && \
	(!defined(HAVE_GETHOSTBYNAME_R) || !defined(HAVE_GETHOSTBYADDR_R))
pthread_mutex_t	*hostentry::hemutex;
#endif

int hostentry::initialize(const char *hostname, const char *address,
							int len, int type) {
	if (he) {
		he=NULL;
		delete[] buffer;
		buffer=NULL;
	}
	#if defined(HAVE_GETHOSTBYNAME_R) && defined(HAVE_GETHOSTBYADDR_R)
		// gethostbyname_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		int	errnop=0;
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			if (!((hostname)
				?(gethostbyname_r(hostname,&hebuffer,
						buffer,size,&he,&errnop))
				:(gethostbyaddr_r(address,len,type,&hebuffer,
						buffer,size,&he,&errnop)))) {
				return he!=NULL;
			}
			delete[] buffer;
			buffer=NULL;
			he=NULL;
			if (errnop!=ENOMEM) {
				return 0;
			}
		}
		return 0;
	#else
		he=NULL;
		return (((hemutex)?!pthread_mutex_lock(hemutex):1) &&
			((he=((hostname)
				?gethostbyname(hostname)
				:gethostbyaddr(address,len,type)))!=NULL) &&
			((hemutex)?!pthread_mutex_unlock(hemutex):1));
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
