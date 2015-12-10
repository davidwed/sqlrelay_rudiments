// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_GSSAPI_H
#define RUDIMENTS_GSSAPI_H

#include <rudiments/private/gssapiincludes.h>

class gssapi {
	public:
		/** Creates a new instance of the gssapi framework. */
		gssapi();

		/** Deletes this instance of the gssapi framework. */
		~gssapi();

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
		 *  gssapimechanism::initialize(const char *str). */
		const char * const	*getAvailableMechanisms();

	#include <rudiments/private/gssapi.h>
};


class gssapimechanism {
	public:
		/** Creates a new instance of the gssapimechanism class. */
		gssapimechanism();

		/** Deletes this instance of the gssapimechanism class. */
		~gssapimechanism();

		/** Initializes this instance of the gssapimechanism class
		 *  using "str", which should be the string representation
		 *  of the object id for an available security mechanism.
		 *  Returns true on success and false on failure. */
		bool	initialize(const char *str);

		/** Initializes this instance of the gssapimechanism class
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

	#include <rudiments/private/gssapimechanism.h>
};


class gssapicredentials {
	public:
		/** Creates an instance of the gssapicredentials class. */
		gssapicredentials();

		/** Deletes this instance of the gssapicredentials class. */
		~gssapicredentials();

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
		void		addDesiredMechanism(gssapimechanism *mech);

		/** Removes security mechanism "mech" from the list of security
		 *  mechanisms that will be requested during a subsequent call
		 *  to one of the acquire methods. */
		void		removeDesiredMechanism(gssapimechanism *mech);

		/** Returns true if security mechanism "mech" is in the list of
		 *  security mechanisms that will be requested during a
		 *  subsequent call to one of the acquire methods and false
		 *  otherwise. */
		bool		inDesiredMechanisms(gssapimechanism *mech);

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
		gssapimechanism *getDesiredMechanism(uint64_t index);


		/** Acquires credentials for service principal "name".
		 *  Returns true on success and false on failure. */
		bool	acquireService(const char *name);

		/** Acquires credentials for user principal "name" using
		 *  "password" as the key.  Either "name" or "password" may
		 *  be NULL if they are not required.
		 *  Returns true on success and false on failure. */
		bool	acquireUserName(const char *name,
					const char *password);

		/** Acquires credentials for kerberos principal "name" using
		 *  "password" as the key.  Either "name" or "password" may
		 *  be NULL if they are not required.
		 *  Returns true on success and false on failure. */
		bool	acquireKerberosPrincipalName(const char *name,
							const char *password);

		/** Acquires credentials for an anonymous principal.
		 *  Returns true on success and false on failure. */
		bool	acquireAnonymous();

		/** Acquires credentials for platform-specific user id "uid".
		 *  Returns true on success and false on failure. */
		bool	acquireUid(uid_t uid);

		/** Acquires credentials for platform-specific user id "uid"
		 *  where "uid" is a string representation of the user id.
		 *  Returns true on success and false on failure. */
		bool	acquireUidString(const char *uid);


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
		bool		inActualMechanisms(gssapimechanism *mech);

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
		gssapimechanism	*getActualMechanism(uint64_t index);


		/** Returns the major-status code of the most recently failed
		 *  call. */
		uint32_t	getMajorStatus();

		/** Returns the minor-status code of the most recently failed
		 *  call. */
		uint32_t	getMinorStatus();

		/** Returns the status string of the most recently failed
		 *  call. */
		const char	*getStatus();


		/** Returns a platform-specific binary internal representation
		 *  of the credentials, or NULL if no call has been made to an
		 *  acquire method, or if release() has been called. */
		const void	*getCredentials();

	#include <rudiments/private/gssapicredentials.h>
};


class gssapicontext {
	public:
		/** Creates an instance of the gssapicontext class. */
		gssapicontext();

		/** Deletes this instance of the gssapicontext class. */
		~gssapicontext();
		
		/** Sets the credentials that will be used during subsequent
		 *  calls to one of the initiate or accept methods. */
		void	setCredentials(gssapicredentials *credentials);

