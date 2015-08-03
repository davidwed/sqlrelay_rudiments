#include <rudiments/sys.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {
	if (!sys::halt()) {
		char	*err=error::getErrorString();
		stdoutput.printf("halt failed: %s\n",err);
		delete[] err;
	}
}
