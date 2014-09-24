// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_FILEDESCRIPTOR_H
#define RUDIMENTS_FILEDESCRIPTOR_H

#include <rudiments/private/filedescriptorincludes.h>

class listener;
class filedescriptorprivate;

/** The filedescriptor class is a base class for other classes that utilize
 *  file descriptors. */
class RUDIMENTS_DLLSPEC filedescriptor {
	public:
		/** Creates an instance of the filedescriptor class. */
		filedescriptor();

		/** Creates an instance of the filedescriptor class
		 *  that is a copy of "f". */
		filedescriptor(const filedescriptor &f);

		/** Makes this instance of the filedescriptor class
		 *  identical to "f". */
		filedescriptor	&operator=(const filedescriptor &f);

		/** Deletes this instance of the filedescriptor class.
		 *  Calls close() if it hasn't already been called. */
		virtual	~filedescriptor();

		/** Closes the file descriptor.
		 *  Returns true on success and false on failure. */
		virtual	bool	close();

		/** Returns the file descriptor. */
		int32_t	getFileDescriptor() const;

		/** Sets the file descriptor associated with
		 *  the class to "filedesc". */
		void	setFileDescriptor(int32_t filedesc);

		/** Duplicates the file descriptor and returns
		 *  the handle of the duplicate descriptor.  The
		 *  old and new descriptors may be used
		 *  interchangeably, they share locks, position
		 *  pointers, flags (except the close-on-exec
		 *  flag), etc.
		 * 
		 *  Returns the lowest-numbered unused descriptor
		 *  on success or -1 on failure. */
		int32_t	duplicate() const;

		/** Sets file descriptor handle "newfd" to be a
		 *  duplicate of this file descriptor.  If
		 *  "newfd" is already open, it will be closed
		 *  first.
		 * 
		 *  Returns true on success and false on failure. */
		bool	duplicate(int32_t newfd) const;

		#ifdef RUDIMENTS_HAS_SSL
		/** Associates an SSL context "ctx" with the
		 *  filedescriptor.  Passing in a NULL for "ctx"
		 *  has the additional side effect of calling
		 *  deInitializeSSL() below.
		 *
		 *  When using OpenSSL, pass in a pointer to an
		 *  instance of the SSL_CTX structure here. */
		void	setSSLContext(void *ctx);

		/** Returns the SSL context currently associated
		 *  with the filedescriptor or NULL if none
		 *  is currently associated.
		 *
		 *  When using OpenSSL, cast the result to a
		 *  pointer to an SSL_CTX structure. */
		void	*getSSLContext();

		/** Should be called after calling
		 *  setSSLContext() and one of open(),
		 *  create() or connect() (from socket child
		 *  classes). Causes the appropriate SSL
		 *  function to be called instead of or in
		 *  concert with read(), write(), connect(),
		 *  accept() and close() methods.
		 * 
		 *  Returns true on success and false on failure. */
		bool	initializeSSL();

		/** Returns a pointer to the currently
		 *  initialized SSL handle or NULL if
		 *  initializeSSL() has not been called or
		 *  failed.
		 *
		 *  When using OpenSSL, cast the result to a
		 *  pointer to an SSL structure. */
		void	*getSSL() const;

		/** Causes read(), write(), connect(), accept()
		 *  and close() methods to be performed
		 *  without the accompanying SSL-specific
		 *  functions. */
		void	deInitializeSSL();

		/** Returns the result code of the previously
		 *  executed SSL command.  If a method fails
		 *  but errno is 0 then an SSL-related error
		 *  occurred.  You may call SSL_get_error()
		 *  on the result of this method to determine
		 *  the exact error. */
		int32_t	getSSLResult() const;
		#endif

		/** Returns true if the client socket supports
		 *  blocking/nonblocking modes and false otherwise. */
		virtual bool	supportsBlockingNonBlockingModes();

		/** Puts the file descriptor in non-blocking
		 *  mode.  Returns true on success and false on
		 *  failure. */
		virtual bool	useNonBlockingMode() const;

		/** Puts the file descriptor in blocking mode.
		 *  Returns true on success and false on
		 *  failure. */
		virtual bool	useBlockingMode() const;

