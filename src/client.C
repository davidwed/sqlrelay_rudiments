// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#define EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
#include <rudiments/client.h>

#include <rudiments/charstring.h>

// For NULL
#include <stdlib.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

client::client() : filedescriptor() {
	clientInit(NULL,-1,-1,0,0);
}

client::client(const client &c) : filedescriptor(c) {
	clientClone(c);
}

client &client::operator=(const client &c) {
	if (this!=&c) {
		filedescriptor::operator=(c);
		delete[] connecterror;
		clientClone(c);
	}
	return *this;
}

client::~client() {
	delete[] connecterror;
}

void client::clientInit(char *connecterror,
			long timeoutsec, long timeoutusec,
			unsigned long retrywait, unsigned long retrycount) {
	this->connecterror=connecterror;
	this->timeoutsec=timeoutsec;
	this->timeoutusec=timeoutusec;
	this->retrywait=retrywait;
	this->retrycount=retrycount;
}

void client::clientClone(const client &c) {
	clientInit(NULL,c.timeoutsec,c.timeoutusec,c.retrywait,c.retrycount);
	connecterror=charstring::duplicate(c.connecterror);
}

char *client::getVerboseConnectError() {
	return connecterror;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
