// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#include <rudiments/tls.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_HAS_SSL
	#include <openssl/ssl.h>
	#include <openssl/err.h>
#endif

#define DEBUG_TLS 1

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
		#ifdef RUDIMENTS_HAS_SSL
			SSL_CTX		*_ctx;
			SSL		*_ssl;
			BIO		*_bio;
		#endif
		filedescriptor		*_fd;
		int32_t			_error;
};

tlscontext::tlscontext(bool server) {
	tls::initTLS();
	#ifdef RUDIMENTS_HAS_SSL
		pvt=new tlscontextprivate;
		pvt->_ctx=SSL_CTX_new((server)?SSLv23_server_method():
						SSLv23_client_method());
		#ifdef DEBUG_TLS
			if (!pvt->_ctx) {
				stdoutput.printf("SSL_CTX_new() failed\n");
				ERR_print_errors_fp(stdout);
			}
		#endif
		#ifdef DEBUG_TLS
			if (!pvt->_ctx) {
				stdoutput.printf("SSL_new() failed\n");
				ERR_print_errors_fp(stdout);
			}
		#endif
		pvt->_bio=NULL;
		SSL_CTX_set_mode(pvt->_ctx,SSL_MODE_AUTO_RETRY);
	#endif
	pvt->_fd=NULL;
	pvt->_error=0;
}

