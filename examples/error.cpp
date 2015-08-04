#include <rudiments/error.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	error::setErrorNumber(EINTR);

	stdoutput.printf("error number: %d\n",error::getErrorNumber());
	char	*err=error::getErrorString();
	stdoutput.printf("error string: %s\n",err);
	delete[] err;

	error::clearError();

	stdoutput.printf("error number (after clear): %d\n",error::getErrorNumber());
	err=error::getErrorString();
	stdoutput.printf("error string (after clear) : %s\n",err);
	delete[] err;
}
