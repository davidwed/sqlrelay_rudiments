// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_FILEDESCRIPTOR_H
#define RUDIMENTS_FILEDESCRIPTOR_H

#include <rudiments/private/filedescriptorincludes.h>

// The filedescriptor class is a base class for other classes that utilize
// file descriptors.

class filedescriptor {
	public:
			filedescriptor();
				// Creates an uninitialized filedescriptor.
			filedescriptor(int filedesc);
				// Creates a filedescriptor and associates the
				// already open file descriptor "filedesc"
				// with it.
		virtual	~filedescriptor();
				// Calls close() if it hasn't already been
				// called.


		virtual bool	close();
				// Closes the file descriptor.
				// Returns true on success and false on failure.

		virtual int	getFileDescriptor() const;
				// Returns the file descriptor.
		virtual void	setFileDescriptor(int filedesc);
				// Sets the file descriptor associated with
				// the class to "filedesc".

		virtual int	duplicate() const;
				// Duplicates the file descriptor and returns
				// the handle of the duplicate descriptor.  The
				// old and new descriptors may be used
				// interchangeably, they share locks, position
				// pointers, flags (except the close-on-exec
				// flag), etc.
				//
				// Returns the lowest-numbered unused descriptor
				// on success or -1 on failure.
		virtual bool	duplicate(int newfd) const;
				// Sets file descriptor handle "newfd" to be a
				// duplicate of this file descriptor.  If
				// "newfd" is already open, it will be closed
				// first.
				//
				// Returns true on success and false on failure.

		#ifdef RUDIMENTS_HAS_SSL
		virtual void	setSSLContext(SSL_CTX *ctx);
				// Associates SSL context "ctx" with the
				// filedescriptor.  Passing in a NULL for "ctx"
				// has the additional side effect of calling
				// deInitializeSSL() below.
		virtual bool	initializeSSL();
				// Should be called after calling
				// setSSLContext() and one of open(),
				// create() or connect() (from socket child
				// classes). Causes the appropriate SSL
				// function to be called instead of or in
				// concert with read(), write(), connect(),
				// accept() and close() methods.
				//
				// Returns true on success and false on failure
		virtual SSL	*getSSL() const;
				// Returns a pointer to the currently
				// initialized SSL handle or NULL if
				// initializeSSL() has not been called or
				// failed.
		virtual void	deInitializeSSL();
				// Causes read(), write(), connect(), accept()
				// and close() methods to be performed
				// without the accompanying SSL-specific
				// functions.

		int		getSSLResult() const;
				// Returns the result code of the previously
				// executed SSL command.  If a method fails
				// but errno is 0 then an SSL-related error
				// occurred.  You may call SSL_get_error()
				// on the result of this method to determine
				// the exact error.
		#endif

		virtual bool	useNonBlockingMode();
				// Puts the file descriptor in non-blocking
				// mode.  Returns true on success and false on
				// failure.
		virtual bool	useBlockingMode();
				// Puts the file descriptor in blocking mode.
				// Returns true on success and false on
				// failure.
		virtual bool	isUsingNonBlockingMode();
				// Returns true if the file descriptor is in
				// non-blocking mode and false otherwise.

		// Write methods - write data to the file descriptor.
		// These methods return the number of bytes that were
		// successfully written or RESULT_ERROR if an error occurred.
		virtual ssize_t	write(unsigned short number);
				// Write "number" to the file descriptor.
		virtual ssize_t	write(unsigned long number);
				// Write "number" to the file descriptor.
		virtual ssize_t	write(short number);
				// Write "number" to the file descriptor.
		virtual ssize_t	write(long number);
				// Write "number" to the file descriptor.
		virtual ssize_t	write(float number);
				// Write "number" to the file descriptor.
		virtual ssize_t	write(double number);
				// Write "number" to the file descriptor.
		virtual ssize_t	write(unsigned char character);
				// Write "character" to the file descriptor.
		virtual ssize_t	write(char character);
				// Write "character" to the file descriptor.
		virtual ssize_t	write(bool value);
				// Write "value" to the file descriptor.
		virtual ssize_t	write(const unsigned char *string);
				// Write "string" to the file descriptor.  Note
				// that "string" must be NULL-terminated.
		virtual ssize_t	write(const char *string);
				// Write "string" to the file descriptor.  Note
				// that "string" must be NULL-terminated.
		virtual ssize_t	write(const unsigned char *string, size_t size);
				// Write "size" bytes from "string" to the file
				// descriptor.  Note that it is possible to
				// write beyond the string's NULL terminator
				// using this method.
		virtual ssize_t	write(const char *string, size_t size);
				// Write "size" bytes from "string" to the file
				// descriptor.  Note that it is possible to
				// write beyond the string's NULL terminator
				// using this method.
		virtual ssize_t	write(const void *buffer, size_t size);
				// Write "size" bytes from "buffer" to the file
				// descriptor.