		/** Returns true if the file descriptor is in
		 *  non-blocking mode and false otherwise. */
		virtual bool	isUsingNonBlockingMode() const;

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		ssize_t	write(uint16_t number) const;

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		ssize_t	write(uint32_t number) const;

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		ssize_t	write(uint64_t number) const;

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		ssize_t	write(int16_t number) const;

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		ssize_t	write(int32_t number) const;

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		ssize_t	write(int64_t number) const;

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		ssize_t	write(float number) const;

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		ssize_t	write(double number) const;

		/** Writes "character" to the file descriptor.  Returns the
		 *  number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	write(unsigned char character) const;

		/** Writes "character" to the file descriptor.  Returns the
		 *  number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	write(char character) const;

		/** Writes "value" to the file descriptor.  Returns the
		 *  number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	write(bool value) const;

		/** Writes NULL-terminated "string" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	write(const unsigned char *string) const;

		/** Writes NULL-terminated "string" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	write(const char *string) const;

		/** Writes "size" bytes of "string" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred.  Note that it is
 		 *  possible to write beyond the string's NULL terminator
 		 *  using this method.  */
		ssize_t	write(const unsigned char *string, size_t size) const;

		/** Writes "size" bytes of "string" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred.  Note that it is
 		 *  possible to write beyond the string's NULL terminator
 		 *  using this method.  */
		ssize_t	write(const char *string, size_t size) const;

		/** Writes "size" bytes of "buffer" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	write(const void *buffer, size_t size) const;

		/** Writes "number" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(uint16_t number, int32_t sec, int32_t usec) const;

		/** Writes "number" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(uint32_t number, int32_t sec, int32_t usec) const;

		/** Writes "number" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(uint64_t number, int32_t sec, int32_t usec) const;

		/** Writes "number" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(int16_t number, int32_t sec, int32_t usec) const;

		/** Writes "number" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(int32_t number, int32_t sec, int32_t usec) const;

		/** Writes "number" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(int64_t number, int32_t sec, int32_t usec) const;

		/** Writes "number" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(float number, int32_t sec, int32_t usec) const;

		/** Writes "number" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(double number, int32_t sec, int32_t usec) const;

		/** Writes "character" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(unsigned char character,
					int32_t sec, int32_t usec) const;

		/** Writes "character" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(char character, int32_t sec, int32_t usec) const;

		/** Writes "value" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(bool value, int32_t sec, int32_t usec) const;

		/** Writes NULL-terminated "string" to the file descriptor with
		 *  a timeout of "sec" seconds and "usec" microseconds.
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred or RESULT_TIMEOUT if a
		 *  timeout occurred. */
		ssize_t	write(const unsigned char *string,
					int32_t sec, int32_t usec) const;

		/** Writes NULL-terminated "string" to the file descriptor with
		 *  a timeout of "sec" seconds and "usec" microseconds.
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred or RESULT_TIMEOUT if a
		 *  timeout occurred. */
		ssize_t	write(const char *string,
					int32_t sec, int32_t usec) const;

		/** Writes "size" bytes of "string" to the file descriptor with
		 *  a timeout of "sec" seconds and "usec" microseconds.
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred or RESULT_TIMEOUT if a
		 *  timeout occurred.  Note that it is possible to write beyond
		 *  the string's NULL terminator using this method.  */
		ssize_t	write(const unsigned char *string, size_t size,
					int32_t sec, int32_t usec) const;

		/** Writes "size" bytes of "string" to the file descriptor with
		 *  a timeout of "sec" seconds and "usec" microseconds.
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred or RESULT_TIMEOUT if a
		 *  timeout occurred.  Note that it is possible to write beyond
		 *  the string's NULL terminator using this method.  */
		ssize_t	write(const char *string, size_t size,
					int32_t sec, int32_t usec) const;

		/** Writes "size" bytes of "buffer" to the file descriptor with
		 *  a timeout of "sec" seconds and "usec" microseconds.
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred or RESULT_TIMEOUT if a
		 *  timeout occurred. */
		ssize_t	write(const void *buffer, size_t size,
					int32_t sec, int32_t usec) const;

		/** Prints "..." to the filedescriptor using "format" which
		 *  should comply with standard printf formatting rules. */
		size_t	printf(const char *format, ...);

