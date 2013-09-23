// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SOCKETCLIENT_H
#define RUDIMENTS_SOCKETCLIENT_H

#include <rudiments/private/socketclientincludes.h>

class socketclientprivate;

/** The socketclient class provides a base class for classes that implement
 *  the client side of a client-server communication model over a socket. */
class RUDIMENTS_DLLSPEC socketclient : public client {
	public:

		/** Creates an instance of the socketclient class. */
		socketclient();

		/** Creates an instance of the socketclient class
		 *  that is a copy of "c". */
		socketclient(const socketclient &c);

		/** Makes this instance of the socketclient class
		 *  idenitcal to "c". */
		socketclient	&operator=(const socketclient &c);

		/** Deletes this instance of the socketclient class. */
		virtual	~socketclient();

		/** Returns true if the client socket supports
		 *  blocking/nonblocking modes and false otherwise. */
		virtual bool	supportsBlockingNonBlockingModes();

		/** Puts the client socket in non-blocking mode.  Returns true
		 *  on success and false on failure.  Returns false if the
		 *  system doesn't support blocking/nonblocking modes. */
		virtual bool	useNonBlockingMode() const;

		/** Puts the client socket in blocking mode.  Returns true on
		 *  success and false on failure.  Returns false if the system
		 *  doesn't support blocking/nonblocking modes. */
		virtual bool	useBlockingMode() const;

		/** Returns true if the file descriptor is in
		 *  non-blocking mode and false otherwise. */
		virtual bool	isUsingNonBlockingMode() const;

		/** Use the ioctl() system call to perform various low-level
		 *  file descriptor operations. */
		virtual int32_t	ioCtl(int32_t command, void *arg) const;

	#include <rudiments/private/socketclient.h>
};

#endif