		// Write methods - write data to the file descriptor with a
		// timeout of "sec" seconds and "usec" microseconds.
		// These methods return the number of bytes that were
		// successfully written, RESULT_ERROR if an error occurred or
		// RESULT_TIMEOUT if a timeout occurred.
		virtual ssize_t	write(unsigned short number,
							long sec, long usec);
				// Write "number" to the file descriptor.
		virtual ssize_t	write(unsigned long number,
							long sec, long usec);
				// Write "number" to the file descriptor.
		virtual ssize_t	write(short number, long sec, long usec);
				// Write "number" to the file descriptor.
		virtual ssize_t	write(long number, long sec, long usec);
				// Write "number" to the file descriptor.
		virtual ssize_t	write(float number, long sec, long usec);
				// Write "number" to the file descriptor.
		virtual ssize_t	write(double number, long sec, long usec);
				// Write "number" to the file descriptor.
		virtual ssize_t	write(unsigned char character,
							long sec, long usec);
				// Write "character" to the file descriptor.
		virtual ssize_t	write(char character, long sec, long usec);
				// Write "character" to the file descriptor.
		virtual ssize_t	write(bool value, long sec, long usec);
				// Write "value" to the file descriptor.
		virtual ssize_t	write(const unsigned char *string,
							long sec, long usec);
				// Write "string" to the file descriptor.  Note
				// that "string" must be NULL-terminated.
		virtual ssize_t	write(const char *string, long sec, long usec);
				// Write "string" to the file descriptor.  Note
				// that "string" must be NULL-terminated.
		virtual ssize_t	write(const unsigned char *string,
					size_t size, long sec, long usec);
				// Write "size" bytes from "string" to the file
				// descriptor.  Note that it is possible to
				// write beyond the string's NULL terminator
				// using this method.
		virtual ssize_t	write(const char *string,
					size_t size, long sec, long usec);
				// Write "size" bytes from "string" to the file
				// descriptor.  Note that it is possible to
				// write beyond the string's NULL terminator
				// using this method.
		virtual ssize_t	write(const void *buffer,
					size_t size, long sec, long usec);
				// Write "size" bytes from "buffer" to the file
				// descriptor.


		// Read methods - read data from the file descriptor.
		// These methods return the number of bytes that were
		// successfully read or RESULT_ERROR if an error occurred.
		virtual ssize_t	read(unsigned short *buffer);
				// Reads sizeof(unsigned short) bytes
				// from the file descriptor into "buffer".
		virtual ssize_t	read(unsigned long *buffer);
				// Reads sizeof(unsigned long) bytes
				// from the file descriptor into "buffer".
		virtual ssize_t	read(short *buffer);
				// Reads sizeof(short) bytes
				// from the file descriptor into "buffer".
		virtual ssize_t	read(long *buffer);
				// Reads sizeof(long) bytes
				// from the file descriptor into "buffer".
		virtual ssize_t	read(float *buffer);
				// Reads sizeof(float) bytes from the file
				// descriptor into "buffer".
		virtual ssize_t	read(double *buffer);
				// Reads sizeof(double) bytes from the file
				// descriptor into "buffer".
		virtual ssize_t	read(unsigned char *buffer);
				// Reads sizeof(unsigned char) bytes
				// from the file descriptor into "buffer".
		virtual ssize_t	read(char *buffer);
				// Reads sizeof(char) bytes from the file
				// descriptor into "buffer".
		virtual ssize_t	read(bool *buffer);
				// Reads sizeof(bool) bytes from the file
				// descriptor into "buffer".
		virtual ssize_t	read(unsigned char *buffer, size_t size);
				// Reads "size" bytes from the file descriptor
				// into "buffer".
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


		// Read methods - read data from the file descriptor with a
		// timeout of "sec" seconds and "usec" microseconds.
		// These methods return the number of bytes that were
		// successfully read, RESULT_ERROR if an error occurred or
		// RESULT_TIMEOUT if a timeout occurred.
		virtual ssize_t	read(unsigned short *buffer,
							long sec, long usec);
				// Reads sizeof(unsigned short) bytes
				// from the file descriptor into "buffer".
		virtual ssize_t	read(unsigned long *buffer,
							long sec, long usec);
				// Reads sizeof(unsigned long) bytes
				// from the file descriptor into "buffer".
		virtual ssize_t	read(short *buffer, long sec, long usec);
				// Reads sizeof(short) bytes
				// from the file descriptor into "buffer".
		virtual ssize_t	read(long *buffer, long sec, long usec);
				// Reads sizeof(long) bytes
				// from the file descriptor into "buffer".
		virtual ssize_t	read(float *buffer, long sec, long usec);
				// Reads sizeof(float) bytes from the file
				// descriptor into "buffer".
		virtual ssize_t	read(double *buffer, long sec, long usec);
				// Reads sizeof(double) bytes from the file
				// descriptor into "buffer".
		virtual ssize_t	read(unsigned char *buffer,
							long sec, long usec);
				// Reads sizeof(unsigned char) bytes
				// from the file descriptor into "buffer".
		virtual ssize_t	read(char *buffer, long sec, long usec);
				// Reads sizeof(char) bytes from the file
				// descriptor into "buffer".
		virtual ssize_t	read(bool *buffer, long sec, long usec);
				// Reads sizeof(bool) bytes from the
				// file descriptor into "buffer".
		virtual ssize_t	read(unsigned char *buffer,
					size_t size, long sec, long usec);
				// Reads "size" bytes from the file descriptor
				// into "buffer".
		virtual ssize_t	read(char *buffer,
					size_t size, long sec, long usec);
				// Reads "size" bytes from the file descriptor
				// into "buffer".
		virtual ssize_t	read(void *buf,
					size_t size, long sec, long usec);
				// Reads "size" bytes from the file descriptor
				// into "buffer".
		virtual ssize_t	read(char **buffer, char *terminator,
							long sec, long usec);
				// Reads from the file desciptor into "buffer"
				// until "terminator" is encountered.  Note
				// that "buffer" is allocated internally and
				// must be freed by the calling program.


