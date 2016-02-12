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

		void		setCertificateChainFile(const char *filename);
		const char	*getCertificateChainFile();

		void		setPrivateKeyFile(const char *filename,
							const char *password);
		const char	*getPrivateKeyFile();
		const char	*getPrivateKeyPassword();

		void		setCiphers(const char *ciphers);
		const char	*getCiphers();

		void		setVerifyPeer(bool verify);
		bool		getVerifyPeer();

		void		setCertificateAuthorityFile(const char *cafile);
		const char	*getCertificateAuthorityFile();

		void		setCertificateAuthorityPath(const char *capath);
		const char	*getCertificateAuthorityPath();

		void		setVerificationDepth(uint32_t depth);
		uint32_t	getVerificationDepth();

		void		setKeyExchangeCertificate(const char *dhcert);
		const char	*getKeyExchangeCertificate();

		bool	connect();
		bool	accept();
		
		void		setFileDescriptor(filedescriptor *fd);
		filedescriptor	*getFileDescriptor();

		bool		peerCertificateIsValid();
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
