// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DEVICE_H
#define RUDIMENTS_DEVICE_H

#include <rudiments/private/deviceincludes.h>

// The device class provides methods for interacting with devices.

class device : public file {
	public:

			device();
			// Creates an uninitialized instance of the
			// device class.
		virtual	~device();

		static bool	createDeviceNode(const char *filename,
						bool blockdevice,
						unsigned short major,
						unsigned short minor,
						mode_t perms);
			// Creates device node "filename" with major number
			// "major" and minor number "minor".  The device node
			// will be created as a block device if "blockdevice"
			// is true, otherwise it will be created as a character
			// device.  The device node will be assigned
			// permissions "perms".
			//
			// Returns true on success and false on failure.
};

#endif
