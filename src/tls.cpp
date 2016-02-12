// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#include <rudiments/tls.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_HAS_SSL
	#include <openssl/ssl.h>
	#include <openssl/err.h>
#endif

threadmutex	tls::_tlsmutex;
bool		tls::_initialized=false;

bool tls::supportsTLS() {
	#ifdef RUDIMENTS_HAS_SSL
		return true;
	#else
		return false;
	#endif
}

void tls::initTLS() {
	#ifdef RUDIMENTS_HAS_SSL
		_tlsmutex.lock();
		if (!_initialized) {
			SSL_library_init();
			SSL_load_error_strings();
			_initialized=true;
		}
		_tlsmutex.unlock();
	#endif
}



class tlscontextprivate {
	friend class tlscontext;
	private:
		const char		*_cert;
		const char		*_pvtkey;
		const char		*_pvtkeypwd;
		const char		*_ciphers;
		bool			_verifypeer;
		const char		*_cafile;
		const char		*_capath;
		uint32_t		_depth;
		const char		*_kecert;
		int32_t			_error;
		stringbuffer		_errorstr;
		bool			_dirty;
		#ifdef RUDIMENTS_HAS_SSL
			SSL_CTX		*_ctx;
			SSL		*_ssl;
			BIO		*_bio;
		#endif
		filedescriptor		*_fd;
};

tlscontext::tlscontext() : securitycontext() {
	pvt=new tlscontextprivate;
	pvt->_cert=NULL;
	pvt->_pvtkey=NULL;
	pvt->_pvtkeypwd=NULL;
	pvt->_ciphers=NULL;
	pvt->_verifypeer=NULL;
	pvt->_cafile=NULL;
	pvt->_capath=NULL;
	pvt->_depth=0;
	pvt->_kecert=NULL;
	pvt->_error=0;
	pvt->_errorstr.clear();
	pvt->_dirty=true;
	#ifdef RUDIMENTS_HAS_SSL
		pvt->_bio=NULL;
		pvt->_ssl=NULL;
		pvt->_ctx=NULL;
	#endif
	pvt->_fd=NULL;
}

tlscontext::~tlscontext() {
	#ifdef RUDIMENTS_HAS_SSL
		if (pvt->_ctx) {
			if (pvt->_ssl) {
				// SSL_free frees the BIO too
				SSL_free(pvt->_ssl);
				pvt->_ssl=NULL;
				pvt->_bio=NULL;
			}
			SSL_CTX_free(pvt->_ctx);
			pvt->_ctx=NULL;
		}
	#else
	#endif
}

void tlscontext::setCertificateChainFile(const char *filename) {
	pvt->_dirty=true;
	pvt->_cert=filename;
}

const char *tlscontext::getCertificateChainFile() {
	return pvt->_cert;
}

void tlscontext::setPrivateKeyFile(const char *filename, const char *password) {
	pvt->_dirty=true;
	pvt->_pvtkey=filename;
	pvt->_pvtkeypwd=password;
}

const char *tlscontext::getPrivateKeyFile() {
	return pvt->_pvtkey;
}

const char *tlscontext::getPrivateKeyPassword() {
	return pvt->_pvtkeypwd;
}

void tlscontext::setCiphers(const char *ciphers) {
	pvt->_dirty=true;
	pvt->_ciphers=ciphers;
}

const char *tlscontext::getCiphers() {
	return pvt->_ciphers;
}

void tlscontext::setVerifyPeer(bool verify) {
	pvt->_dirty=true;
	pvt->_verifypeer=verify;
}

bool tlscontext::getVerifyPeer() {
	return pvt->_verifypeer;
}

void tlscontext::setCertificateAuthorityFile(const char *cafile) {
	pvt->_dirty=true;
	pvt->_cafile=cafile;
}

const char *tlscontext::getCertificateAuthorityFile() {
	return pvt->_cafile;
}

