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
		#ifdef INCLUDE_INETSERVER
			return new inetserversocket();
		#else
			return NULL;
		#endif
	} else if (!charstring::compare(type,"unix")) {
		#ifdef INCLUDE_UNIXSERVER
			return new unixserversocket();
		#else
			return NULL;
		#endif
	} else if (!charstring::compare(type,"modem")) {
		#ifdef INCLUDE_MODEMSERVER
			return new modemserver();
		#else
			return NULL;
		#endif
	}
	return NULL;
}

client *clientserverfactory::getClient(const char *type) {
	if (!charstring::compare(type,"inet")) {
		#ifdef INCLUDE_INETCLIENT
			return new inetclientsocket();
		#else
			return NULL;
		#endif
	} else if (!charstring::compare(type,"unix")) {
		#ifdef INCLUDE_UNIXCLIENT
			return new unixclientsocket();
		#else
			return NULL;
		#endif
	} else if (!charstring::compare(type,"modem")) {
		#ifdef INCLUDE_MODEMCLIENT
			return new modemclient();
		#else
			return NULL;
		#endif
	}
	return NULL;
}
