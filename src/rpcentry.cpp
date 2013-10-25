// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/rpcentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

// for rpcent, functions
#ifdef RUDIMENTS_HAVE_NETDB_H
	#include <netdb.h>
#endif
#ifdef RUDIMENTS_HAVE_RPCENT_H
	#include <rpc/rpcent.h>
#endif
#ifdef RUDIMENTS_HAVE_RPC_H
	#include <rpc/rpc.h>
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#define MAXBUFFER	(32*1024)

#if (defined(RUDIMENTS_HAVE_GETRPCBYNAME) && \
	defined(RUDIMENTS_HAVE_GETRPCBYNUMBER)) || \
	(defined(RUDIMENTS_HAVE_GETRPCBYNAME_R) && \
		defined(RUDIMENTS_HAVE_GETRPCBYNUMBER_R))
	#define RUDIMENTS_HAVE_RPC
#endif

#ifdef RUDIMENTS_HAVE_RPC
class rpcentryprivate {
	friend class rpcentry;
	private:
		rpcent	*_re;
		#if defined(RUDIMENTS_HAVE_GETRPCBYNAME_R) && \
			defined(RUDIMENTS_HAVE_GETRPCBYNUMBER_R)
			rpcent	_rebuffer;
			char	*_buffer;
		#endif
};
#endif

// LAME: not in the class
#if defined(RUDIMENTS_HAVE_RPC) && \
	(!defined(RUDIMENTS_HAVE_GETRPCBYNAME_R) || \
		!defined(RUDIMENTS_HAVE_GETRPCBYNUMBER_R))
static threadmutex	*_remutex;
#endif


rpcentry::rpcentry() {
	#ifdef RUDIMENTS_HAVE_RPC
		pvt=new rpcentryprivate;
		pvt->_re=NULL;
		#if defined(RUDIMENTS_HAVE_GETRPCBYNAME_R) && \
			defined(RUDIMENTS_HAVE_GETRPCBYNUMBER_R)
			rawbuffer::zero(&pvt->_rebuffer,sizeof(pvt->_rebuffer));
			pvt->_buffer=NULL;
		#endif
	#endif
}

rpcentry::~rpcentry() {
	#ifdef RUDIMENTS_HAVE_RPC
		#if defined(RUDIMENTS_HAVE_GETRPCBYNAME_R) && \
			defined(RUDIMENTS_HAVE_GETRPCBYNUMBER_R)
			delete[] pvt->_buffer;
		#endif
		delete pvt;
	#endif
}

const char *rpcentry::getName() const {
	#ifdef RUDIMENTS_HAVE_RPC
		return pvt->_re->r_name;
	#else
		return NULL;
	#endif
}

int32_t rpcentry::getNumber() const {
	#ifdef RUDIMENTS_HAVE_RPC
		return pvt->_re->r_number;
	#else
		return -1;
	#endif
}

const char * const *rpcentry::getAliasList() const {
	#ifdef RUDIMENTS_HAVE_RPC
		return pvt->_re->r_aliases;
	#else
		return NULL;
	#endif
}

bool rpcentry::needsMutex() {
	#if !defined(RUDIMENTS_HAVE_GETRPCBYNAME_R) || \
		!defined(RUDIMENTS_HAVE_GETRPCBYNUMBER_R)
		return true;
	#else
		return false;
	#endif
}

void rpcentry::setMutex(threadmutex *mtx) {
	#if defined(RUDIMENTS_HAVE_RPC) && \
		!defined(RUDIMENTS_HAVE_GETRPCBYNAME_R) && \
		!defined(RUDIMENTS_HAVE_GETRPCBYNUMBER_R)
		_remutex=mtx;
	#endif
}

bool rpcentry::initialize(const char *name) {
	return initialize(name,0);
}

bool rpcentry::initialize(int32_t number) {
	return initialize(NULL,number);
}

bool rpcentry::initialize(const char *rpcname, int32_t number) {

	#if defined(RUDIMENTS_HAVE_GETRPCBYNAME_R) && \
		defined(RUDIMENTS_HAVE_GETRPCBYNUMBER_R)
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
		for (int32_t size=1024; size<MAXBUFFER; size=size+1024) {
			pvt->_buffer=new char[size];
			#if defined(RUDIMENTS_HAVE_GETRPCBYNAME_R_5) && \
				defined(RUDIMENTS_HAVE_GETRPCBYNUMBER_R_5)
			if (!((rpcname)
				?(getrpcbyname_r(rpcname,&pvt->_rebuffer,
							pvt->_buffer,size,
							&pvt->_re))
				:(getrpcbynumber_r(number,&pvt->_rebuffer,
							pvt->_buffer,size,
							&pvt->_re)))) {
				return (pvt->_re!=NULL);
			}
			#elif defined(RUDIMENTS_HAVE_GETRPCBYNAME_R_4) && \
				defined(RUDIMENTS_HAVE_GETRPCBYNUMBER_R_4)
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
	#elif defined(RUDIMENTS_HAVE_GETRPCBYNAME) && \
		defined(RUDIMENTS_HAVE_GETRPCBYNUMBER)
		pvt->_re=NULL;
		return (!(remutex && !remutex->lock()) &&
			((pvt->_re=((rpcname)
				?getrpcbyname(const_cast<char *>(rpcname))
				:getrpcbynumber(number)))!=NULL) &&
			!(remutex && !remutex->unlock()));
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool rpcentry::getNumber(const char *name, int32_t *number) {
	#ifdef RUDIMENTS_HAVE_RPC
		rpcentry	re;
		if (re.initialize(name)) {
			*number=re.getNumber();
			return true;
		}
	#endif
	return false;
}

bool rpcentry::getName(int32_t number, char **name) {
	#ifdef RUDIMENTS_HAVE_RPC
		rpcentry	re;
		if (re.initialize(number)) {
			*name=charstring::duplicate(re.getName());
			return true;
		}
	#endif
	return false;
}