void tlscontext::setCertificateAuthorityPath(const char *capath) {
	pvt->_dirty=true;
	pvt->_capath=capath;
}

const char *tlscontext::getCertificateAuthorityPath() {
	return pvt->_capath;
}

void tlscontext::setVerificationDepth(uint32_t depth) {
	pvt->_dirty=true;
	pvt->_depth=depth;
}

uint32_t tlscontext::getVerificationDepth() {
	return pvt->_depth;
}

void tlscontext::setKeyExchangeCertificate(const char *kecert) {
	pvt->_dirty=true;
	pvt->_kecert=kecert;
}

const char *tlscontext::getKeyExchangeCertificate() {
	return pvt->_kecert;
}

void tlscontext::setFileDescriptor(filedescriptor *fd) {
	pvt->_fd=fd;
	#ifdef RUDIMENTS_HAS_SSL
		if (pvt->_bio) {
			BIO_set_fd(pvt->_bio,
				(pvt->_fd)?pvt->_fd->getFileDescriptor():-1,
				BIO_NOCLOSE);
		}
	#endif
}

filedescriptor *tlscontext::getFileDescriptor() {
	return pvt->_fd;
}

bool tlscontext::connect() {
	#ifdef RUDIMENTS_HAS_SSL
		if (!init()) {
			return false;
		}
		int	ret=SSL_connect(pvt->_ssl);
		setError(ret);
		return (ret==1);
	#else
		return false;
	#endif
}

#ifdef RUDIMENTS_HAS_SSL
static int passwdCallback(char *buf, int size, int rwflag, void *userdata) {
	charstring::copy(buf,(char *)userdata,size);
	buf[size-1]=(char)NULL;
	return charstring::length(buf);
}
#endif

