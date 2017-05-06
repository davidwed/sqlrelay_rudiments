// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_TLS_H
#define RUDIMENTS_TLS_H

#include <rudiments/private/tlsincludes.h>

class RUDIMENTS_DLLSPEC tls {
	public:
		/** Returns true if rudiments was built with TLS/SSL support
		 *  and false otherwise. */
		static	bool	supported();

	#include <rudiments/private/tls.h>
};

class RUDIMENTS_DLLSPEC tlscontext : public securitycontext {
	public:
		/** Creates a new instance of the tlscontext class. */
		tlscontext();

		/** Deletes this instance of the tlscontext class. */
		virtual	~tlscontext();

		/** Sets the protocol version to use during the next call to
		 *  connect() or accept().
		 *
		 *  Valid values include SSL2, SSL3, TLS1, TLS1.1, TLS1.2 or
		 *  any more recent version of TLS, as supported by and enabled
		 *  in the underlying TLS/SSL library.  If left blank or empty
		 *  then the highest supported version will be negotiated. */
		void	setProtocolVersion(const char *version);

		/** Returns the protocol version that will be used during the
		 *  next call to connect() or accept(). */
		const char	*getProtocolVersion();

		/** Sets the location of the certificate chain file to use
		 *  during the next call to connect() or accept().
		 *
		 *  If "filename" is NULL or empty then no certificate will
		 *  be sent to the peer.
		 *
		 *  Otherwise...
		 *
		 *
		 *  On non-Windows platforms, "filename" must refer to an
		 *  actual file.  On Windows platforms, it may refer to an
		 *  actual file or to a certificate found in a Windows
		 *  Certificate Store.
		 *  
		 *
		 *  Actual files must contain the client's certificate and
		 *  the chain of signing certificates, terminating in a
		 *  certificate for a root certificate authority.  On Windows
		 *  platforms, the file must also contain the client's private
		 *  key.  On non-Windows platforms, the private key may be
		 *  stored in a separate file, specified by setPrivateKeyFile().
		 *  
		 *  Note that the supported file formats may vary between
		 *  platforms.  A variety of formats are generally supported
		 *  on Linux and Unix platforms (.pem, .pfx, etc.) but only
		 *  .pfx files are currently supported on Windows.
		 *
		 *
		 *  Certificates in a Windows Certificate Store must have an
		 *  associated private key and associated chain of signing
		 *  certificates, terminating in a certificate for a root
		 *  certificate authority.
		 *
		 *  To specify an entry in a Windows Certificate Store,
		 *  "filename" must be specified in one of the following
		 *  formats:
		 * 	location:store:subject
		 * 	store:subject
		 * 	subject
		 *
		 *  The "location" parameter identifies the certificate store
		 *  location, and must be one of the following:
		 * 	CURRENT_USER
		 *	LOCAL_MACHINE
		 *	CURRENT_SERVICE
		 *	SERVICES
		 *	USERS
		 *	CURRENT_USER_GROUP_POLICY
		 *	LOCAL_MACHINE_GROUP_POLICY
		 *	LOCAL_MACHINE_ENTERPRISE
		 *  If "location" is omitted then it defaults to CURRENT_USER.
		 *
		 *  The "store" parameter identifies the certificate store, and
		 *  must be one of the following:
		 *  	MY
		 *  	Root
		 *  	Trust
		 *  	CA
		 *  If "store" is omitted then it defaults to MY.
		 *
		 *  The "subject" parameter identifies the certificate.  The
		 *  first certificate in the specified location/store who's
		 *  Subject contains "subject" (case-insensitive) will be used.
		 *  Note that the order of the certificates in the store is not
		 *  guaranteed, so "subject" should contain enough information
		 *  to uniquely identify a certificate.
		 */
		void		setCertificateChainFile(const char *filename);

		/** Returns the location of the certificate chain file that
		 *  will be used during the next call to connect() or
		 *  accept(). */
		const char	*getCertificateChainFile();

		/** Ignored on Windows platforms.
		 *
		 *  On non-Windows platforms:
		 *
		 *  Sets the location of the private key file to use
		 *  during the next call to connect() or accept().
		 *
		 *  If no private key file is specified via this call, either
		 *  because the call is omitted, or because "filename" is
		 *  NULL or empty, then the certificate chain file will be
		 *  searched for the private key.
		 *  
		 *  Note that the supported file formats may vary between
		 *  platforms.  A variety of formats are generally supported
		 *  on Linux and Unix platforms (.pem, .pfx, etc.) but only
		 *  the .pfx format is currently supported on Windows. */
		void		setPrivateKeyFile(const char *filename);

		/** Returns the location of the private key file that
		 *  (on non-Windows platforms) will be used during the
		 *  next call to connect() or accept(). */
		const char	*getPrivateKeyFile();

