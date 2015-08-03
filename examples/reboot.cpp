#include <rudiments/sys.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {
	if (!sys::reboot()) {
		char	*err=error::getErrorString();
		stdoutput.printf("reboot failed: %s\n",err);
		delete[] err;
	}
}