bool tlscontext::init() {
	#ifdef RUDIMENTS_HAS_SSL

		if (!pvt->_dirty) {
			return true;
		}

		tls::initTLS();

		close();

		bool	retval=true;

		// We do all of this here, at once, rather than spreading it
		// out over the various methods of the class for various
		// reasons...
		// * We want to allocate SSL_CTX, BIO, or SSL structures
		//   on-demand.
		// * Modifying a SSL_CTX after an SSL has been created from it
		//   can wreck the SSL.  We'd like to be able to call this
		//   class's methods at will and have the changes be applied
		//   at the appropriate time: during the next connect() or
		//   accept().
		// * It's not clear how to restore parameters to their defaults
		//   without just deleting and re-creating the context.  Nor
		//   is it clear whether any method to do it is consistent
		//   across openssl versions or other ssl implementations.

		// create the context
		pvt->_ctx=SSL_CTX_new(
				#if defined(RUDIMENTS_HAS_TLSV1_2_METHOD)
					TLSv1_2_method()
				#elif defined(RUDIMENTS_HAS_TLSV1_1_METHOD)
					TLSv1_1_method()
				#elif defined(RUDIMENTS_HAS_TLSV1_METHOD)
					TLSv1_method()
				#elif defined(RUDIMENTS_HAS_SSLV23_METHOD)
					SSLv23_method()
				#elif defined(RUDIMENTS_HAS_SSLV2_METHOD)
					SSLv2_method()
				#endif
				);

		// set auto-retry mode
		SSL_CTX_set_mode(pvt->_ctx,SSL_MODE_AUTO_RETRY);

		// set certificate chain file
		if (!charstring::isNullOrEmpty(pvt->_cert)) {
			if (SSL_CTX_use_certificate_chain_file(
						pvt->_ctx,pvt->_cert)!=1) {
				retval=false;
			}
		}

		// set private key (and password, if provided)
		if (retval && !charstring::isNullOrEmpty(pvt->_pvtkey)) {
			if (pvt->_pvtkeypwd) {
				SSL_CTX_set_default_passwd_cb(
					pvt->_ctx,passwdCallback);
				SSL_CTX_set_default_passwd_cb_userdata(
					pvt->_ctx,(void *)pvt->_pvtkeypwd);
			}
			if (SSL_CTX_use_PrivateKey_file(
						pvt->_ctx,pvt->_pvtkey,
						SSL_FILETYPE_PEM)!=1 ||
				SSL_CTX_check_private_key(pvt->_ctx)!=1) {
				retval=false;
			}
		}

		// set ciphers
		if (retval && !charstring::isNullOrEmpty(pvt->_ciphers)) {
			if (SSL_CTX_set_cipher_list(
					pvt->_ctx,pvt->_ciphers)!=1) {
				retval=false;
			}
		}

		// set whether to verify peer or not
		if (retval && pvt->_verifypeer) {
			SSL_CTX_set_verify(pvt->_ctx,SSL_VERIFY_PEER,NULL);
		}

		// set the certificate authority file/path
		if (retval) {
			if (!charstring::isNullOrEmpty(pvt->_cafile)) {
				if (SSL_CTX_load_verify_locations(
					pvt->_ctx,pvt->_cafile,NULL)!=1) {
					retval=false;
				}
			} else if (!charstring::isNullOrEmpty(pvt->_capath)) {
				if (SSL_CTX_load_verify_locations(
					pvt->_ctx,NULL,pvt->_capath)!=1) {
					retval=false;
				}
			}
		}

		// set the verification depth
		if (retval && pvt->_depth) {
			SSL_CTX_set_verify_depth(pvt->_ctx,pvt->_depth);
		}

		// use diffie-hellman key exchange
		if (retval && !charstring::isNullOrEmpty(pvt->_kecert)) {
			SSL_CTX_set_options(pvt->_ctx,SSL_OP_SINGLE_DH_USE);
			BIO	*bio=BIO_new_file(pvt->_kecert,"r");
			#if (OPENSSL_VERSION_NUMBER < 0x00904000)
				DH	*dh=
				PEM_read_bio_DHparams(bio,NULL,NULL);
			#else
				DH	*dh=
				PEM_read_bio_DHparams(bio,NULL,NULL,NULL);
			#endif
			BIO_free(bio);
			if (!dh || SSL_CTX_set_tmp_dh(pvt->_ctx,dh)!=1) {
				retval=false;
			}
		}

		if (retval) {
			// these must be created here rather than above,
			// apparently modifying the context after creating an
			// SSL prevents the SSL from working properly...

			// create bio and set the file descriptor, if possible
			pvt->_bio=BIO_new(BIO_s_fd());
			if (pvt->_fd) {
				BIO_set_fd(pvt->_bio,
						pvt->_fd->getFileDescriptor(),
						BIO_NOCLOSE);
			}

			// create ssl and attach bio
			pvt->_ssl=SSL_new(pvt->_ctx);
			SSL_set_bio(pvt->_ssl,pvt->_bio,pvt->_bio);

			pvt->_dirty=false;
		} else {
			SSL_CTX_free(pvt->_ctx);
			pvt->_ctx=NULL;
		}

		// return success or failure
		return retval;
	#else
		return false;
	#endif
}

bool tlscontext::accept() {
	#ifdef RUDIMENTS_HAS_SSL
		if (!init()) {
			return false;
		}
		int	ret=SSL_accept(pvt->_ssl);
		setError(ret);
		return (ret==1);
	#else
		return false;
	#endif
}

bool tlscontext::peerCertificateIsValid() {
	#ifdef RUDIMENTS_HAS_SSL
		clearError();
		return (pvt->_ssl &&
			SSL_get_verify_result(pvt->_ssl)==X509_V_OK);
	#else
		return false;
	#endif
}