		/** Sets the password to use when accessing the private key
		 *  in the certificate chain file during the next call to
		 *  connect() or accept().
		 *
		 *  Note: password-protected private keys are currently
		 *  not supported on Windows platforms. */
		void		setPrivateKeyPassword(const char *password);

		/** Returns the password that will be used to access the
		 *  private key in the certificate chain file during the next
		 *  call to connect() or accept().
		 *
		 *  Note: password-protected private keys are currently
		 *  not supported on Windows platforms. */
		const char	*getPrivateKeyPassword();

		/** Sets the list of ciphers to allow during the next call to
		 *  connect() or accept().  Ciphers may be separated by spaces,
		 *  commas, or colons.   If "ciphers" is NULL or empty then
		 *  a default set of ciphers will be used.
		 *
		 *  For a list of valid ciphers on Linux and Unix platforms,
		 *  see:  man ciphers
		 *
		 *  For a list of valid ciphers on Windows platforms, see:
		 *  https://msdn.microsoft.com/en-us/library/windows/desktop/aa375549%28v=vs.85%29.aspx
		 *  On Windows platforms, the ciphers (alg_id's) should omit
		 *  CALG_ and may be given with underscores or dashes.
		 *  For example: 3DES_112 */
		void		setCiphers(const char *ciphers);

		/** Returns the list of ciphers that will be allowed during the
		 *  next call to connect() or accept(). */
		const char	*getCiphers();

		/** Instructs the next call to connect() or accept() whether
		 *  or not to validate the peer's certificate.  Defaults to
		 *  true for connect() and false for accept(). */
		void		setValidatePeer(bool validatepeer);

		/** Returns whether or not the peer's certificate was explicitly
		 *  configured to be validated during the next call to connect()
		 *  or accept(). */
		bool		getValidatePeer();

		/** Sets the validation depth to use when validating the peer's
		 *  certificate during the next call to connect() or accept().
		 * 
		 *  There could be any number of intermediate signing
		 *  authorities between the peer's certificate and a top-level
		 *  certificate authority.
		 *
		 *  For example, the certificate chain could consist of:
		 *      * peer's certificate (signed by A)
		 *      * A's certificate (signed by B)
		 *      * B's certificate (signed by C)
		 *      * ...
		 *      * Y's certificate (signed by a top-level authority)
		 *      * top-level authority's certificate
		 *
		 *  Setting the validation depth instructs the context to
		 *  only allow "depth" certificates between the peer
		 *  certificate and a top-level authority.
		 *
		 *  The default, and maximum depth is 9.  Setting a depth
		 *  greater than 9 has the same effect as setting it to 9.
		 *  Setting the depth to 0 also has the same effect as setting
		 *  it to 9. */
		void		setValidationDepth(uint16_t depth);
		
		/** Returns the validation depth that will be used when
		 *  validating the peer's certificate during the next call to
		 *  connect() or accept(). */
		uint16_t	getValidationDepth();

		/** Sets the location of the certificate authority to use when
		 *  validating the peer's certificate during the next call
		 *  to connect() or accept().
		 *
		 *  On Windows, "ca" must be a file name.
		 *
		 *  On non-Windows systems, "ca" can be either a file or
		 *  directory name.  If it is a directory name, then all
		 *  certificate authority files found in that directory will be
		 *  used.  If it a file name, then only that file will be used.
		 *
		 *  If "ca" is NULL or empty then no validation of the peer
		 *  certificate will occur during the next call to connect() or
		 *  accept(). */

		/** Sets the location of the certificate authority to use when
		 *  validating the peer's certificate during the next call
		 *  to connect() or accept().
		 *
		 *  If "ca" is NULL or empty then no validation of the peer
		 *  certificate will occur during the next call to connect() or
		 *  accept().
		 *
		 *  Otherwise...
		 *
		 *
		 *  On non-Windows systems, "ca" can be either a file name or
		 *  directory name.  If it a file name, then only that file
		 *  will be used.  If it is a directory name, then all
		 *  certificate authority files found in that directory will
		 *  be used.  On Windows platforms, "ca" may refer to a file or
		 *  to a certificate found in a Windows Certificate Store.
		 *  
		 *
		 *  Note that the supported file formats may vary between
		 *  platforms.  A variety of formats are generally supported
		 *  on Linux and Unix platforms (.pem, .pfx, etc.) but only
		 *  .pfx files are currently supported on Windows.
		 *
		 *  To specify an entry in a Windows Certificate Store,
		 *  "ca" must be specified in one of the following formats:
		 * 	location:store:subject
		 * 	store:subject
		 * 	subject
		 *
		 *  The "location" parameter identifies the certificate store
		 *  location, and must be one of the following:
		 * 	CURRENT_USER
		 *	LOCAL_MACHINE
		 *	CURRENT_SERVICE
		 *	SERVICES
		 *	USERS
		 *	CURRENT_USER_GROUP_POLICY
		 *	LOCAL_MACHINE_GROUP_POLICY
		 *	LOCAL_MACHINE_ENTERPRISE
		 *  If "location" is omitted then it defaults to CURRENT_USER.
		 *
		 *  The "store" parameter identifies the certificate store, and
		 *  must be one of the following:
		 *  	MY
		 *  	Root
		 *  	Trust
		 *  	CA
		 *  If "store" is omitted then it defaults to MY.
		 *
		 *  The "subject" parameter identifies the CA certificate.  The
		 *  first certificate in the specified location/store who's
		 *  Subject contains "subject" (case-insensitive) will be used.
		 *  Note that the order of the certificates in the store is not
		 *  guaranteed, so "subject" should contain enough information
		 *  to uniquely identify a CA certificate.
		 */
		void		setCertificateAuthority(const char *ca);

