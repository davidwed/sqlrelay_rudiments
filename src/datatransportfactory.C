// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/datatransportfactory.h>
#include <rudiments/inetserversocket.h>
#include <rudiments/inetclientsocket.h>
#include <rudiments/unixserversocket.h>
#include <rudiments/unixclientsocket.h>

server	*datatransportfactory::getServer(const char *type) {
	if (!strcmp(type,"inet")) {
		return new inetserversocket();
	} else if (!strcmp(type,"unix")) {
		return new unixserversocket();
	}
	return NULL;
}

client	*datatransportfactory::getClient(const char *type) {
	if (!strcmp(type,"inet")) {
		return new inetclientsocket();
	} else if (!strcmp(type,"unix")) {
		return new unixclientsocket();
	}
	return NULL;
}
