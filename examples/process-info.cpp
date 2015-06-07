#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	stdoutput.printf("Process ID		: %d\n",
					process::getProcessId());
	stdoutput.printf("Parent Process ID	: %d\n",
					process::getParentProcessId());
	stdoutput.printf("Process Group ID	: %d\n",
					process::getProcessGroupId());
	stdoutput.printf("Session ID		: %d\n",
					process::getSessionId());
}
