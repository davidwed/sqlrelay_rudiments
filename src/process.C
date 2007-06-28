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

#ifdef MINGW32
	#include <process.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

pid_t process::getProcessId() {
#ifndef MINGW32
	return getpid();
#else
	return _getpid();
#endif
}

pid_t process::getParentProcessId() {
#ifndef MINGW32
	return getppid();
#else
	return -1;
#endif
}

pid_t process::getProcessGroupId() {
	return getProcessGroupId(0);
}

pid_t process::getProcessGroupId(pid_t pid) {
#ifndef MINGW32
	return getpgid(pid);
#else
	return -1;
#endif
}

bool process::setProcessGroupId() {
	return setProcessGroupId(0);
}

bool process::setProcessGroupId(pid_t pgid) {
	return setProcessGroupId(0,pgid);
}

bool process::setProcessGroupId(pid_t pid, pid_t pgid) {
#ifndef MINGW32
	return !setpgid(pid,pgid);
#else
	return true;
#endif
}

pid_t process::getSessionId() {
	return getSessionId(0);
}

pid_t process::getSessionId(pid_t pid) {
#ifndef MINGW32
	return getsid(pid);
#else
	return -1;
#endif
}

uid_t process::getRealUserId() {
#ifndef MINGW32
	return getuid();
#else
	return -1;
#endif
}

uid_t process::getEffectiveUserId() {
#ifndef MINGW32
	return geteuid();
#else
	return -1;
#endif
}

bool process::setUserId(uid_t uid) {
#ifdef RUDIMENTS_HAVE_SETUID
	return !setuid(uid);
#else
	return true;
#endif
}

bool process::setEffectiveUserId(uid_t uid) {
#ifndef MINGW32
	return !seteuid(uid);
#else
	return true;
#endif
}

bool process::setRealAndEffectiveUserId(uid_t uid, uid_t euid) {
#ifndef MINGW32
	return !setreuid(uid,euid);
#else
	return true;
#endif
}

gid_t process::getRealGroupId() {
#ifndef MINGW32
	return getgid();
#else
	return -1;
#endif
}

gid_t process::getEffectiveGroupId() {
#ifndef MINGW32
	return getegid();
#else
	return -1;
#endif
}

bool process::setGroupId(gid_t gid) {
#ifdef RUDIMENTS_HAVE_SETGID
	return !setgid(gid);
#else
	return true;
#endif
}

bool process::setEffectiveGroupId(gid_t gid) {
#ifndef MINGW32
	return !setegid(gid);
#else
	return true;
#endif
}

bool process::setRealAndEffectiveGroupId(gid_t gid, gid_t egid) {
#ifndef MINGW32
	return !setregid(gid,egid);
#else
	return true;
#endif
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
