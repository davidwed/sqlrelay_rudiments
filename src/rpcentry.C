// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/rpcentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

// Some systems (notably cygwin 1.5.7-1) define getrpcbyname and getrpcbynumber
// in their header files but then either don't implement them or don't export
// the symbols.
#if (defined(HAVE_GETRPCBYNAME_R) || defined(HAVE_GETRPCBYNAME)) \
	&& (defined(HAVE_GETRPCBYNUMBER_R) || defined(HAVE_GETRPCBYNUMBER)) \

#include <stdio.h>
#include <stdlib.h>

#define MAXBUFFER	(32*1024)

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class rpcentryprivate {
	friend class rpcentry;
	private:
		rpcent	*_re;
		#if defined(HAVE_GETRPCBYNAME_R) && \
				defined(HAVE_GETRPCBYNUMBER_R)
			rpcent	_rebuffer;
			char	*_buffer;
		#endif
};

// LAME: not in the class
#if defined(RUDIMENTS_HAS_THREADS) && \
	(!defined(HAVE_GETRPCBYNAME_R) || !defined(HAVE_GETRPCBYNUMBER_R))
static mutex	*_remutex;
#endif


rpcentry::rpcentry() {
	pvt=new rpcentryprivate;
	pvt->_re=NULL;
	#if defined(HAVE_GETRPCBYNAME_R) && defined(HAVE_GETRPCBYNUMBER_R)
		rawbuffer::zero(&pvt->_rebuffer,sizeof(pvt->_rebuffer));
		pvt->_buffer=NULL;
	#endif
}

rpcentry::~rpcentry() {
	#if defined(HAVE_GETRPCBYNAME_R) && defined(HAVE_GETRPCBYNUMBER_R)
		delete[] pvt->_buffer;
	#endif
	delete pvt;
}

const char *rpcentry::getName() const {
	return pvt->_re->r_name;
}

int rpcentry::getNumber() const {
	return pvt->_re->r_number;
}

const char * const *rpcentry::getAliasList() const {
	return pvt->_re->r_aliases;
}

#ifdef RUDIMENTS_HAS_THREADS
bool rpcentry::needsMutex() {
	#if !defined(HAVE_GETRPCBYNAME_R) || !defined(HAVE_GETRPCBYNUMBER_R)
		return true;
	#else
		return false;
	#endif
}

void rpcentry::setMutex(mutex *mtx) {
	#if !defined(HAVE_GETRPCBYNAME_R) || !defined(HAVE_GETRPCBYNUMBER_R)
		_remutex=mtx;
	#endif
}
#endif

bool rpcentry::initialize(const char *name) {
	return initialize(name,0);
}

bool rpcentry::initialize(int number) {
	return initialize(NULL,number);
}

bool rpcentry::initialize(const char *rpcname, int number) {

	#if defined(HAVE_GETRPCBYNAME_R) && defined(HAVE_GETRPCBYNUMBER_R)
		if (pvt->_re) {
			pvt->_re=NULL;
			delete[] pvt->_buffer;
			pvt->_buffer=NULL;
		}
		// getrpcbyname_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			pvt->_buffer=new char[size];
			#if defined(HAVE_GETRPCBYNAME_R_5) && \
				defined(HAVE_GETRPCBYNUMBER_R_5)
			if (!((rpcname)
				?(getrpcbyname_r(rpcname,&pvt->_rebuffer,
							pvt->_buffer,size,
							&pvt->_re))
				:(getrpcbynumber_r(number,&pvt->_rebuffer,
							pvt->_buffer,size,
							&pvt->_re)))) {
				return (pvt->_re!=NULL);
			}
			#elif defined(HAVE_GETRPCBYNAME_R_4) && \
				defined(HAVE_GETRPCBYNUMBER_R_4)
			if ((rpcname)
				?(pvt->_re=getrpcbyname_r(rpcname,
							&pvt->_rebuffer,
							pvt->_buffer,size))
				:(pvt->_re=getrpcbynumber_r(number,
							&pvt->_rebuffer,
							pvt->_buffer,size))) {
				return true;
			}
			#endif
			delete[] pvt->_buffer;
			pvt->_buffer=NULL;
			pvt->_re=NULL;
			if (error::getErrorNumber()!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		pvt->_re=NULL;
		#ifdef RUDIMENTS_HAS_THREADS
		return (!(remutex && !remutex->lock()) &&
			((pvt->_re=((rpcname)
				?getrpcbyname(const_cast<char *>(rpcname))
				:getrpcbynumber(number)))!=NULL) &&
			!(remutex && !remutex->unlock()));
		#else
		return ((pvt->_re=((rpcname)
				?getrpcbyname(const_cast<char *>(rpcname))
				:getrpcbynumber(number)))!=NULL);
		#endif
	#endif
}

bool rpcentry::getAliasList(const char *name, char ***aliaslist) {
	rpcentry	re;
	if (re.initialize(name)) {
		int	counter;
		for (counter=0; re.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int i=0; i<counter; i++) {
			alias[i]=charstring::duplicate(re.getAliasList()[i]);
		}
		*aliaslist=alias;
		return true;
	}
	return false;
}

bool rpcentry::getName(int number, char **name) {
	rpcentry	re;
	if (re.initialize(number)) {
		*name=charstring::duplicate(re.getName());
		return true;
	}
	return false;
}

bool rpcentry::getAliasList(int number, char ***aliaslist) {
	rpcentry	re;
	if (re.initialize(number)) {
		int	counter;
		for (counter=0; re.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int i=0; i<counter; i++) {
			alias[i]=charstring::duplicate(re.getAliasList()[i]);
		}
		*aliaslist=alias;
		return true;
	}
	return false;
}

void rpcentry::print() const {

	if (!pvt->_re) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Number: %d\n",getNumber());
	printf("Alias list:\n");
	for (int i=0; getAliasList()[i]; i++) {
		printf("	%s\n",getAliasList()[i]);
	}
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
