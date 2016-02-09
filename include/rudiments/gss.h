// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_GSS_H
#define RUDIMENTS_GSS_H

#include <rudiments/private/gssincludes.h>

class RUDIMENTS_DLLSPEC gss {
	public:
		/** Creates a new instance of the gss framework. */
		gss();

		/** Deletes this instance of the gss framework. */
		~gss();

		/** Returns the available underlying security mechanisms as
		 *  a NULL-terminated array.   The members of the array
		 *  are string representations of the object ids for each
		 *  security mechanism.
		 *
		 *  For example:
		 *    { 1 2 840 113554 1 2 2 }
		 *    { 1 3 5 1 5 2 }
		 *    { 1 2 840 48018 1 2 2 }
		 *    { 1 3 6 1 5 2 5 }
		 *    { 1 3 6 1 5 5 2 }
		 *
		 *  These strings may be passed into
		 *  gssmechanism::initialize(const char *str). */
		const char * const	*getAvailableMechanisms();

		/** Returns true if rudiments was built with GSS support and
		 *  false otherwise. */
		static	bool	supportsGSS();

	#include <rudiments/private/gss.h>
};


class RUDIMENTS_DLLSPEC gssmechanism {
	public:
		/** Creates a new instance of the gssmechanism class. */
		gssmechanism();

		/** Deletes this instance of the gssmechanism class. */
		~gssmechanism();

		/** Initializes this instance of the gssmechanism class
		 *  using "str", which should be the string representation
		 *  of the object id for an available security mechanism.
		 *  Returns true on success and false on failure. */
		bool	initialize(const char *str);

		/** Initializes this instance of the gssmechanism class
		 *  using "oid", which should be the platform-spcific binary
		 *  representation of the object id for an available security
		 *  mechanism.
		 *  Returns true on success and false on failure. */
		bool	initialize(const void *oid);

		/** Clears any previous initialization of this intstance. */
		void	clear();

		/** Returns the string representation of the object id for the
		 *  security mechanism represented by this instance, or NULL
		 *  if it has not been initialized or has been cleared. */
		const char	*getString();

		/** Returns the platform-specific binary representation of the
		 *  object id for the security mechanism represented by this
		 *  instance, or NULL if it has not been initialized or has
		 *  been cleared. */
		const void	*getObjectId();

	#include <rudiments/private/gssmechanism.h>
};


class RUDIMENTS_DLLSPEC gsscredentials {
	public:
		/** Creates an instance of the gsscredentials class. */
		gsscredentials();

		/** Deletes this instance of the gsscredentials class. */
		~gsscredentials();

		/** Sets the location of the keytab to use when acquiring
		 *  credentials. */
		bool		setKeytab(const char *keytab);

		/** Returns the location of the keytab that will be used
		 *  when acquiring credentials. */
		const char	*getKeytab();

		/** Sets the credentials-lifetime that will be requested during
		 *  subsequent calls to the acquire methods.
		 *  Setting "desiredlifetime" to the largest 32-bit unsigned
		 *  integer requests an indefinite lifetime. */
		void		setDesiredLifetime(uint32_t desiredlifetime);

		/** Returns the desired credentials-lifetime set by a previous
		 *  call to setDesiredLifetime() or the largest 32-bit integer
		 *  (indicating an indefinite lifetime, the default) if no
		 *  desired credentials-lifetime has been set. */
		uint32_t	getDesiredLifetime();

		/** Adds security mechanism "mech" to the list of security
		 *  mechanisms that will be requested during a subsequent call
		 *  to one of the acquire methods. */
		void		addDesiredMechanism(gssmechanism *mech);

		/** Removes security mechanism "mech" from the list of security
		 *  mechanisms that will be requested during a subsequent call
		 *  to one of the acquire methods. */
		void		removeDesiredMechanism(gssmechanism *mech);

		/** Returns true if security mechanism "mech" is in the list of
		 *  security mechanisms that will be requested during a
		 *  subsequent call to one of the acquire methods and false
		 *  otherwise. */
		bool		inDesiredMechanisms(gssmechanism *mech);

		/** Removes all security mechanism from the list of security
		 *  mechanisms that will be requested during a subsequent call
		 *  to one of the acquire methods. */
		void		clearDesiredMechanisms();

