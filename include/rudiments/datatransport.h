// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef DATATRANSPORT_H
#define DATATRANSPORT_H

#include <rudiments/private/config.h>

#include <rudiments/filedescriptor.h>

// The datatransport class is a base class for other classes that transport data
// over (or associated with) a file descriptor (such as clients and servers).

class datatransport : public filedescriptor {
	public:

			datatransport();
				// Creates an uninitialized datatransport.
			datatransport(int fd);
				// Creates a datatransport and associates the
				// already open file descriptor "fd" with it.

		// Write methods - write data to the file descriptor.
		// These methods return the number of bytes that were
		// successfully written.
		virtual ssize_t	write(unsigned short number);
				// Converts "number" to network byte order
				// and writes it to the file descriptor.
		virtual ssize_t	write(unsigned long number);
				// Converts "number" to network byte order
				// and writes it to the file descriptor.
		virtual ssize_t	write(float number);
				// Write "number" to the file descriptor.
		virtual ssize_t	write(double number);
				// Write "number" to the file descriptor.
		virtual ssize_t	write(char character);
				// Write "character" to the file descriptor.
		virtual ssize_t	write(const char *string);
				// Write "string" to the file descriptor.  Note
				// that "string" must be NULL-terminated.
		virtual ssize_t	write(const char *string, size_t size);
				// Write "size" bytes from "string" to the file
				// descriptor.  Note that it is possible to
				// write beyond the string's NULL terminator
				// using this method.
		virtual ssize_t	write(const void *buffer, size_t size);
				// Write "size" bytes from "buffer" to the file
				// descriptor.


		// Read methods - read data from the file descriptor.
		// These methods return the number of bytes that were
		// successfully read.
		virtual ssize_t	read(unsigned short *buffer);
				// Reads sizeof(unsigned short) bytes
				// from the file descriptor into "buffer" and
				// converts "buffer" to host byte order.
		virtual ssize_t	read(unsigned long *buffer);
				// Reads sizeof(unsigned short) bytes
				// from the file descriptor into "buffer" and
				// converts "buffer" to host byte order.
		virtual ssize_t	read(float *buffer);
				// Reads sizeof(float) bytes from the file
				// descriptor into "buffer".
		virtual ssize_t	read(double *buffer);
				// Reads sizeof(double) bytes from the file
				// descriptor into "buffer".
		virtual ssize_t	read(char *buffer);
				// Reads sizeof(char) bytes from the file
				// descriptor into "buffer".
		virtual ssize_t	read(char *buffer, size_t size);
				// Reads "size" bytes from the file descriptor
				// into "buffer".
		virtual ssize_t	read(void *buf, size_t size);
				// Reads "size" bytes from the file descriptor
				// into "buffer".
		virtual ssize_t	read(char **buffer, char *terminator);
				// Reads from the file desciptor into "buffer"
				// until "terminator" is encountered.  Note
				// that "buffer" is allocated internally and
				// must be freed by the calling program.

	#include <rudiments/private/datatransport.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/datatransportinlines.h>
#endif

#endif