		/** Prints "argp" to the filedescriptor using "format" which
		 *  should comply with standard printf formatting rules.
		 *
		 *  Note that argp is a pointer to a va_list, not just a
		 *  va_list. */
		size_t	printf(const char *format, va_list *argp);

		/** Prints "c" to the filedescriptor, however if "c" is
		 *  non-printing character then it is printed as a hex value of
		 *  the format: (0x0a).  Carriage returns, line feeds and tabs
		 *  are printed as \n, \r and \t. */
		void	safePrint(char c);

		/** Prints "str" to the filedescriptor, however all non-printing
		 *  characters are printed as hex values of the format: (0x0a)
		 *  and carriage returns, line feeds and tabs are printed as
		 *  \n, \r and \t. */
		void	safePrint(const char *str);

		/** Prints "length" characters of "str" to the filedescriptor,
		 *  however all non-printing characters are printed as hex
		 *  values of the format: (0x0a) and carriage returns, line
		 *  feeds and tabs are printed as \n, \r and \t. */
		void	safePrint(const char *str, int32_t length);

		/** Prints "value" to the file descriptor
		 *  as a string of ones and zeros. */
		void	printBits(unsigned char value);

		/** Prints "value" to the file descriptor
		 *  as a string of ones and zeros. */
		void	printBits(uint16_t value);

		/** Prints "value" to the file descriptor
		 *  as a string of ones and zeros. */
		void	printBits(uint32_t value);

		/** Prints "value" to the file descriptor
		 *  as a string of ones and zeros. */
		void	printBits(uint64_t value);

		/** Prints "value" to the file descriptor
		 *  as a string of ones and zeros. */
		void	printBits(char value);

		/** Prints "value" to the file descriptor
		 *  as a string of ones and zeros. */
		void	printBits(int16_t value);

		/** Prints "value" to the file descriptor
		 *  as a string of ones and zeros. */
		void	printBits(int32_t value);

		/** Prints "value" to the file descriptor
		 *  as a string of ones and zeros. */
		void	printBits(int64_t value);

		/** Prints "value" to the file descriptor
		 *  as a string of ones and zeros. */
		void	printBits(unsigned char *bits, uint64_t size);

		/** Reads an unsigned 16-bit integer from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred. */
		ssize_t	read(uint16_t *buffer) const;

		/** Reads an unsigned 32-bit integer from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred. */
		ssize_t	read(uint32_t *buffer) const;

		/** Reads an unsigned 64-bit integer from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred. */
		ssize_t	read(uint64_t *buffer) const;

		/** Reads a 16-bit integer from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		ssize_t	read(int16_t *buffer) const;

		/** Reads a 32-bit integer from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		ssize_t	read(int32_t *buffer) const;

		/** Reads a 64-bit integer from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		ssize_t	read(int64_t *buffer) const;

		/** Reads a floating point number from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred. */
		ssize_t	read(float *buffer) const;

		/** Reads a double-precision floating point number from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred. */
		ssize_t	read(double *buffer) const;

		/** Reads an unsigned character from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred. */
		ssize_t	read(unsigned char *buffer) const;