		/** Returns the number of security mechanisms in the list of
		 *  security mechanisms that will be requested during a
		 *  subsequent call to one of the acquire methods. */
		uint64_t	getDesiredMechanismCount();

		/** Returns the "index"'th security mechanism from the list of
		 *  security mechanisms that will be requested during a
		 *  subsequent call to one of the acquire methods. */
		gssmechanism *getDesiredMechanism(uint64_t index);


		/** Acquires credentials for service principal "name".
		 *  Returns true on success and false on failure. */
		bool	acquireForService(const char *name);

		/** Acquires credentials for user principal "name".
		 *  Returns true on success and false on failure. */
		bool	acquireForUser(const char *name);

		/** Acquires credentials for kerberos principal "name".
		 *  Returns true on success and false on failure. */
		bool	acquireForKrbPrincipal(const char *name);

		/** Acquires credentials for an anonymous principal.
		 *  Returns true on success and false on failure. */
		bool	acquireForAnonymous();


		/** Returns true if credentials are currently
		 *  acquired and false otherwise. */
		bool	acquired();


		/** Releases any previously acquired credentials. */
		void	release();


		/** Returns the name for which credentials were requested in
		 *  the most recent call to acquire, if the name was provided
		 *  as a string.  Returns NULL if no call to an acquire method
		 *  has been made, or if release() has been called. */
		const char	*getName();

		/** Returns the actual lifetime of the credentials, assigned
		 *  during the most recent call to an aquire method.  Returns
		 *  the largest unsigned 32-bit integer (indicating an
		 *  indefinite lifetime, the default) if no call to an acquire
		 *  method has been made, or if release() has been called. */
		uint32_t	getActualLifetime();

		/** Returns true if security mechanism "mech" is in the list of
		 *  security mechanisms that was actually used during the
		 *  most recent call to one of the acquire methods and false
		 *  otherwise. */
		bool		inActualMechanisms(gssmechanism *mech);

		/** Returns the number of security mechanisms in the list of
		 *  security mechanisms that were actually used during the
		 *  most recent call to one of the acquire methods or
		 *  0 if no call to an acquire method has been made, or if
		 *  release() has been called. */
		uint64_t	getActualMechanismCount();

		/** Returns the "index"'th security mechanism from the list of
		 *  security mechanisms that were actually used during the
		 *  most recent call to one of the acquire methods or NULL
		 *  if no call to an acquire method has been made, or if
		 *  release() has been called. */
		gssmechanism	*getActualMechanism(uint64_t index);


		/** Returns the major-status code of the most recently failed
		 *  call. */
		uint32_t	getMajorStatus();

		/** Returns the minor-status code of the most recently failed
		 *  call. */
		uint32_t	getMinorStatus();

		/** Returns the status string of the most recently failed
		 *  call. */
		const char	*getStatus();

		/** Returns the GSS major status string of the most
		 *  recently failed call. */
		const char	*getGSSMajorStatus();

		/** Returns the GSS minor status string of the most
		 *  recently failed call. */
		const char	*getGSSMinorStatus();

		/** Returns the mechanism major status string of the most
		 *  recently failed call. */
		const char	*getMechanismMajorStatus();

		/** Returns the mechanism minor status string of the most
 		 *  recently failed call. */
		const char	*getMechanismMinorStatus();


		/** Returns a platform-specific binary internal representation
		 *  of the credentials, or NULL if no call has been made to an
		 *  acquire method, or if release() has been called. */
		const void	*getCredentials();

	#include <rudiments/private/gsscredentials.h>
};


class RUDIMENTS_DLLSPEC gsscontext {
	public:
		/** Creates an instance of the gsscontext class. */
		gsscontext();

		/** Deletes this instance of the gsscontext class. */
		~gsscontext();
		
		/** Sets the credentials that will be used during subsequent
		 *  calls to one of initiate() or accept(). */
		void	setCredentials(gsscredentials *credentials);

		/** Returns the credentials set by a previous call to
		 *  setCredentials() or NULL if no credentials have been set. */
		gsscredentials	*getCredentials();

