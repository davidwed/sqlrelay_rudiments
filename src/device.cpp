// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/device.h>
#include <rudiments/error.h>

// for mknod
#ifdef RUDIMENTS_HAVE_SYS_STAT_H
	#include <sys/stat.h>
#endif

#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif

class deviceprivate {
	friend class device;
	private:
};

device::device() : file() {
	pvt=new deviceprivate;
	dontGetCurrentPropertiesOnOpen();
	type("device");
}

device::device(const device &d) : file(d) {
	pvt=new deviceprivate;
	type("device");
}

device &device::operator=(const device &d) {
	if (this!=&d) {
		file::operator=(d);
	}
	return *this;
}

device::~device() {
	delete pvt;
}

bool device::createDeviceNode(const char *filename, bool blockdevice,
				uint16_t major, uint16_t minor, mode_t perms) {
	#if defined(RUDIMENTS_HAVE_MKNOD)
		mode_t	mode=perms|((blockdevice)?S_IFBLK:S_IFCHR);
		dev_t	dev=(major<<8|minor);
		int32_t	result;
		error::clearError();
		do {
			result=mknod(filename,mode,dev);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}
