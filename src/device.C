// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/device.h>

#include <unistd.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

device::device() : file() {
	dontGetCurrentPropertiesOnOpen();
	type="device";
}

device::device(const device &d) : file(d) {
	type="device";
}

device &device::operator=(const device &d) {
	if (this!=&d) {
		file::operator=(d);
	}
	return *this;
}

device::~device() {}

bool device::createDeviceNode(const char *filename, bool blockdevice,
				unsigned short major, unsigned short minor,
				mode_t perms) {
	mode_t	mode=perms|((blockdevice)?S_IFBLK:S_IFCHR);
	dev_t	dev=(major<<8|minor);
	return !mknod(filename,mode,dev);
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
