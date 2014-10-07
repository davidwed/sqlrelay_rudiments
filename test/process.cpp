// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/process.h>
#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <rudiments/stdio.h>
#include <rudiments/snooze.h>

#ifdef _WIN32
const char	*cmd="c:\\cygwin64\\bin\\ls.exe";
#else
const char	*cmd="/bin/ls";
#endif
const char	*args[]={"ls","../",NULL};

int main(int argc, const char **argv) {

	stdoutput.printf("Process ID		: %d\n",
					process::getProcessId());
	stdoutput.printf("Parent Process ID	: %d\n",
					process::getParentProcessId());
	stdoutput.printf("Process Group ID	: %d\n",
					process::getProcessGroupId());
	stdoutput.printf("Session ID		: %d\n",
					process::getSessionId());
	stdoutput.printf("Real User ID		: %d\n",
					process::getRealUserId());
	stdoutput.printf("Effective User ID	: %d\n",
					process::getEffectiveUserId());
	stdoutput.printf("Real Group ID		: %d\n",
					process::getRealGroupId());
	stdoutput.printf("Effective Group ID	: %d\n",
					process::getEffectiveGroupId());
	stdoutput.printf("Create PID File	: %d\n",
			process::createPidFile("pidfile",
				permissions::evalPermString("rw-r--r--")));
	stdoutput.printf("Check for PID File	: %d\n",
					process::checkForPidFile("pidfile"));
	file::remove("pidfile");

	stdoutput.printf("spawn'ing...\n");
	pid_t	pid=process::spawn(cmd,args,false);
	snooze::macrosnooze(1);
	stdoutput.printf("Process id was %d\n",pid);

	stdoutput.printf("exec'ing...\n");
	process::exec(cmd,args);
	stdoutput.printf("failed\n");
}