		/** Reads an character from the file descriptor into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	read(char *buffer) const;

		/** Reads a boolean value from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		ssize_t	read(bool *buffer) const;

		/** Reads "size" unsigned characters from the file descriptor
		 *  into "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		ssize_t	read(unsigned char *buffer, size_t size) const;

		/** Reads "size" characters from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		ssize_t	read(char *buffer, size_t size) const;

		/** Reads "size" bytes from the file descriptor into "buf".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	read(void *buf, size_t size) const;

		/** Reads from the file desciptor into "buffer" until
		 *  "terminator" is encountered.
		 *
		 *  Note that "buffer" is allocated internally and must be freed
		 *  by the calling program.
		 *
		 *  Returns the number of bytes that were read or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	read(char **buffer, const char *terminator) const;

		/** Reads an unsigned 16-bit integer from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(uint16_t *buffer, int32_t sec, int32_t usec) const;

		/** Reads an unsigned 32-bit integer from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(uint32_t *buffer, int32_t sec, int32_t usec) const;

		/** Reads an unsigned 64-bit integer from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(uint64_t *buffer, int32_t sec, int32_t usec) const;

		/** Reads a 16-bit integer from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(int16_t *buffer, int32_t sec, int32_t usec) const;

		/** Reads a 32-bit integer from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(int32_t *buffer, int32_t sec, int32_t usec) const;

		/** Reads a 64-bit integer from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(int64_t *buffer, int32_t sec, int32_t usec) const;

		/** Reads a floating point number from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(float *buffer, int32_t sec, int32_t usec) const;

		/** Reads a double-precision floating point number from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(double *buffer, int32_t sec, int32_t usec) const;

		/** Reads an unsigned character from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(unsigned char *buffer,
					int32_t sec, int32_t usec) const;

		/** Reads a character from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(char *buffer, int32_t sec, int32_t usec) const;

		/** Reads a boolean value from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(bool *buffer, int32_t sec, int32_t usec) const;

		/** Reads "size" unsigned characters from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(unsigned char *buffer, size_t size,
					int32_t sec, int32_t usec) const;

		/** Reads "size" characters from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(char *buffer, size_t size,
					int32_t sec, int32_t usec) const;

		/** Reads "size" bytes from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(void *buf, size_t size,
					int32_t sec, int32_t usec) const;

		/** Reads from the file desciptor into "buffer" until
		 *  "terminator" is encountered with a timeout of "sec" seconds
		 *  and "usec" microseconds.
		 *
		 *  Note that "buffer" is allocated internally and must be freed
		 *  by the calling program.
		 *
		 *  Returns the number of bytes that were read or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	read(char **buffer, const char *terminator,
					int32_t sec, int32_t usec) const;


		/** Causes the application to wait until a read()
		 *  will proceed without blocking or until "sec"
		 *  seconds and "usec" microseconds have elapsed.
		 * 
		 *  Entering -1 for either parameter causes the
		 *  method to wait indefinitely.  
		 * 
		 *  Entering 0 for both parameters causes the
		 *  method to fall through immediately unless a
		 *  data is immediately available.
		 * 
		 *  Returns RESULT_ERROR on error, RESULT_TIMEOUT
		 *  on timeout and otherwise returns the number
		 *  of file descriptors that are ready to be
		 *  read from.
		 * 
		 *  This will usually be 1, indicating that the
		 *  file descriptor represented by the class is
		 *  ready to be read from. */
		int32_t	waitForNonBlockingRead(int32_t sec, int32_t usec)
									const;

		/** Causes the application to wait until a
		 *  write() will proceed without blocking or
		 *  until "sec" seconds and "usec" microseconds
		 *  have elapsed.
		 * 
		 *  Entering -1 for either parameter causes the
		 *  method to wait indefinitely.  
		 * 
		 *  Entering 0 for both parameters causes the
		 *  method to fall through immediately unless a
		 *  data is immediately available.
		 * 
		 *  Returns RESULT_ERROR on error, RESULT_TIMEOUT
		 *  on timeout and otherwise returns the number
		 *  of file descriptors that are ready to be
		 *  written to.
		 * 
		 *  This will usually be 1, indicating that the
		 *  file descriptor represented by the class is
		 *  ready to be written to. */
		int32_t	waitForNonBlockingWrite(int32_t sec, int32_t usec)
									const;


		/** Causes reads to automatically retry if interrupted by a
		 *  signal.  By default, if a read is occurring and a signal
		 *  interrupts it, the read fails, the system error is set to
		 *  EINTR and the read must be retried. */
		void	retryInterruptedReads();

		/** Causes reads not to automatically retry if interrupted by a
		 *  signal.  This is the default behavior.  By default, if a
		 *  read is occurring and a signal interrupts it, the read
		 *  fails, the system error is set to EINTR and the read must
		 *  be retried. */
		void	dontRetryInterruptedReads();

		/** Returns true if interrupted reads will be retried and false
		 *  otherwise. */
		bool	getRetryInterruptedReads() const;

		/** Causes writes to automatically retry if interrupted by a
		 *  signal.  By default, if a write is occurring and a signal
		 *  interrupts it, the write fails, the system error is set to
		 *  EINTR and the write must be retried. */
		void	retryInterruptedWrites();

		/** Causes writes not to automatically retry if interrupted by a
		 *  signal.  This is the default behavior.  By default, if a
		 *  write is occurring and a signal interrupts it, the write
		 *  fails, the system error is set to EINTR and the write must
		 *  be retried. */
		void	dontRetryInterruptedWrites();

