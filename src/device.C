// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/device.h>

#include <unistd.h>

device::device() : file() {
	dontGetCurrentPropertiesOnOpen();
}

device::device(int fd) : file(fd) {
	dontGetCurrentPropertiesOnOpen();
}

device::~device() {}

bool device::createDeviceNode(const char *filename, bool blockdevice,
				unsigned short major, unsigned short minor,
				mode_t perms) {
	mode_t	mode=perms|((blockdevice)?S_IFBLK:S_IFCHR);
	dev_t	dev=(major<<8|minor);
	return !mknod(filename,mode,dev);
}
