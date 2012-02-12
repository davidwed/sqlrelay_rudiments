// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SYSTEM_H
#define RUDIMENTS_SYSTEM_H

#include <rudiments/private/system.h>

// wrap:
//	sys/utsname.h - uname()
//	stdlib.h - getloadavg()
//	unistd.h - sysconf()
//			confstr() - use sysctl() on bsd
//			gethostid(),sethostid() - use sysctl() on bsd
//			getpagesize() - use sysconf() on bsd
//			sync()
//	sys/raw.h - structs/defines for /dev/rawctl ioctl()'s
//	sys/syslog.h - openlog(),closelog(),setlogmask(),syslog(),vsyslog()
//	not in solaris - 
//		sys/sysctl.h - sysctl()
//	linux only - 
//		sys/kdaemon.h - bdflush()
//		sys/klog.h - klogctl()
//		sys/personality.h - personality() - set linux kernel personality
//		sys/reboot.h - reboot()
//		sys/swap.h - swapon()/swapoff()
//		sys/sysinfo.h - functions for gettting overall system statistics
//				sysinfo(),
//				get_nprocs_conf(),get_nprocs(),
//				get_phys_pages,get_avphys_pages()

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class systemprivate;

class DLLSPEC system {
	public:

	#include <rudiments/private/system.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