		/** Returns true if interrupted writes will be retried and false
		 *  otherwise. */
		bool	getRetryInterruptedWrites() const;

		/** Causes wait operations to be automatically retried if
		 *  interrupted by a signal.  This is the default behavior.
		 *  Otherwise, if a wait is occurring and a signal interrupts
		 *  it, the wait fails, the system error is set to EINTR and
		 *  the wait must be retried. */
		void	retryInterruptedWaits();

		/** Causes wait operations not to be automatically retried if
		 *  interrupted by a signal.  If this is set, a wait is
		 *  occurring and a signal interrupts it, the wait fails, the
		 *  system error is set to EINTR and the wait must be
		 *  retried. */
		void	dontRetryInterruptedWaits();

		/** Returns true if interrupted waits will be retried and false
		 *  otherwise. */
		bool	getRetryInterruptedWaits() const;

		/** Causes fcntl operations to be automatically retried if
		 *  interrupted by a signal.  This is the default behavior.
		 *  Otherwise, if a fcntl is occurring and a signal interrupts
		 *  it, the fcntl fails, the system error is set to EINTR and
		 *  the fcntl must be retried. */
		void	retryInterruptedFcntl();

		/** Causes fcntl operations not to be automatically retried if
		 *  interrupted by a signal.  If this is set, a fcntl is
		 *  occurring and a signal interrupts it, the fcntl fails, the
		 *  system error is set to EINTR and the fcntl must be
		 *  retried. */
		void	dontRetryInterruptedFcntl();

		/** Returns true if interrupted calls to fcntl will be retried
		 *  and false otherwise. */
		bool	getRetryInterruptedFcntl() const;

		/** Causes ioctl operations to be automatically retried if
		 *  interrupted by a signal.  This is the default behavior.
		 *  Otherwise, if a ioctl is occurring and a signal interrupts
		 *  it, the ioctl fails, the system error is set to EINTR and
		 *  the ioctl must be retried. */
		void	retryInterruptedIoctl();

		/** Causes ioctl operations not to be automatically retried if
		 *  interrupted by a signal.  If this is set, a ioctl is
		 *  occurring and a signal interrupts it, the ioctl fails, the
		 *  system error is set to EINTR and the ioctl must be
		 *  retried. */
		void	dontRetryInterruptedIoctl();

		/** Returns true if interrupted calls to ioctl will be retried
		 *  and false otherwise. */
		bool	getRetryInterruptedIoctl() const;


		/** By default, read() will attempt to read the specified number
		 *  of bytes from the file descriptor, even if several passes
		 *  are necessary.
		 *
		 *  This method causes a read() to attempt only a single pass
		 *  and return the number of bytes that were read during that
		 *  pass whether the specified number of bytes were read or not.
		 *
		 *  Note that reads longer than SSIZE_MAX will only attempt to
		 *  read SSIZE_MAX bytes. */
		void	allowShortReads();

		/** By default, read() will attempt to read the specified number
		 *  of bytes from the file descriptor, even if several passes
		 *  are necessary.
		 *
		 *  This method causes that default behavior to be observed. */
		void	dontAllowShortReads();

		/** By default, write() will attempt to write the specified
		 *  number of bytes from the file descriptor, in several passes
		 *  if necessary.
		 *
		 *  This method causes a write() to attempt only a single pass
		 *  and return the number of bytes that were written during
		 *  that pass whether the specified number of bytes were
		 *  written or not.
		 *
		 *  Note that writes longer than SSIZE_MAX will only attempt to
		 *  write SSIZE_MAX bytes. */
		void	allowShortWrites();

		/** By default, write() will attempt to write the specified
		 *  number of bytes from the file descriptor, even if several
		 *  passes are necessary.
		 *
		 *  This method causes that default behavior to be observed. */
		void	dontAllowShortWrites();


		/** Sends file descriptor "fd" to the file descriptor.  This
		 *  is useful for passing an open file descriptor from one
		 *  process to another over a unix socket, for example. */
		virtual bool	passFileDescriptor(int32_t fd) const;

		/** Receives a file descriptor into buffer "fd".  This
		 *  is useful for receiving an open file descriptor passed
		 *  from another process over a unix socket, for example. */
		virtual bool	receiveFileDescriptor(int32_t *fd) const;


