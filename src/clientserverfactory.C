// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/clientserverfactory.h>
#include <rudiments/inetserversocket.h>
#include <rudiments/inetclientsocket.h>
#include <rudiments/unixserversocket.h>
#include <rudiments/unixclientsocket.h>
#include <rudiments/modemclient.h>
#include <rudiments/modemserver.h>
#include <rudiments/charstring.h>

server *clientserverfactory::getServer(const char *type) {
	if (!charstring::compare(type,"inet")) {
		return new inetserversocket();
	} else if (!charstring::compare(type,"unix")) {
		return new unixserversocket();
	} else if (!charstring::compare(type,"modem")) {
		return new modemserver();
	}
	return NULL;
}

client *clientserverfactory::getClient(const char *type) {
	if (!charstring::compare(type,"inet")) {
		return new inetclientsocket();
	} else if (!charstring::compare(type,"unix")) {
		return new unixclientsocket();
	} else if (!charstring::compare(type,"modem")) {
		return new modemclient();
	}
	return NULL;
}
