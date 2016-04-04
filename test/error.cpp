// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/error.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("error");

	error::setErrorNumber(EINTR);
	test("getErrorNumber()",error::getErrorNumber()==EINTR);
	test("getErrorString()",
		!charstring::compare(error::getErrorString(),
					"Interrupted system call"));
	error::clearError();
	test("clearError()/getErrorNumber()",!error::getErrorNumber());
	test("clearError()/getErrorString()",
		!charstring::compare(error::getErrorString(),"Success"));
}