tlscontext::~tlscontext() {
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

bool tlscontext::useCertificateChainFile(const char *filename) {
	#ifdef RUDIMENTS_HAS_SSL
		if (SSL_CTX_use_certificate_chain_file(pvt->_ctx,filename)!=1) {
			#ifdef DEBUG_TLS
				stdoutput.printf("SSL_CTX_use_"
						"certificate_chain_file"
						"(%s) failed\n",filename);
				ERR_print_errors_fp(stdout);
			#endif
			return false;
		}
		return true;
	#else
		return false;
	#endif
}

#ifdef RUDIMENTS_HAS_SSL
static int passwdCallback(char *buf, int size, int rwflag, void *userdata) {
	charstring::copy(buf,(char *)userdata,size);
	buf[size-1]=(char)NULL;
	#ifdef DEBUG_TLS
		stdoutput.printf("supplying password: \"%s\"\n",buf);
		ERR_print_errors_fp(stdout);
	#endif
	return charstring::length(buf);
}
#endif

bool tlscontext::usePrivateKeyFile(const char *filename, const char *password) {
	#ifdef RUDIMENTS_HAS_SSL
		if (password) {
			SSL_CTX_set_default_passwd_cb(
					pvt->_ctx,passwdCallback);
			SSL_CTX_set_default_passwd_cb_userdata(
					pvt->_ctx,(void *)password);
		}
		if (SSL_CTX_use_PrivateKey_file(
				pvt->_ctx,filename,SSL_FILETYPE_PEM)!=1) {
			#ifdef DEBUG_TLS
				stdoutput.printf("SSL_CTX_use_"
						"PrivateKey_file"
						"(%s) failed\n",filename);
				ERR_print_errors_fp(stdout);
			#endif
			return false;
		}
		if (SSL_CTX_check_private_key(pvt->_ctx)!=1) {
			#ifdef DEBUG_TLS
				stdoutput.printf("SSL_CTX_check_"
						"private_key() failed\n");
				ERR_print_errors_fp(stdout);
			#endif
			return false;
		}
		return true;
	#else
		return false;
	#endif
}

void tlscontext::setVerifyPeer() {
	#ifdef RUDIMENTS_HAS_SSL
		SSL_CTX_set_verify(pvt->_ctx,SSL_VERIFY_PEER,NULL);
	#else
	#endif
}

bool tlscontext::loadVerifyLocations(const char *cafile,
						const char *capath) {
	#ifdef RUDIMENTS_HAS_SSL
		if (SSL_CTX_load_verify_locations(pvt->_ctx,cafile,capath)!=1) {
			#ifdef DEBUG_TLS
				stdoutput.printf("SSL_CTX_load_"
						"verify_locations"
						"(%s,%s) failed\n",
						cafile,capath);
				ERR_print_errors_fp(stdout);
			#endif
			return false;
		}
		return true;
	#else
		return false;
	#endif
}

void tlscontext::setVerifyDepth(int32_t depth) {
	#ifdef RUDIMENTS_HAS_SSL
		SSL_CTX_set_verify_depth(pvt->_ctx,depth);
	#else
	#endif
}

bool tlscontext::useDiffieHellmanKeyExchange(const char *dhcert) {
	#ifdef RUDIMENTS_HAS_SSL
		if (charstring::isNullOrEmpty(dhcert)) {
			#ifdef DEBUG_TLS
				stdoutput.printf("null or empty dh cert\n");
			#endif
			return false;
		}
		SSL_CTX_set_options(pvt->_ctx,SSL_OP_SINGLE_DH_USE);
		BIO	*bio=BIO_new_file(dhcert,"r");
		#ifdef DEBUG_TLS
			if (!bio) {
				stdoutput.printf("BIO_new_file(%s) "
						"failed\n",dhcert);
				ERR_print_errors_fp(stdout);
			}
		#endif
		#if (OPENSSL_VERSION_NUMBER < 0x00904000)
			DH	*dh=PEM_read_bio_DHparams(bio,NULL,NULL);
		#else
			DH	*dh=PEM_read_bio_DHparams(bio,NULL,NULL,NULL);
		#endif
		BIO_free(bio);
		if (!dh) {
			#ifdef DEBUG_TLS
				stdoutput.printf("PEM_read_bio_DHparams "
						"failed\n");
				ERR_print_errors_fp(stdout);
			#endif
			return false;
		}
		if (SSL_CTX_set_tmp_dh(pvt->_ctx,dh)!=1) {
			#ifdef DEBUG_TLS
				stdoutput.printf("SSL_CTX_set_tmp_dh() "
						"failed\n");
				ERR_print_errors_fp(stdout);
			#endif
			return false;
		}
		return true;
	#else
		return false;
	#endif
}

tlscertificate *tlscontext::getPeerCertificate() {
	#ifdef RUDIMENTS_HAS_SSL
		X509	*cert=SSL_get_peer_certificate(pvt->_ssl);
		if (!cert) {
			#ifdef DEBUG_TLS
				stdoutput.printf("SSL_get_peer_certificate() "
						"failed\n");
				ERR_print_errors_fp(stdout);
			#endif
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
		if (!pvt->_ssl) {
			#ifdef DEBUG_TLS
				stdoutput.printf("peerCertificateIsValid() - "
						"pvt->_ssl is NULL\n");
			#endif
			return false;
		}
		if (SSL_get_verify_result(pvt->_ssl)!=X509_V_OK) {
			#ifdef DEBUG_TLS
				stdoutput.printf("SSL_get_verify_result() "
						"failed\n");
				ERR_print_errors_fp(stdout);
			#endif
			return false;
		}
		return true;
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
			#ifdef DEBUG_TLS
				if (!pvt->_bio) {
					stdoutput.printf("BIO_new_fd() "
								"failed\n");
					ERR_print_errors_fp(stdout);
				}
			#endif
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
		if (!pvt->_ssl) {
			#ifdef DEBUG_TLS
				stdoutput.printf("connect() -"
						"pvt->_ssl is NULL\n");
			#endif
		}
		clearError();
		int	ret=SSL_connect(pvt->_ssl);
		getError(ret);
		#ifdef DEBUG_TLS
			if (ret!=1) {
				stdoutput.printf("SSL_connect() "
						"failed with %d\n",ret);
				ERR_print_errors_fp(stdout);
			}
		#endif
		return (ret==1);
	#else
		return false;
	#endif
}

bool tlscontext::accept() {
	#ifdef RUDIMENTS_HAS_SSL
		if (!pvt->_ssl) {
			#ifdef DEBUG_TLS
				stdoutput.printf("accept() -"
						"pvt->_ssl is NULL\n");
			#endif
		}
		clearError();
		int	ret=SSL_accept(pvt->_ssl);
		getError(ret);
		#ifdef DEBUG_TLS
			if (ret!=1) {
				stdoutput.printf("SSL_accept() "
						"failed with %d\n",ret);
				ERR_print_errors_fp(stdout);
			}
		#endif
		return (ret==1);
	#else
		return false;
	#endif
}

ssize_t tlscontext::read(void *buf, ssize_t count) {
	#ifdef RUDIMENTS_HAS_SSL
		if (!pvt->_ssl) {
			#ifdef DEBUG_TLS
				stdoutput.printf("read() -"
						"pvt->_ssl is NULL\n");
			#endif
		}
		clearError();
		int	ret=SSL_read(pvt->_ssl,buf,count);
		getError(ret);
		#ifdef DEBUG_TLS
			if (ret<0) {
				stdoutput.printf("SSL_read() "
						"failed with %d\n",ret);
				ERR_print_errors_fp(stdout);
			}
		#endif
		return ret;
	#else
		return 0;
	#endif
}

ssize_t tlscontext::write(const void *buf, ssize_t count) {
	#ifdef RUDIMENTS_HAS_SSL
		if (!pvt->_ssl) {
			#ifdef DEBUG_TLS
				stdoutput.printf("write() -"
						"pvt->_ssl is NULL\n");
			#endif
		}
		clearError();
		int	ret=SSL_write(pvt->_ssl,buf,count);
		getError(ret);
		#ifdef DEBUG_TLS
			if (ret<0) {
				stdoutput.printf("SSL_write() "
						"failed with %d\n",ret);
				ERR_print_errors_fp(stdout);
			}
		#endif
		return ret;
	#else
		return 0;
	#endif
}

ssize_t tlscontext::pending() {
	#ifdef RUDIMENTS_HAS_SSL
		if (!pvt->_ssl) {
			#ifdef DEBUG_TLS
				stdoutput.printf("pending() -"
						"pvt->_ssl is NULL\n");
			#endif
		}
		clearError();
		int	ret=SSL_pending(pvt->_ssl);
		getError(ret);
		#ifdef DEBUG_TLS
			if (ret!=1) {
				stdoutput.printf("SSL_pending() "
						"failed with %d\n",ret);
				ERR_print_errors_fp(stdout);
			}
		#endif
		return (ret==1);
	#else
		return 0;
	#endif
}

bool tlscontext::close() {
	#ifdef RUDIMENTS_HAS_SSL
		if (!pvt->_ssl) {
			#ifdef DEBUG_TLS
				stdoutput.printf("close() -"
						"pvt->_ssl is NULL\n");
			#endif
		}
		clearError();
		int	ret=SSL_shutdown(pvt->_ssl);
		if (!ret) {
			clearError();
			ret=SSL_shutdown(pvt->_ssl);
		}
		getError(ret);
		#ifdef DEBUG_TLS
			if (ret!=1) {
				stdoutput.printf("SSL_shutdown() "
						"failed with %d\n",ret);
				ERR_print_errors_fp(stdout);
			}
		#endif
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
			#ifdef DEBUG_TLS
				stdoutput.printf("getError() -"
						"pvt->_ssl is NULL\n");
			#endif
		}
		pvt->_error=SSL_get_error(pvt->_ssl,ret);
	#else
	#endif
}

int32_t tlscontext::getError() {
	return pvt->_error;
}

const char *tlscontext::getErrorString() {
	return ERR_error_string(pvt->_error,NULL);
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
		if (!pvt->_cert) {
			#ifdef DEBUG_TLS
				stdoutput.printf("getCommonName() - "
						"no cert specified\n");
				ERR_print_errors_fp(stdout);
			#endif
			return NULL;
		}
		if (X509_NAME_get_text_by_NID(
				X509_get_subject_name(pvt->_cert),
				NID_commonName,
				pvt->_commonname,
				sizeof(pvt->_commonname))==-1) {
			#ifdef DEBUG_TLS
				stdoutput.printf("X509_NAME_get_text_by_NID() "
						"failed\n");
				ERR_print_errors_fp(stdout);
			#endif
			return NULL;
		}
		return pvt->_commonname;
	#else
		return NULL;
	#endif
}

void tlscertificate::setCertificate(void *cert) {
	#ifdef RUDIMENTS_HAS_SSL
		pvt->_cert=(X509 *)cert;
	#else
		return NULL;
	#endif
}
