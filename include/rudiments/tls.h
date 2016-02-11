// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_TLS_H
#define RUDIMENTS_TLS_H

#include <rudiments/private/tlsincludes.h>

class RUDIMENTS_DLLSPEC tls {
	public:
		/** Returns true if rudiments was built with TLS support and
		 *  false otherwise. */
		static	bool	supportsTLS();

	#include <rudiments/private/tls.h>
};

class RUDIMENTS_DLLSPEC tlscontext : public securitycontext {
	public:
		/** Deletes this instance of the tlscontext class. */
		~tlscontext();

		bool	useCertificateChainFile(const char *filename);

		bool	usePrivateKeyFile(const char *filename,
						const char *password);

		void	setVerifyPeer();

		bool	loadVerifyLocations(const char *cafile,
						const char *capath);

		void	setVerifyDepth(int32_t depth);

		bool	useDiffieHellmanKeyExchange(const char *dhcert);

		tlscertificate	*getPeerCertificate();

		bool	peerCertificateIsValid();
		
		/** Sets the filedescriptor that will be used during subsequent
		 *  calls to one of connect() or accept().
		 *
		 *  Note that if this instance is set as the current TLS
		 *  context of a child of the socketclient class, then this
		 *  method is called implicitly during a successful call to
		 *  connect().
		 *
		 *  Note also that if this instance is set as the current
		 *  TLS context of a child of the socketserver class, then
		 *  this method is called implicitly during a successful call
		 *  to accept().
		 *  */
		void	setFileDescriptor(filedescriptor *fd);

		/** Returns the filedscriptor by a previous call to
		 *  setFileDescriptor() or NULL if no filedescriptor has been
		 *  set. */
		filedescriptor	*getFileDescriptor();

		bool	connect();

		bool	accept();


		/** Reads data from the file descriptor configured by
		 *  setFileDescriptor() into "buf" until "count" bytes have
		 *  been read.  Returns the number of unwrapped bytes that
		 *  were written to "buf" or RESULT_ERROR if an error
		 *  occurred. */
		ssize_t	read(void *buf, ssize_t count);

		/** Writes "count" bytes from "buf" to the file descriptor set
		 *  by setFileDescriptor().  Returns the number of unwrapped
		 *  bytes that were written or RESULT_ERROR if an error
		 *  occurred. */
		ssize_t	write(const void *buf, ssize_t count);

		/** Returns the number of bytes that are buffered and available
		 *  for immediate read. */
		ssize_t pending();

		bool	close();

		int32_t		getError();

		const char	*getErrorString();

	#include <rudiments/private/tlscontext.h>
};

class RUDIMENTS_DLLSPEC tlsclientcontext : public tlscontext {
	public:
		/** Creates an instance of the tlsclientcontext class. */
		tlsclientcontext() : tlscontext(false) {};
};

class RUDIMENTS_DLLSPEC tlsservercontext : public tlscontext {
	public:
		/** Creates an instance of the tlssevercontext class. */
		tlsservercontext() : tlscontext(true) {};
};

class RUDIMENTS_DLLSPEC tlscertificate {
	public:
		/** Creates an instance of the tlscertificate class. */
		tlscertificate();

		/** Deletes this instance of the tlscertificate class. */
		~tlscertificate();

		// X509_NAME_get_text_by_NID
		// X509_get_subject_name
		const char	*getCommonName();

	#include <rudiments/private/tlscertificate.h>
};

#endif
