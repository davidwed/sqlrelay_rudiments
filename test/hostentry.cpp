// Copyright (c) 2002  David Muse
// See the file COPYING for more information

#include <rudiments/hostentry.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

// for AF_INET
#ifdef RUDIMENTS_HAVE_NETDB_H
	#include <netdb.h>
#endif
#ifdef RUDIMENTS_HAVE_WINSOCK2_H
	#include <winsock2.h>
#endif

int main(int argc, const char **argv) {

	header("hostentry");

	// unintialized
	hostentry	he;
	stdoutput.printf("uninitialized:\n");
	test("name",!he.getName());
	test("alias list",!he.getAliasList());
	test("address type",he.getAddressType()==-1);
	test("address length",he.getAddressLength()==-1);
	test("address list",!he.getAddressList());
	stdoutput.printf("\n");

	// localhost
	he.initialize("localhost");
	stdoutput.printf("localhost:\n");
	// this shouldn't be empty, but there's no
	// guarantee it'll actually be "localhost"
	test("name",he.getName());
	// it's likely there will be at least localhost.localdomain
	// but we can't count on that
	//test("alias list",he.getAliasList());
	test("address type",he.getAddressType()==AF_INET);
	test("address length",he.getAddressLength()==4);
	test("address list",he.getAddressList());
	stdoutput.printf("\n");

	// 127.0.0.1
	stdoutput.printf("127.0.0.1:\n");
	char	address[]={127,0,0,1};
	he.initialize(address,4,AF_INET);
	// this shouldn't be empty, but there's no
	// guarantee it'll actually be "localhost"
	test("name",he.getName());
	// it's likely there will be at least localhost.localdomain
	// but we can't count on that
	//test("alias list",he.getAliasList());
	test("address type",he.getAddressType()==AF_INET);
	test("address length",he.getAddressLength()==4);
	test("address list",he.getAddressList());
	stdoutput.printf("\n");

	// null-safety
	stdoutput.printf("NULL:\n");
	he.initialize(NULL);
	test("name",!he.getName());
	test("alias list",!he.getAliasList());
	test("address type",he.getAddressType()==-1);
	test("address length",he.getAddressLength()==-1);
	test("address list",!he.getAddressList());
	stdoutput.printf("\n");
}
