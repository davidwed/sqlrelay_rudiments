// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_TLS_H
#define RUDIMENTS_TLS_H

#include <rudiments/private/tlsincludes.h>

class RUDIMENTS_DLLSPEC tls {
	public:
		static	bool	supportsTLS();

	#include <rudiments/private/tls.h>
};

class RUDIMENTS_DLLSPEC tlscontext : public securitycontext {
	public:
		tlscontext();
		virtual	~tlscontext();

		bool	setCertificateChainFile(const char *filename);

		bool	setPrivateKeyFile(const char *filename,
						const char *password);

		bool	setCiphers(const char *ciphers);

		void	verifyPeer();
		void	dontVerifyPeer();

		bool	setCertificateAuthorityFile(const char *cafile);
		bool	setCertificateAuthorityPath(const char *capath);

		bool	setVerificationDepth(uint32_t depth);

		bool	useDiffieHellmanKeyExchange(const char *dhcert);
		
		void	setFileDescriptor(filedescriptor *fd);

		filedescriptor	*getFileDescriptor();

		bool	connect();

		bool	accept();


		bool	peerCertificateIsValid();

		tlscertificate	*getPeerCertificate();


		ssize_t	read(void *buf, ssize_t count);

		ssize_t	write(const void *buf, ssize_t count);

		ssize_t pending();

		bool	close();

		int32_t		getError();

		const char	*getErrorString();

	#include <rudiments/private/tlscontext.h>
};

class RUDIMENTS_DLLSPEC tlscertificate {
	public:
		tlscertificate();
		~tlscertificate();

		const char	*getCommonName();

	#include <rudiments/private/tlscertificate.h>
};

#endif
