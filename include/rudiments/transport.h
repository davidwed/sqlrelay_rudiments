// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <rudiments/private/config.h>

#include <rudiments/filedescriptor.h>

// The transport class is a base class for other classes that transport data
// over (or associated with) a file descriptor (such as clients and servers).

class transport : public filedescriptor {
	public:

			transport();
				// Creates an uninitialized transport.
			transport(int fd);
				// Creates a transport and associates the
				// already open file descriptor "fd" with it.

		using filedescriptor::write;
		using filedescriptor::read;

		// Write methods - write data to the file descriptor.
		// These methods return the number of bytes that were
		// successfully written.
		virtual ssize_t	write(unsigned short number);
				// Converts "number" to network byte order
				// and writes it to the file descriptor.
		virtual ssize_t	write(unsigned long number);
				// Converts "number" to network byte order
				// and writes it to the file descriptor.


		// Read methods - read data to the file descriptor.
		// These methods return the number of bytes that were
		// successfully read.
		virtual ssize_t	read(unsigned short *buffer);
				// Reads sizeof(unsigned short) bytes
				// from the file descriptor into "buffer" and
				// converts "buffer" to host byte order.
		virtual ssize_t	read(unsigned long *buffer);
				// Reads sizeof(unsigned long) bytes
				// from the file descriptor into "buffer" and
				// converts "buffer" to host byte order.

	#include <rudiments/private/transport.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/transportinlines.h>
#endif

#endif