		/** Returns the location of the certificate authority that
		 *  will be used when validating the peer's certificate during
		 *  the next call to connect() or accept(). */
		const char	*getCertificateAuthority();

		/** Sets the file descriptor that will be used to communicate
		 *  with the peer during the next call to connect() or
		 *  accept(). */
		void		setFileDescriptor(filedescriptor *fd);

		/** Returns the file descrptor that will be used to communicate
 		 *  with the peer during the next call to connect() or
 		 *  accept(). */
		filedescriptor	*getFileDescriptor();


		/** Initiates a security context with the server with whom
		 *  a connection is already established across the
		 *  filedescriptor previously set using setFileDescriptor().
		 *
		 *  Note that if this instance is set as the current security
		 *  context of a child of the socketclient class, then this
		 *  method is called implicitly during a successful call to
		 *  connect().
		 *
		 *  Returns true on success and false on failure. */
		bool	connect();


		/** Accepts a security context from the client with whom
		 *  a connection is already established across the
		 *  filedescriptor previously set using setFileDescriptor().
		 *
		 *  Note that if this instance is set as the current GSS
		 *  context of a child of the socketserver class, then this
		 *  method is called implicitly during a successful call to
		 *  accept().
		 *
		 *  Returns true on success and false on failure. */
		bool	accept();

		/** Returns the certificate sent by the peer during the most
		 *  recent call to connect() or accept() or NULL if the peer
		 *  didn't send a certificate.
		 *  Note that this method allocates an instance of
		 *  tlscertificate internally and returns it.  The calling
		 *  program must delete this instance. */
		tlscertificate	*getPeerCertificate();

		/** Reads from the file descriptor previously configured by
		 *  setFileDescriptor() into "buf" until "count" bytes have
		 *  been read.  Returns the number of bytes that were written
		 *  to "buf" or RESULT_ERROR if an error occurred. */
		ssize_t	read(void *buf, ssize_t count);

		/** Writes "count" bytes from "buf" to the file descriptor
		 *  previously configured by setFileDescriptor().
		 *  Returns the number of bytes that were written or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	write(const void *buf, ssize_t count);

		/** Returns the number of bytes that are buffered and available
		 *  for immediate read. */
		ssize_t pending();

		/** Releases any security context established during the
		 *  previous call to connect() or accept(). */
		bool	close();


		/** Returns the maximum size that can be written or read in
		 *  a single call to write() or read(). */
		ssize_t	getSizeMax();


		/** Returns the error code of the most recently failed call. */
		int32_t		getError();

		/** Returns the error string of the most recently failed
		 *  call. */
		const char	*getErrorString();

	#include <rudiments/private/tlscontext.h>
};

class RUDIMENTS_DLLSPEC tlscertificate {
	public:
		/** Creates a new instance of the tlscertificate class. */
		tlscertificate();

		/** Deletes this instance of the tlscertificate class. */
		~tlscertificate();

		/** Returns the version from the certificate. */
		uint32_t	getVersion();

		/** Returns the serial number from the certificate. */
		uint64_t	getSerialNumber();

		/** Returns the signature algorithm from the certificate. */
		const char	*getSignatureAlgorithm();

		/** Returns the issuer from the certificate. */
		const char	*getIssuer();

		/** Returns the date the at the certificate is valid from. */
		datetime	*getValidFrom();

		/** Returns the date the at the certificate is valid to. */
		datetime	*getValidTo();

		/** Returns the subject from the certificate. */
		const char	*getSubject();

		/** Returns the common name from the certificate. */
		const char	*getCommonName();

		/** Returns the public key algorithm from the certificate. */
		const char	*getPublicKeyAlgorithm();

		/** Returns the public key from the certificate. */
		const unsigned char	*getPublicKey();

		/** Returns the length of the public key, in bytes,
		 *  from the certificate. */
		uint64_t	getPublicKeyByteLength();

		/** Returns the length of the public key, in bits,
		 *  from the certificate. */
		uint64_t	getPublicKeyBitLength();

		/** Returns the subject alternate names from the certificate. */
		linkedlist< char * >	*getSubjectAlternateNames();

	#include <rudiments/private/tlscertificate.h>
};

#endif
