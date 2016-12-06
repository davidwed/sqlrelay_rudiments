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
				#ifdef _WIN32
					"Interrupted function call"
				#else
					"Interrupted system call"
				#endif
				));
	error::clearError();
	test("clearError()/getErrorNumber()",!error::getErrorNumber());

	//stdoutput.printf("%s\n",error::getErrorString());
	const char	*success[]={
		"Success",
		"No error",
		"No error: 0",
		"Error 0",
		"Unknown error",
		NULL
	};
	test("clearError()/getErrorString()",
		charstring::inSet(error::getErrorString(),success));
}
