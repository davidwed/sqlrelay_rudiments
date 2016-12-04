// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/process.h>
#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <rudiments/stdio.h>
#include <rudiments/snooze.h>
#include "test.cpp"

#ifdef _WIN32
	const char	*cmd="ls.bat";
	const char	*args[]={"ls","..\\",NULL};
#else
	const char	*cmd="/bin/ls";
	const char	*args[]={"ls","../",NULL};
#endif

int main(int argc, const char **argv) {

	header("process");

	pid_t	pid=process::getProcessId();
	pid_t	ppid=process::getParentProcessId();
	pid_t	pgid=process::getProcessGroupId();
	pid_t	sid=process::getSessionId();
	uid_t	uid=process::getUserId();
	uid_t	euid=process::getEffectiveUserId();
	gid_t	gid=process::getGroupId();
	gid_t	egid=process::getEffectiveGroupId();

	/*stdoutput.printf("pid: %d\n",pid);
	stdoutput.printf("ppid: %d\n",ppid);
	stdoutput.printf("pgid: %d\n",pgid);
	stdoutput.printf("sid: %d\n",sid);
	stdoutput.printf("uid: %d\n",uid);
	stdoutput.printf("euid: %d\n",euid);
	stdoutput.printf("gid: %d\n",gid);
	stdoutput.printf("egid: %d\n",egid);*/

	test("pid",pid>1);
	test("ppid",ppid>1);
	test("pgid",pgid==pid);
	test("sid",sid==ppid);
	test("uid/euid",uid==euid);
	test("gid/egid",gid==egid);
	test("create pidfile",process::createPidFile("pidfile",
				permissions::evalPermString("rw-r--r--")));
	test("check pidfile",process::checkForPidFile("pidfile"));
	file::remove("pidfile");
	stdoutput.printf("\n");

	test("spawn",process::spawn(cmd,args,false)>1);
	snooze::macrosnooze(1);
	stdoutput.printf("\n");

	process::exec(cmd,args);
}
