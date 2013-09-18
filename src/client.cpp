// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/client.h>

#include <rudiments/charstring.h>

class clientprivate {
	friend class client;
	private:
		char		*_connecterror;
		int32_t		_timeoutsec;
		int32_t		_timeoutusec;
		uint32_t	_retrywait;
		uint32_t	_retrycount;
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
			int32_t timeoutsec, int32_t timeoutusec,
			uint32_t retrywait, uint32_t retrycount) {
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

int32_t client::_timeoutsec() {
	return pvt->_timeoutsec;
}

int32_t client::_timeoutusec() {
	return pvt->_timeoutusec;
}

uint32_t client::_retrywait() {
	return pvt->_retrywait;
}

uint32_t client::_retrycount() {
	return pvt->_retrycount;
}
