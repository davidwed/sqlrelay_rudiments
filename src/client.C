// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#define EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
#include <rudiments/client.h>

client::client() : filedescriptor() {
	connecterror=NULL;
}

client::~client() {
	delete[] connecterror;
}

char *client::getVerboseConnectError() {
	return connecterror;
}