		/** Returns the credentials set by a previous call to
		 *  setCredentials() or NULL if no credentials have been set. */
		gssapicredentials	*getCredentials();

		/** Sets the peer filedescriptor (presumably an open socket)
		 *  that will be used during subsequent calls to one of the
		 *  initiate or accept methods. */
		void	setPeer(filedescriptor *peer);

		/** Returns the filedscriptor by a previous call to setPeer()
		 *  or NULL if no filedescriptor has been set. */
		filedescriptor	*getPeer();

		/** Sets the context-lifetime that will be requested during
		 *  subsequent calls to the initiate or accept methods.
		 *  Setting "desiredlifetime" to the largest 32-bit unsigned
		 *  integer requests an indefinite lifetime. */
		void	setDesiredLifetime(uint32_t desiredlifetime);

		/** Returns the desired context-lifetime set by a previous call
		 *  to setDesiredLifetime() or the largest 32-bit integer
		 *  (indicating an indefinite lifetime, the default) if no
		 *  desired context-lifetime has been set. */
		uint32_t	getDesiredLifetime();

		/** Sets the security mechanism that will be requested during
		 *  subsequent calls to the initiate or accept methods. */
		void	setDesiredMechanism(gssapimechanism *desiredmechanism);

		/** Returns the security mechanism set by a previous call to
		 *  setDesiredMechanism() or NULL if no security mechanism has
		 *  been set. */
		gssapimechanism	*getDesiredMechanism();

		/** Sets the context-flags that will be requested during
		 *  subsequent calls to the initiate or accept methods. */
		void	setDesiredFlags(uint32_t desiredflags);

		/** Returns the context-flags set by a previous call to
		 *  setDesiredFlags() or 0 if no flags have been set. */
		uint32_t	getDesiredFlags();


		/** Initiates a context with service principal "name" with
		 *  whom a connection is established across the filedescriptor
		 *  previously set using setPeer().
		 *  Returns true on success and false on failure. */
		bool	initiateToService(const char *name);

		/** Initiates a context with service principal "name" with
		 *  whom a connection is established across the filedescriptor
		 *  previously set using setPeer(), with a timeout of "sec"
		 *  seconds and "usec" microseconds.
		 *  Returns true on success and false on failure. */
		bool	initiateToService(const char *name,
						int32_t sec, int32_t usec);


		/** Accepts a security context from a principal with whom a
		 *  connection is established across the filedescriptor
		 *  previously set using setPeer().
		 *  Returns true on success and false on failure. */
		bool	accept();

		/** Accepts a security context from a principal with whom a
		 *  connection is established across the filedescriptor
		 *  previously set using setPeer(), with a timeout of "sec"
		 *  seconds and "usec" microseconds.
		 *  Returns true on success and false on failure. */
		bool	accept(int32_t sec, int32_t usec);


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
		 *  during the most recent call to one of the initiate or accept
		 *  methods or NULL if no call to an initiate or accept method
		 *  has been made, or if release() has been called. */
		gssapimechanism	*getActualMechanism();

		/** Returns the context flags that were actually used during
		 *  the most recent call to one of the initiate or accept
		 *  methods or 0 if no call to an initiate or accept method
		 *  has been made, or if release() has been called. */
		uint32_t	getActualFlags();

		/** Returns the remaining lifetime of the context. */
		uint32_t	getRemainingLifetime();


		/** Returns the name of the principal who initiated this
		 *  context. */
		const char	*getInitiator();

		/** Returns a string representation of the object id of the
		 *  type of the principal who initiated this context. */
		const char	*getInitiatorType();

		/** Returns the name of the principal who accepted this
		 *  context. */
		const char	*getAcceptor();

		/** Returns a string representation of the object id of the
		 *  type of the principal who accepted this context. */
		const char	*getAcceptorType();


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


		/** Returns the major-status code of the most recently failed
		 *  call. */
		uint32_t	getMajorStatus();

		/** Returns the minor-status code of the most recently failed
		 *  call. */
		uint32_t	getMinorStatus();

		/** Returns the status string of the most recently failed
		 *  call. */
		const char	*getStatus();
		
	#include <rudiments/private/gssapicontext.h>
};

#endif