		// Wait methods.
		virtual int	waitForNonBlockingRead(long sec, long usec);
				// Causes the application to wait until a read()
				// will proceed without blocking or until "sec"
				// seconds and "usec" microseconds have elapsed.
				//
				// Entering -1 for either parameter causes the
				// method to wait indefinitely.  
				//
				// Entering 0 for both parameters causes the
				// method to fall through immediately unless a
				// data is immediately available.
				//
				// Returns RESULT_ERROR on error, RESULT_TIMEOUT
				// on timeout and otherwise returns the number
				// of file descriptors that are ready to be
				// read from.
				//
				// This will usually be 1, indicating that the
				// file descriptor represented by the class is
				// ready to be read from.  However, if
				// useListener() has been called, it may be
				// greater than 1, indicating that a set of
				// file descriptors that the listener is
				// listening on are all ready to be read from.
		virtual int	waitForNonBlockingWrite(long sec, long usec);
				// Causes the application to wait until a
				// write() will proceed without blocking or
				// until "sec" seconds and "usec" microseconds
				// have elapsed.
				//
				// Entering -1 for either parameter causes the
				// method to wait indefinitely.  
				//
				// Entering 0 for both parameters causes the
				// method to fall through immediately unless a
				// data is immediately available.
				//
				// Returns RESULT_ERROR on error, RESULT_TIMEOUT
				// on timeout and otherwise returns the number
				// of file descriptors that are ready to be
				// written to.
				//
				// This will usually be 1, indicating that the
				// file descriptor represented by the class is
				// ready to be written to.  However, if
				// useListener() has been called, it may be
				// greater than 1, indicating that a set of
				// file descriptors that the listener is
				// listening on are all ready to be written to.


		// By default, if a read or write is occurring and a signal
		// interrupts it, the read or write returns with errno set to
		// EINTR and must be retried.  These methods override that
		// behavior.
		void	retryInterruptedReads();
			// Causes reads to automatically retry if interrupted
			// by a signal.
		void	dontRetryInterruptedReads();
			// Causes reads not to automatically retry if
			// interrupted by a signal.  This is the default.
		void	retryInterruptedWrites();
			// Causes writes to automatically retry if
			// interrupted by a signal.  This is the default.
		void	dontRetryInterruptedWrites();
			// Causes writes not to automatically retry if
			// interrupted by a signal.  This is the default.
		void	retryInterruptedWaits();
			// Causes waits to automatically retry if
			// interrupted by a signal.  This is the default.
		void	dontRetryInterruptedWaits();
			// Causes waits not to automatically retry if
			// interrupted by a signal.  This is the default.


		// By default, read() will attempt to read the specified number
		// of bytes from the file descriptor, in several passes if
		// necessary.  No single pass will try to read more than
		// SSIZE_MAX bytes.  These methods override that behavior.
		void	allowShortReads();
			// Causes a read to return the number of bytes that
			// were read in a single pass from the file descriptor.
			// Note that reads longer than SSIZE_MAX will always
			// return SSIZE_MAX or fewer bytes.  This is useful,
			// for example, when reading from devices which block
			// rather than returning 0 for EOF.
		void	dontAllowShortReads();
			// Causes a read to attempt to read the specified
			// number of bytes from the file descriptor, in several
			// passes if necessary.


		// By default, calls to waitForNonBlockingRead() and
		// waitForNonBlockingWrite() will wait using the select()
		// system call.  These methods override that behavior.
		void	useListener(listener *lstnr);
			// Causes calls to waitForNonBlockingRead() and
			// waitForNonBlockingWrite() to in turn call
			// lstnr->waitForNonBlockingRead() or
			// lstnr->waitForNonBlockingWrite().
		void	dontUseListener();
			// Causes calls to waitForNonBlockingRead() and
			// waitForNonBlockingWrite() to use select().
			// This is the default.
		listener	*getListener();
			// Returns the listener set previously by useListener() 
			// or NULL if none has been set.


		int	fcntl(int command, long arg);
		int	ioctl(int command, void *arg);
			// Interfaces for performing varions miscellaneous
			// operations on the file descriptor.

	#include <rudiments/private/filedescriptor.h>
};

#endif