tlscertificate *tlscontext::getPeerCertificate() {
	#ifdef RUDIMENTS_HAS_SSL
		clearError();
		if (!pvt->_ssl) {
			return NULL;
		}
		X509	*cert=SSL_get_peer_certificate(pvt->_ssl);
		if (!cert) {
			return NULL;
		}
		tlscertificate	*tlscert=new tlscertificate();
		tlscert->setCertificate((void *)cert);
		return tlscert;
	#else
		return NULL;
	#endif
}

ssize_t tlscontext::read(void *buf, ssize_t count) {
	#ifdef RUDIMENTS_HAS_SSL
		clearError();
		if (!pvt->_ssl) {
			return false;
		}
		int	ret=SSL_read(pvt->_ssl,buf,count);
		setError(ret);
		return ret;
	#else
		return 0;
	#endif
}

ssize_t tlscontext::write(const void *buf, ssize_t count) {
	#ifdef RUDIMENTS_HAS_SSL
		clearError();
		if (!pvt->_ssl) {
			return -1;
		}
		int	ret=SSL_write(pvt->_ssl,buf,count);
		setError(ret);
		return ret;
	#else
		return 0;
	#endif
}

ssize_t tlscontext::pending() {
	#ifdef RUDIMENTS_HAS_SSL
		clearError();
		if (!pvt->_ssl) {
			return -1;
		}
		int	ret=SSL_pending(pvt->_ssl);
		setError(ret);
		return (ret==1);
	#else
		return 0;
	#endif
}

bool tlscontext::close() {
	#ifdef RUDIMENTS_HAS_SSL
		int	ret=1;
		if (pvt->_ssl) {
			clearError();
			ret=SSL_shutdown(pvt->_ssl);
			if (!ret) {
				clearError();
				ret=SSL_shutdown(pvt->_ssl);
			}
			setError(ret);
		}
		return (ret==1);
	#else
		return false;
	#endif
}

void tlscontext::clearError() {
	pvt->_error=0;
	pvt->_errorstr.clear();
	while (ERR_get_error()) {}
}

void tlscontext::setError(int32_t ret) {
	#ifdef RUDIMENTS_HAS_SSL
		if (!pvt->_ssl || ret==1) {
			return;
		}
		pvt->_errorstr.clear();
		bool	first=true;
		for (;;) {
			if (first) {
				first=false;
			} else {
				pvt->_errorstr.append('\n');
			}
			unsigned long	err=ERR_get_error();
			if (!err) {
				break;
			}
			pvt->_error=err;
			pvt->_errorstr.append(
				ERR_error_string(pvt->_error,NULL));
		}
	#else
	#endif
}

int32_t tlscontext::getError() {
	return pvt->_error;
}

const char *tlscontext::getErrorString() {
	#ifdef RUDIMENTS_HAS_SSL
		return pvt->_errorstr.getString();
	#else
		return NULL;
	#endif
}



class tlscertificateprivate {
	friend class tlscertificate;
	private:
		#ifdef RUDIMENTS_HAS_SSL
			X509	*_cert;
			char	_commonname[1024];
		#endif
};

tlscertificate::tlscertificate() {
	pvt=new tlscertificateprivate;
	#ifdef RUDIMENTS_HAS_SSL
		pvt->_cert=NULL;
	#endif
}

tlscertificate::~tlscertificate() {
	#ifdef RUDIMENTS_HAS_SSL
	#endif
}

const char *tlscertificate::getCommonName() {
	#ifdef RUDIMENTS_HAS_SSL
		return (!pvt->_cert ||
			// FIXME: X509_NAME_get_text_by_NID is deprecated
			X509_NAME_get_text_by_NID(
				X509_get_subject_name(pvt->_cert),
				NID_commonName,
				pvt->_commonname,
				sizeof(pvt->_commonname))==-1)?
				NULL:pvt->_commonname;
	#else
		return NULL;
	#endif
}

void tlscertificate::setCertificate(void *cert) {
	#ifdef RUDIMENTS_HAS_SSL
		pvt->_cert=(X509 *)cert;
	#else
	#endif
}