		/** Sets the filedescriptor that will be used during subsequent
		 *  calls to one of initiate() or accept().
		 *
		 *  Note that if this instance is set as the current GSS
		 *  context of a child of the socketclient class, then this
		 *  method is called implicitly during a successful call to
		 *  connect().
		 *
		 *  Note also that if this instance is set as the current
		 *  GSS context of a child of the socketserver class, then
		 *  this method is called implicitly during a successful call
		 *  to accept().
		 *  */
		void	setFileDescriptor(filedescriptor *fd);

		/** Returns the filedscriptor by a previous call to
		 *  setFileDescriptor() or NULL if no filedescriptor has been
		 *  set. */
		filedescriptor	*getFileDescriptor();

		/** Sets the context-lifetime that will be requested during
		 *  subsequent calls to initiate() or accept().
		 *  Setting "desiredlifetime" to the largest 32-bit unsigned
		 *  integer requests an indefinite lifetime. */
		void	setDesiredLifetime(uint32_t desiredlifetime);

		/** Returns the desired context-lifetime set by a previous call
		 *  to setDesiredLifetime() or the largest 32-bit integer
		 *  (indicating an indefinite lifetime, the default) if no
		 *  desired context-lifetime has been set. */
		uint32_t	getDesiredLifetime();

		/** Sets the security mechanism that will be requested during
		 *  subsequent calls to initiate() or accept(). */
		void	setDesiredMechanism(gssmechanism *desiredmechanism);

		/** Returns the security mechanism set by a previous call to
		 *  setDesiredMechanism() or NULL if no security mechanism has
		 *  been set. */
		gssmechanism	*getDesiredMechanism();

		/** Sets the context-flags that will be requested during
		 *  subsequent calls to initiate() or accept(). */
		void	setDesiredFlags(uint32_t desiredflags);

		/** Sets the context-flags that will be requested during
		 *  subsequent calls to initiate() or accept(). */
		void	setDesiredFlags(const char *desiredflags);

		/** Returns the context-flags set by a previous call to
		 *  setDesiredFlags() or 0 if no flags have been set. */
		uint32_t	getDesiredFlags();

		/** Sets the service that will be requested during subsequent
 		 *  calls to initiate(). */
		void	setService(const char *service);

		/** Returns the service that will be requested during
 		 *  subsequent calls to initiate(). */
		const char	*getService();


		/** Initiates a context with service principal "name" with
		 *  whom a connection is established across the filedescriptor
		 *  previously set using setPeer().
		 *
		 *  Note that if this instance is set as the current GSS
		 *  context of a child of the socketclient class, then this
		 *  method is called implicitly during a successful call to
		 *  connect().
		 *
		 *  Returns true on success and false on failure. */
		bool	initiate();


		/** Accepts a security context from a principal with whom a
		 *  connection is established across the filedescriptor
		 *  previously set using setPeer().
		 *
		 *  Note that if this instance is set as the current GSS
		 *  context of a child of the socketserver class, then this
		 *  method is called implicitly during a successful call to
		 *  accept().
		 *
		 *  Returns true on success and false on failure. */
		bool	accept();


		/** Releases any security context established using an initiate
		 *  or accept method. */
		void	release();


		/** Returns the actual lifetime of the context, assigned
		 *  during the most recent call to an initiate or accept method.
		 *  Returns the largest unsigned 32-bit integer (indicating an
		 *  indefinite lifetime, the default) if no call to an initiate
		 *  or accept method has been made, or if release() has been
		 *  called. */
		uint32_t	getActualLifetime();

		/** Returns the security mechanism that was actually used
		 *  during the most recent call to one of initiate() or
		 *  accept() or NULL if no call to initiate() or accept()
		 *  has been made, or if release() has been called. */
		gssmechanism	*getActualMechanism();

		/** Returns the context flags that were actually used during
		 *  the most recent call to one of initiate() or accept()
		 *  or 0 if no call to an initiate() or accept() method
		 *  has been made, or if release() has been called. */
		uint32_t	getActualFlags();

		/** Returns the remaining lifetime of the context. */
		uint32_t	getRemainingLifetime();


		/** Returns the name of the principal who initiated this
		 *  context. */
		const char	*getInitiator();

		/** Returns the name of the principal who accepted this
		 *  context. */
		const char	*getAcceptor();


		/** Returns true if the calling process was the initator of
		 *  this context and false if it was not. */
		bool	getIsInitiator();

