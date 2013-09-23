// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SOCKETSERVER_H
#define RUDIMENTS_SOCKETSERVER_H

#include <rudiments/private/socketserverincludes.h>

class socketserverprivate;

/** The socketserver class provides a base class for classes that implement
 *  the server side of a client-server communication model over a socket. */
class RUDIMENTS_DLLSPEC socketserver : public server {
	public:

		/** Creates an instance of the socketserver class. */
		socketserver();

		/** Creates an instance of the socketserver class
 		 *  that is a copy of "s". */
		socketserver(const socketserver &s);

		/** Makes this instance of the socketserver class
		 *  identical to "s". */
		socketserver	&operator=(const socketserver &s);

		/** Deletes this instance of the socketserver class. */
		virtual		~socketserver();

		/** Returns false if the system doesn't support
		 *  blocking/nonblocking modes and true
		 *  otherwise. */
		virtual bool	supportsBlockingNonBlockingModes();

		/** Puts the file descriptor in non-blocking
		 *  mode.  Returns true on success and false on
		 *  failure.
		 *  Returns false if the system doesn't support
		 *  blocking/nonblocking modes. */
		virtual bool	useNonBlockingMode() const;

		/** Puts the file descriptor in blocking mode.
		 *  Returns true on success and false on
		 *  failure.
		 *  Returns false if the system doesn't support
		 *  blocking/nonblocking modes. */
		virtual bool	useBlockingMode() const;

		/** Returns true if the file descriptor is in
		 *  non-blocking mode and false otherwise. */
		virtual bool	isUsingNonBlockingMode() const;

		/** Use the ioctl() system call to perform various low-level
		 *  file descriptor operations. */
		virtual int32_t	ioCtl(int32_t command, void *arg) const;


		/** Instructs the socket to stay open for
		 *  "timeout" seconds even after close() is
		 *  called to allow it to receive any data that
		 *  may still be buffered.  The default is not
		 *  to linger on close.
		 * 
		 *  Returns true on success and false on failure. */
		bool	lingerOnClose(int32_t timeout);

		/** Instructs the socket to close immediately
		 *  when close() is called, dumping any data
		 *  that may still be buffered but that it
		 *  may not have received.  This is the default.
		 * 
		 *  Returns true on success and false on failure. */
		bool	dontLingerOnClose();

		/** Allows sockets in the TIME_WAIT state to be
		 *  reused.  The default is to allocate a new socket.
		 * 
		 *  Returns true on success and false on failure. */
		bool	reuseAddresses();

		/** Prevents sockets in the TIME_WAIT state from
		 *  being reused.  This is the default.
		 * 
		 *  Returns true on success and false on failure. */
		bool	dontReuseAddresses();

		/** Waits until a client connects then places
		 *  that connection in queue.  Up to "backlog"
		 *  connections may be queued before future
		 *  conenctions are refused.
		 * 
		 *  Returns true on success and false on failure. */
		virtual	bool	listen(int32_t backlog);

	#include <rudiments/private/socketserver.h>
};

#endif
