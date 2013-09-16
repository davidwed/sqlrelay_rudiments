// Copyright (c) 2004 David Muse
// See the COPYING file for more information

//#define EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
#include <rudiments/client.h>

#include <rudiments/charstring.h>

// For NULL
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

class clientprivate {
	friend class client;
	private:
		char		*_connecterror;
		long		_timeoutsec;
		long		_timeoutusec;
		unsigned long	_retrywait;
		unsigned long	_retrycount;
};

client::client() : filedescriptor() {
	pvt=new clientprivate;
	initialize(NULL,-1,-1,0,0);
}

client::client(const client &c) : filedescriptor(c) {
	pvt=new clientprivate;
	clientClone(c);
}

client &client::operator=(const client &c) {
	if (this!=&c) {
		filedescriptor::operator=(c);
		delete[] pvt->_connecterror;
		clientClone(c);
	}
	return *this;
}

client::~client() {
	delete[] pvt->_connecterror;
	delete pvt;
}

void client::initialize(char *connecterror,
			long timeoutsec, long timeoutusec,
			unsigned long retrywait, unsigned long retrycount) {
	pvt->_connecterror=connecterror;
	pvt->_timeoutsec=timeoutsec;
	pvt->_timeoutusec=timeoutusec;
	pvt->_retrywait=retrywait;
	pvt->_retrycount=retrycount;
	type("client");
}

void client::clientClone(const client &c) {
	initialize(NULL,c.pvt->_timeoutsec,c.pvt->_timeoutusec,
				c.pvt->_retrywait,c.pvt->_retrycount);
	pvt->_connecterror=charstring::duplicate(c.pvt->_connecterror);
}

const char *client::getVerboseConnectError() {
	return pvt->_connecterror;
}

void client::setVerboseConnectError(const char *error) {
	delete[] pvt->_connecterror;
	pvt->_connecterror=charstring::duplicate(error);
}

char **client::_connecterror() {
	return &pvt->_connecterror;
}

long client::_timeoutsec() {
	return pvt->_timeoutsec;
}

long client::_timeoutusec() {
	return pvt->_timeoutusec;
}

unsigned long client::_retrywait() {
	return pvt->_retrywait;
}

unsigned long client::_retrycount() {
	return pvt->_retrycount;
}