		/** Sends socket "sock" to the file descriptor.  This
		 *  is useful for passing an open socket from one
		 *  process to another over a unix socket, for example. */
		virtual bool	passSocket(int32_t sock) const;

		/** Receives a socket into buffer "sock".  This is useful for
		 *  receiving an open file descriptor passed from another
		 *  process over a unix socket, for example. */
		virtual bool	receiveSocket(int32_t *sock) const;


		/** Translate integers from native byte order to network byte
		 *  order during writes and vice-versa during reads.  By
		 *  default, no translation is done. */
		void	translateByteOrder();

		/** Don't translate integers from native byte order to network
		 *  byte order during writes and vice-versa during reads.  This
		 *  is the default behavior. */
		void	dontTranslateByteOrder();

		/** Use the fcntl() system call to perform various low-level
		 *  file descriptor operations. */
		virtual int32_t	fCntl(int32_t command, long arg) const;

		/** Use the ioctl() system call to perform various low-level
		 *  file descriptor operations. */
		virtual int32_t	ioCtl(int32_t command, void *arg) const;

		/** Causes small write()'s to be collected up and sent together
		 *  when either the kernel's write buffer is full or when a
		 *  maximum of 0.2 seconds has gone by.
		 *  (enable Nagle's algorithm)
		 * 
		 *  This is the default.
		 * 
		 *  Returns true on success and false on failure. */
		bool	useNaglesAlgorithm();

		/** Causes all write()'s to be sent immediately.
		 *  (disables Nagle's algorithm)
		 * 
		 *  Returns true on success and false on failure. */
		bool	dontUseNaglesAlgorithm();

		/** Sets the size of the kernel's TCP write buffer to "size"
		 *  bytes.  This is only useful for TCP socket file
		 *  descriptors.  Returns true on success and false on
		 *  failure. */
		bool	setTcpWriteBufferSize(int32_t size);

		/** Fetches the size of the kernel's TCP write buffer (in bytes)
		 *  into buffer "size".  This is only useful for TCP socket
		 *  file descriptors.  Returns true on success and false on
		 *  failure. */
		bool	getTcpWriteBufferSize(int32_t *size);

		/** Sets the size of the kernel's TCP read buffer to "size"
		 *  bytes.  This is only useful for TCP socket file
		 *  descriptors.  Returns true on success and false on
		 *  failure. */
		bool	setTcpReadBufferSize(int32_t size);

		/** Fetches the size of the kernel's TCP read buffer (in bytes)
		 *  into buffer "size".  This is only useful for TCP socket
		 *  file descriptors.  Returns true on success and false on
		 *  failure. */
		bool	getTcpReadBufferSize(int32_t *size);

		/** Disables IPv4 on this file descriptor, only IPv6 will be
		 *  used.  This is only useful for TCP Socket file descriptors.
		 *  Returns true on success and false on failure. */
		bool	disableIPv4();

		/** Enables IPv4 (in addition to IPv6) on this file descriptor
		 *  (the default).  This is only useful for TCP Socket file
		 *  descriptors.  Returns true on success and false on
		 *  failure. */
		bool	enableIPv4();

		/** Returns a string representing the type of the
		 *  filedescriptor.  This is "filedescriptor" by default but
		 *  a child class may override this method and return
		 *  something else. */
		const char	*getType() const;

		/** Returns the IP address of the client at the other end of the
		 *  connection if the filedescriptor is an inet socket or NULL
		 *  otherwise.
		 * 
		 *  Note that the buffer for the address is allocated internally
		 *  and must be freed by the calling program. */
		char	*getPeerAddress() const;


		/** If an application does many small writes, the overhead of
		 *  all of those system calls can slow the application down
		 *  substantially.  To address that issue, the filedescriptor
		 *  class can buffer data passed in to any of it's write()
		 *  methods and only make system calls when the buffer is full
		 *  or when it's flushed manually.  Note that when using
		 *  buffered writes, no data is actually written to the file
		 *  descriptor until the buffer is full or until it's flushed
		 *  manually.
		 *
		 *  Do not confuse this buffer with the tcp write buffer.  The
		 *  tcp write buffer resides in kernel space, is populated by
		 *  the write() system call and is used to minimize network
		 *  latency, not application latency due to system calls.
		 *  This buffer is in user space and populated prior to the
		 *  write() system call.
		 *
		 *  This method sets the write buffer size to "size" bytes.
		 *  A size of 0 means not to buffer writes at all.
		 *
		 *  Returns true on success and false on failure. */
		bool	setWriteBufferSize(ssize_t size) const;

