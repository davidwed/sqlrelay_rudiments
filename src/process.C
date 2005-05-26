// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/process.h>

#ifndef __USE_XOPEN_EXTENDED
	// for getsid()
	#define __USE_XOPEN_EXTENDED
#endif
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

pid_t process::getProcessId() {
	return getpid();
}

pid_t process::getParentProcessId() {
	return getppid();
}

pid_t process::getProcessGroupId() {
	return getProcessGroupId(0);
}

pid_t process::getProcessGroupId(pid_t pid) {
	return getpgid(pid);
}

bool process::setProcessGroupId() {
	return setProcessGroupId(0);
}

bool process::setProcessGroupId(pid_t pgid) {
	return setProcessGroupId(0,pgid);
}

bool process::setProcessGroupId(pid_t pid, pid_t pgid) {
	return !setpgid(pid,pgid);
}

pid_t process::getSessionId() {
	return getSessionId(0);
}

pid_t process::getSessionId(pid_t pid) {
	return getsid(pid);
}

uid_t process::getRealUserId() {
	return getuid();
}

uid_t process::getEffectiveUserId() {
	return geteuid();
}

bool process::setUserId(uid_t uid) {
	#ifdef HAVE_SETUID
		return !setuid(uid);
	#else
		return true;
	#endif
}

bool process::setEffectiveUserId(uid_t uid) {
	return !seteuid(uid);
}

bool process::setRealAndEffectiveUserId(uid_t uid, uid_t euid) {
	return !setreuid(uid,euid);
}

gid_t process::getRealGroupId() {
	return getgid();
}

gid_t process::getEffectiveGroupId() {
	return getegid();
}

bool process::setGroupId(gid_t gid) {
	#ifdef HAVE_SETGID
		return !setgid(gid);
	#else
		return true;
	#endif
}

bool process::setEffectiveGroupId(gid_t gid) {
	return !setegid(gid);
}

bool process::setRealAndEffectiveGroupId(gid_t gid, gid_t egid) {
	return !setregid(gid,egid);
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
