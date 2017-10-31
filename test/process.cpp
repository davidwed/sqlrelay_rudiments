// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/process.h>
#include <rudiments/permissions.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/directory.h>
#include <rudiments/file.h>
#include <rudiments/stdio.h>
#include <rudiments/snooze.h>
#include "test.cpp"

int main(int argc, const char **argv) {

	header("process");

	pid_t	pid=process::getProcessId();
	pid_t	ppid=process::getParentProcessId();
	pid_t	pgid=process::getProcessGroupId();
	uid_t	uid=process::getUserId();
	uid_t	euid=process::getEffectiveUserId();
	gid_t	gid=process::getGroupId();
	gid_t	egid=process::getEffectiveGroupId();

	/*pid_t	sid=process::getSessionId();
	stdoutput.printf("pid: %d\n",pid);
	stdoutput.printf("ppid: %d\n",ppid);
	stdoutput.printf("pgid: %d\n",pgid);
	stdoutput.printf("sid: %d\n",sid);
	stdoutput.printf("uid: %d\n",uid);
	stdoutput.printf("euid: %d\n",euid);
	stdoutput.printf("gid: %d\n",gid);
	stdoutput.printf("egid: %d\n",egid);*/

	test("pid",pid>1);
	test("ppid",ppid>1);
#ifndef _WIN32
	#ifdef RUDIMENTS_HAVE_GETPGID
	test("pgid",pgid>1);
	#else
	test("pgid",pgid==-1);
	#endif
	test("uid/euid",uid==euid);
	test("gid/egid",gid==egid);
#endif


	stringbuffer	cmd;
#ifdef _WIN32
	cmd.append(directory::getCurrentWorkingDirectory())->
					append("\\")->append("ls.bat");
	const char	*args[]={"ls","..\\",NULL};
#else
	cmd.append("/bin/ls");
	const char	*args[]={"ls","../",NULL};
#endif


	test("create pidfile",process::createPidFile("pidfile",
				permissions::evalPermString("rw-r--r--")));
	test("check pidfile",process::checkForPidFile("pidfile"));
	file::remove("pidfile");
	stdoutput.printf("\n");

	test("spawn",process::spawn(cmd.getString(),args,false)>1);
	snooze::macrosnooze(1);
	stdoutput.printf("\n");

	process::exec(cmd.getString(),args);
}