		/** If an application does many small writes, the overhead of
		 *  all of those system calls can slow the application down
		 *  substantially.  To address that issue, the filedescriptor
		 *  class can buffer data passed in to any of it's write()
		 *  methods and only make system calls when the buffer is full
		 *  or when it's flushed manually.  Note that when using
		 *  buffered writes, no data is actually written to the file
		 *  descriptor until the buffer is full or until it's flushed
		 *  manually.
		 *
		 *  Do not confuse this buffer with the tcp write buffer.  The
		 *  tcp write buffer resides in kernel space, is populated by
		 *  the write() system call in an analagous manner and is used
		 *  to minimize network latency, not application latency due to
		 *  system calls.  This buffer is in user space and populated
		 *  prior to the write() system call.
		 *
		 *  This method causes the contents of the write buffer to
		 *  be written to the filedescriptor immediately.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  If the buffer cannot be flushed within "sec" seconds and
		 *  "usec" microseconds, then the method will fail.  Set either
		 *  "sec" or "usec" to -1 to disable the timeout and allow the
		 *  method to block until the buffer has been flushed. */
		bool	flushWriteBuffer(int32_t sec, int32_t usec) const;


		/** If an application does many small reads, the overhead of
		 *  all of those system calls can slow the application down
		 *  substantially.  To address that issue, the filedescriptor
		 *  class can create a read buffer and attempt to keep it full.
		 *  When the first read is attempted, it will attempt to read
		 *  "size" bytes into the buffer and only return the number of
		 *  bytes specified in the read.  Subsequent reads will just
		 *  return data from the buffer without doing additional
		 *  system calls unless the buffer is empty.
		 *
		 *  Do not confuse this buffer with the tcp read buffer.  The
		 *  tcp read buffer resides in kernel space, is populated by
		 *  the read() system call in an analagous manner and is used
		 *  to minimize network latency, not application latency due to
		 *  system calls.  This buffer is in user space and populated
		 *  in the manner described above.
		 *
		 *  This method sets the read buffer size to "size" bytes and
		 *  returns true on success and false on failure. */
		bool	setReadBufferSize(ssize_t size) const;

		/** If the close-on-exec status is false (the default), then the
		 *  file descriptor will remain open across an execve() call,
		 *  otherwise it will be closed.
		 * 
		 *  This method sets the close-on-exec status to true. */
		bool	closeOnExec();

		/** If the close-on-exec status is false (the default), then the
		 *  file descriptor will remain open across an execve() call,
		 *  otherwise it will be closed.
		 * 
		 *  This method sets the close-on-exec status to false
		 *  (the default). */
		bool	dontCloseOnExec();

		/** If the close-on-exec status is false (the default), then the
		 *  file descriptor will remain open across an execve() call,
		 *  otherwise it will be closed.
		 * 
		 *  This method returns true if the close-on-exec status is set
		 *  to true and false otherwise. */
		bool	getCloseOnExec();

		/** Converts an unsigned 16-bit integer from host byte order to
		 *  network byte order. */
		static	uint16_t	hostToNet(uint16_t value);

		/** Converts an unsigned 32-bit integer from host byte order to
		 *  network byte order. */
		static	uint32_t	hostToNet(uint32_t value);

		/** Converts an unsigned 64-bit integer from host byte order to
		 *  network byte order. */
		static	uint64_t	hostToNet(uint64_t value);

		/** Converts an unsigned 16-bit integer from network byte order
		 * to host byte order. */
		static	uint16_t	netToHost(uint16_t value);

		/** Converts an unsigned 32-bit integer from network byte order
		 * to host byte order. */
		static	uint32_t	netToHost(uint32_t value);

		/** Converts an unsigned 64-bit integer from network byte order
		 * to host byte order. */
		static	uint64_t	netToHost(uint64_t value);

	#include <rudiments/private/filedescriptor.h>
};

#endif
