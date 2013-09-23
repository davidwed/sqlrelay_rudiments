// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/clientserverfactory.h>
#include <rudiments/inetsocketserver.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/unixsocketserver.h>
#include <rudiments/unixsocketclient.h>
#include <rudiments/modemclient.h>
#include <rudiments/modemserver.h>
#include <rudiments/charstring.h>

server *clientserverfactory::getServer(const char *type) {
	if (!charstring::compare(type,"inet")) {
		#ifdef INCLUDE_INETSERVER
			return new inetsocketserver();
		#else
			return NULL;
		#endif
	} else if (!charstring::compare(type,"unix")) {
		#ifdef INCLUDE_UNIXSERVER
			return new unixsocketserver();
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
			return new inetsocketclient();
		#else
			return NULL;
		#endif
	} else if (!charstring::compare(type,"unix")) {
		#ifdef INCLUDE_UNIXCLIENT
			return new unixsocketclient();
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
