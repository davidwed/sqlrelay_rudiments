// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#include <rudiments/tls.h>
#include <rudiments/charstring.h>
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
		filedescriptor		*_fd;
		int32_t			_error;
		#ifdef RUDIMENTS_HAS_SSL
			SSL_CTX		*_ctx;
			SSL		*_ssl;
			BIO		*_bio;
		#endif
};

tlscontext::tlscontext() {
	tls::initTLS();
	pvt=new tlscontextprivate;
	pvt->_fd=NULL;
	pvt->_error=0;
	init();
}

tlscontext::~tlscontext() {
	clear();
}

void tlscontext::init() {
	#ifdef RUDIMENTS_HAS_SSL
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
		pvt->_ssl=NULL;
		pvt->_bio=NULL;
		SSL_CTX_set_mode(pvt->_ctx,SSL_MODE_AUTO_RETRY);
	#endif
}

void tlscontext::clear() {
	#ifdef RUDIMENTS_HAS_SSL
		if (pvt->_ssl) {
			// this appears to free the BIO too
			SSL_free(pvt->_ssl);
		} else if (pvt->_bio) {
			BIO_free(pvt->_bio);
		}
		if (pvt->_ctx) {
			SSL_CTX_free(pvt->_ctx);
		}
	#endif
}

bool tlscontext::setCertificateChainFile(const char *filename) {
	#ifdef RUDIMENTS_HAS_SSL
		clearError();
		if (charstring::isNullOrEmpty(filename)) {
			return false;
		}
		return (SSL_CTX_use_certificate_chain_file(
						pvt->_ctx,filename)==1);
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

bool tlscontext::setPrivateKeyFile(const char *filename, const char *password) {
	#ifdef RUDIMENTS_HAS_SSL
		clearError();
		if (charstring::isNullOrEmpty(filename)) {
			return false;
		}
		if (password) {
			SSL_CTX_set_default_passwd_cb(
					pvt->_ctx,passwdCallback);
			SSL_CTX_set_default_passwd_cb_userdata(
					pvt->_ctx,(void *)password);
		}
		return (SSL_CTX_use_PrivateKey_file(
				pvt->_ctx,filename,SSL_FILETYPE_PEM)==1 &&
			SSL_CTX_check_private_key(pvt->_ctx)==1);
	#else
		return false;
	#endif
}

bool tlscontext::setCiphers(const char *ciphers) {
	#ifdef RUDIMENTS_HAS_SSL
		clearError();
		if (charstring::isNullOrEmpty(ciphers)) {
			return false;
		}
		return (SSL_CTX_set_cipher_list(pvt->_ctx,ciphers)==1);
	#else
		return false;
	#endif
}

void tlscontext::verifyPeer() {
	#ifdef RUDIMENTS_HAS_SSL
		SSL_CTX_set_verify(pvt->_ctx,SSL_VERIFY_PEER,NULL);
	#else
	#endif
}

void tlscontext::dontVerifyPeer() {
	#ifdef RUDIMENTS_HAS_SSL
		SSL_CTX_set_verify(pvt->_ctx,SSL_VERIFY_NONE,NULL);
	#else
	#endif
}

bool tlscontext::setCertificateAuthorityFile(const char *cafile) {
	return setCertificateAuthority(cafile,NULL);
}

bool tlscontext::setCertificateAuthorityPath(const char *capath) {
	return setCertificateAuthority(NULL,capath);
}

bool tlscontext::setCertificateAuthority(const char *cafile,
						const char *capath) {
	#ifdef RUDIMENTS_HAS_SSL
		clearError();
		if (charstring::isNullOrEmpty(cafile) &&
			charstring::isNullOrEmpty(capath)) {
			return false;
		}
		return (SSL_CTX_load_verify_locations(
					pvt->_ctx,cafile,capath)==1);
	#else
		return false;
	#endif
}

bool tlscontext::setVerificationDepth(uint32_t depth) {
	#ifdef RUDIMENTS_HAS_SSL
		if (!depth) {
			return false;
		}
		SSL_CTX_set_verify_depth(pvt->_ctx,depth);
		return true;
	#else
	#endif
}

bool tlscontext::useDiffieHellmanKeyExchange(const char *dhcert) {
	#ifdef RUDIMENTS_HAS_SSL
		clearError();
		if (charstring::isNullOrEmpty(dhcert)) {
			return false;
		}
		SSL_CTX_set_options(pvt->_ctx,SSL_OP_SINGLE_DH_USE);
		BIO	*bio=BIO_new_file(dhcert,"r");
		#if (OPENSSL_VERSION_NUMBER < 0x00904000)
			DH	*dh=PEM_read_bio_DHparams(bio,NULL,NULL);
		#else
			DH	*dh=PEM_read_bio_DHparams(bio,NULL,NULL,NULL);
		#endif
		BIO_free(bio);
		if (!dh) {
			return false;
		}
		return (SSL_CTX_set_tmp_dh(pvt->_ctx,dh)==1);
	#else
		return false;
	#endif
}

tlscertificate *tlscontext::getPeerCertificate() {
	#ifdef RUDIMENTS_HAS_SSL
		clearError();
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

bool tlscontext::peerCertificateIsValid() {
	#ifdef RUDIMENTS_HAS_SSL
		clearError();
		return (pvt->_ssl &&
			SSL_get_verify_result(pvt->_ssl)==X509_V_OK);
	#else
		return false;
	#endif
}
		
void tlscontext::setFileDescriptor(filedescriptor *fd) {
	#ifdef RUDIMENTS_HAS_SSL
		pvt->_fd=fd;
		if (pvt->_ssl) {
			// this appears to free the BIO too
			SSL_free(pvt->_ssl);
		} else if (pvt->_bio) {
			BIO_free(pvt->_bio);
		}
		if (pvt->_fd) {
			pvt->_bio=BIO_new_fd(pvt->_fd->getFileDescriptor(),
								BIO_NOCLOSE);
			pvt->_ssl=SSL_new(pvt->_ctx);
			SSL_set_bio(pvt->_ssl,pvt->_bio,pvt->_bio);
		} else {
			pvt->_bio=NULL;
			pvt->_ssl=NULL;
		}
	#else
	#endif
}

filedescriptor *tlscontext::getFileDescriptor() {
	return pvt->_fd;
}

bool tlscontext::connect() {
	#ifdef RUDIMENTS_HAS_SSL
		clearError();
		if (!pvt->_ssl) {
			return false;
		}
		int	ret=SSL_connect(pvt->_ssl);
		getError(ret);
		return (ret==1);
	#else
		return false;
	#endif
}

bool tlscontext::accept() {
	#ifdef RUDIMENTS_HAS_SSL
		clearError();
		if (!pvt->_ssl) {
			return false;
		}
		int	ret=SSL_accept(pvt->_ssl);
		getError(ret);
		return (ret==1);
	#else
		return false;
	#endif
}

ssize_t tlscontext::read(void *buf, ssize_t count) {
	#ifdef RUDIMENTS_HAS_SSL
		clearError();
		if (!pvt->_ssl) {
			return false;
		}
		int	ret=SSL_read(pvt->_ssl,buf,count);
		getError(ret);
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
		getError(ret);
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
		getError(ret);
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
			getError(ret);
		}
		clear();
		init();
		return (ret==1);
	#else
		return false;
	#endif
}

void tlscontext::clearError() {
	pvt->_error=0;
}

void tlscontext::getError(int32_t ret) {
	#ifdef RUDIMENTS_HAS_SSL
		if (!pvt->_ssl) {
			return;
		}
		pvt->_error=SSL_get_error(pvt->_ssl,ret);
	#else
	#endif
}

int32_t tlscontext::getError() {
	return pvt->_error;
}

const char *tlscontext::getErrorString() {
	#ifdef RUDIMENTS_HAS_SSL
		// FIXME: this needs to output the same as
		// ERR_print_errors_fp(stdout)
		return ERR_error_string(pvt->_error,NULL);
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