		/** Returns true if the context has been initiated or accepted
		 *  and false otherwise. */
		bool	getIsOpen();


		/** Wraps "input" data of size "inputsize", allocates a buffer
		 *  internally to store the wrapped data, returns a pointer the
		 *  buffer in "output", and returns the size of the buffer in
		 *  "outputsize".
		 *  Returns true on success and false on failure. */
		bool	wrap(const unsigned char *input,
					size_t inputsize,
					unsigned char **output,
					size_t *outputsize);

		/** Wraps "input" data of size "inputsize", allocates a buffer
		 *  internally to store the wrapped data, returns a pointer the
		 *  buffer in "output", and returns the size of the buffer in
		 *  "outputsize".  Attempts to encrypt the data if
		 *  "useencryption" is set to true.  Returns whether encryption
		 *  was performed or not in "encryptionused".
		 *  Returns true on success and false on failure. */
		bool	wrap(const unsigned char *input,
					size_t inputsize,
					bool useencryption,
					unsigned char **output,
					size_t *outputsize,
					bool *encryptionused);


		/** Unwraps "input" data of size "inputsize", allocates a buffer
		 *  internally to store the unwrapped data, returns a pointer
		 *  the buffer in "output", and returns the size of the buffer
		 *  in "outputsize".
		 *  Returns true on success and false on failure. */
		bool	unwrap(const unsigned char *input,
					size_t inputsize,
					unsigned char **output,
					size_t *outputsize);

		/** Unwraps "input" data of size "inputsize", allocates a buffer
		 *  internally to store the unwrapped data, returns a pointer
		 *  the buffer in "output", and returns the size of the buffer
		 *  in "outputsize".  Returns whether decryption was performed
		 *  or not in "decryptionused".
		 *  Returns true on success and false on failure. */
		bool	unwrap(const unsigned char *input,
					size_t inputsize,
					unsigned char **output,
					size_t *outputsize,
					bool *decryptionused);


		/** Generates a mechanism integrity code for "message" of
		 *  size "messagesize", allocates a buffer internally to store
		 *  the code, returns a pointer the buffer in "mic", and
		 *  returns the size of the buffer in "micsize".
		 *  Returns true on success and false on failure. */
		bool	getMic(const unsigned char *message,
					size_t messagesize,
					unsigned char **mic,
					size_t *micsize);

		/** Performs a mechanism integrity code for "message" of
		 *  size "messagesize" and compares it to the data in "mic"
		 *  of size "micsize".  Returns true if they match and false
		 *  otherwise. */
		bool	verifyMic(const unsigned char *message,
					size_t messagesize,
					const unsigned char *mic,
					size_t micsize);


		/** Reads tokens from the file descriptor configured by
		 *  setFileDescriptor(), unwraps them, and writes the unwrapped
		 *  data to "buf" until "count" bytes have been read.
		 *  Returns the number of unwrapped bytes that were written
		 *  to "buf" or RESULT_ERROR if an error occurred. */
		ssize_t	read(void *buf, ssize_t count);

		/** Wraps "buf" of size "count" bytes and writes it to the
		 *  file descriptor set by setFileDescriptor().
		 *  Returns the number of unwrapped bytes that were written
		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	write(const void *buf, ssize_t count);

		/** Returns the number of bytes that are buffered and available
		 *  for immediate read. */
		ssize_t pending();


		/** Returns the major-status code of the most recently failed
		 *  call. */
		uint32_t	getMajorStatus();

		/** Returns the minor-status code of the most recently failed
		 *  call. */
		uint32_t	getMinorStatus();

		/** Returns the status string of the most recently failed
		 *  call. */
		const char	*getStatus();

		/** Returns the GSS major status string of the most
		 *  recently failed call. */
		const char	*getGSSMajorStatus();

		/** Returns the GSS minor status string of the most
		 *  recently failed call. */
		const char	*getGSSMinorStatus();

		/** Returns the mechanism major status string of the most
		 *  recently failed call. */
		const char	*getMechanismMajorStatus();

		/** Returns the mechanism minor status string of the most
 		 *  recently failed call. */
		const char	*getMechanismMinorStatus();
		
	#include <rudiments/private/gsscontext.h>
};

#endif
