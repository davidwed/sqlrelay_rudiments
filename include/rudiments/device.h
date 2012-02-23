// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DEVICE_H
#define RUDIMENTS_DEVICE_H

#include <rudiments/private/deviceincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class deviceprivate;

/** The device class provides methods for interacting with devices.
 *
 *  It is different from the file class only in that it provides a method
 *  to create a device node and that by default, the device class does not
 *  query the device for parameters (such as size) when it is opened but
 *  the file class does.  Querying a device node for parameters can
 *  have unexpected effects. */
class RUDIMENTS_DLLSPEC device : public file {
	public:

		/** Creates an instance of the device class. */
		device();

		/** Creates an instance of the device class
		 *  that is a copy of "d". */
		device(const device &d);

		/** Makes this instance of the device class
		 *  identical to "d". */
		device	&operator=(const device &d);

		/** Deletes this instance of the device class. */
		virtual	~device();

		/** Creates device node "filename" with major number "major" and
		 *  minor number "minor".  The device node will be created as a
		 *  block device if "blockdevice" is true, otherwise it will be
		 *  created as a character device.  The device node will be
		 *  assigned permissions "perms".  Returns true on success and
		 *  false on failure. */
		static bool	createDeviceNode(const char *filename,
						bool blockdevice,
						uint16_t major,
						uint16_t minor,
						mode_t perms);

	#include <rudiments/private/device.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
