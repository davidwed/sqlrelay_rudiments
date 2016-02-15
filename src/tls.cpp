// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#include <rudiments/tls.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/stdio.h>
#include <rudiments/error.h>

#if defined(RUDIMENTS_HAS_SSL)
	#include <openssl/ssl.h>
	#include <openssl/err.h>
#elif defined(RUDIMENTS_HAS_SSPI)
	#include <rudiments/gss.h>
	#include <rudiments/bytestring.h>
	#include <schannel.h>
#endif

threadmutex	tls::_tlsmutex;
bool		tls::_initialized=false;

bool tls::supportsTLS() {
	#if defined(RUDIMENTS_HAS_SSL)
		return true;
	#elif defined(RUDIMENTS_HAS_SSPI)
		return true;
	#else
		return false;
	#endif
}

void tls::initTLS() {
	#if defined(RUDIMENTS_HAS_SSL)
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
		bool			_validatepeer;
		const char		*_cafile;
		const char		*_capath;
		uint32_t		_depth;
		const char		*_kecert;
		int32_t			_error;
		stringbuffer		_errorstr;
		bool			_dirty;
		#if defined(RUDIMENTS_HAS_SSL)
			SSL_CTX		*_ctx;
			SSL		*_ssl;
			BIO		*_bio;
		#elif defined(RUDIMENTS_HAS_SSPI)
			gssmechanism	_gmech;
			gsscredentials	_gcred;
			gsscontext	_gctx;
			HCERTSTORE	_mycstore;
			PCCERT_CONTEXT	*_cctx;
			SCHANNEL_CRED	_scred;
			DWORD		_algidcount;
			ALG_ID		*_algids;
		#endif
		filedescriptor		*_fd;
};

tlscontext::tlscontext() : securitycontext() {
	pvt=new tlscontextprivate;
	pvt->_cert=NULL;
	pvt->_pvtkey=NULL;
	pvt->_pvtkeypwd=NULL;
	pvt->_ciphers=NULL;
	pvt->_validatepeer=NULL;
	pvt->_cafile=NULL;
	pvt->_capath=NULL;
	pvt->_depth=0;
	pvt->_kecert=NULL;
	pvt->_error=0;
	pvt->_errorstr.clear();
	pvt->_dirty=true;
	#if defined(RUDIMENTS_HAS_SSL)
		pvt->_bio=NULL;
		pvt->_ssl=NULL;
		pvt->_ctx=NULL;
	#elif defined(RUDIMENTS_HAS_SSPI)

		pvt->_mycstore=NULL;
		pvt->_cctx=NULL;
		bytestring::zero(&pvt->_scred,sizeof(pvt->_scred));
		pvt->_algidcount=0;
		pvt->_algids=NULL;

		pvt->_gmech.initialize(UNISP_NAME_A);
		pvt->_gcred.addDesiredMechanism(&pvt->_gmech);
		pvt->_gcred.setPackageSpecificData(&pvt->_scred);
		pvt->_gctx.setDesiredMechanism(&pvt->_gmech);
		pvt->_gctx.setDesiredFlags(ISC_REQ_SEQUENCE_DETECT|
						ISC_REQ_REPLAY_DETECT|
						ISC_REQ_CONFIDENTIALITY|
						ISC_RET_EXTENDED_ERROR|
						ISC_REQ_ALLOCATE_MEMORY|
						ISC_REQ_STREAM);
		pvt->_gctx.setCredentials(&pvt->_gcred);
	#endif
	pvt->_fd=NULL;
}

tlscontext::~tlscontext() {
	#if defined(RUDIMENTS_HAS_SSL)
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
	#elif defined(RUDIMENTS_HAS_SSPI)
		if (pvt->_mycstore) {
			CertCloseStore(pvt->_mycstore,0);
		}
		if (pvt->_cctx) {
			CertFreeCertificateContext(pvt->_cctx[0]);
			delete[] pvt->_cctx;
		}
		delete[] pvt->_algids;
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

void tlscontext::setValidationDepth(uint32_t depth) {
	pvt->_dirty=true;
	pvt->_depth=depth;
}

uint32_t tlscontext::getValidationDepth() {
	return pvt->_depth;
}

void tlscontext::setValidatePeer(bool validate) {
	pvt->_dirty=true;
	pvt->_validatepeer=validate;
}

bool tlscontext::getValidatePeer() {
	return pvt->_validatepeer;
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
	#if defined(RUDIMENTS_HAS_SSL)
		if (pvt->_bio) {
			BIO_set_fd(pvt->_bio,
				(pvt->_fd)?pvt->_fd->getFileDescriptor():-1,
				BIO_NOCLOSE);
		}
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_gctx.setFileDescriptor(pvt->_fd);
	#endif
}

filedescriptor *tlscontext::getFileDescriptor() {
	return pvt->_fd;
}

bool tlscontext::connect() {
	if (!init()) {
		return false;
	}
	#if defined(RUDIMENTS_HAS_SSL)
		int	ret=SSL_connect(pvt->_ssl);
		setError(ret);
		return (ret==1);
	#elif defined(RUDIMENTS_HAS_SSPI)
		return pvt->_gctx.connect();
	#else
		return false;
	#endif
}

#if defined(RUDIMENTS_HAS_SSL)
static int passwdCallback(char *buf, int size, int rwflag, void *userdata) {
	charstring::copy(buf,(char *)userdata,size);
	buf[size-1]=(char)NULL;
	return charstring::length(buf);
}
#endif 

#if defined(RUDIMENTS_HAS_SSPI)
static unsigned int *getCipherAlgs(const char *ciphers,
					bool adddh, DWORD *agcount) {

	char		**c;
	uint64_t	ccount;
	charstring::split(ciphers,",",true,&c,&ccount);

	unsigned int	*algids=new unsigned int[ccount+((adddh)?1:0)];
	for (uint64_t i=0; i<ccount; i++) {

		if (!charstring::compareIgnoringCase(c[i],"3DES")) {
			algids[i]=CALG_3DES;
		} else if (!charstring::compareIgnoringCase(
						c[i],"3DES_112")) {
			algids[i]=CALG_3DES_112;
		} else if (!charstring::compareIgnoringCase(
						c[i],"AES")) {
			algids[i]=CALG_AES;
		} else if (!charstring::compareIgnoringCase(
						c[i],"AES_128")) {
			algids[i]=CALG_AES_128;
		} else if (!charstring::compareIgnoringCase(
						c[i],"AES_192")) {
			algids[i]=CALG_AES_192;
		} else if (!charstring::compareIgnoringCase(
						c[i],"AES_256")) {
			algids[i]=CALG_AES_256;
		} else if (!charstring::compareIgnoringCase(
						c[i],"AGREEDKEY_ANY")) {
			algids[i]=CALG_AGREEDKEY_ANY;
		} else if (!charstring::compareIgnoringCase(
						c[i],"CYLINK_MEK")) {
			algids[i]=CALG_CYLINK_MEK;
		} else if (!charstring::compareIgnoringCase(
						c[i],"DES")) {
			algids[i]=CALG_DES;
		} else if (!charstring::compareIgnoringCase(
						c[i],"DESX")) {
			algids[i]=CALG_DESX;
		} else if (!charstring::compareIgnoringCase(
						c[i],"DH_EPHEM")) {
			algids[i]=CALG_DH_EPHEM;
		} else if (!charstring::compareIgnoringCase(
						c[i],"DH_SF")) {
			algids[i]=CALG_DH_SF;
		} else if (!charstring::compareIgnoringCase(
						c[i],"DSS_SIGN")) {
			algids[i]=CALG_DSS_SIGN;
		#ifdef CALG_ECDH
		} else if (!charstring::compareIgnoringCase(
						c[i],"ECDH")) {
			algids[i]=CALG_ECDH;
		#endif
		#ifdef CALG_ECDH_EPHEM
		} else if (!charstring::compareIgnoringCase(
						c[i],"ECDH_EPHEM")) {
			algids[i]=CALG_ECDH_EPHEM;
		#endif
		#ifdef CALG_ECDSA
		} else if (!charstring::compareIgnoringCase(
						c[i],"ECDSA")) {
			algids[i]=CALG_ECDSA;
		#endif
		} else if (!charstring::compareIgnoringCase(
						c[i],"ECMQV")) {
			algids[i]=CALG_ECMQV;
		} else if (!charstring::compareIgnoringCase(
						c[i],"HASH_REPLACE_OWF")) {
			algids[i]=CALG_HASH_REPLACE_OWF;
		} else if (!charstring::compareIgnoringCase(
						c[i],"HUGHES_MD5")) {
			algids[i]=CALG_HUGHES_MD5;
		} else if (!charstring::compareIgnoringCase(
						c[i],"HMAC")) {
			algids[i]=CALG_HMAC;
		} else if (!charstring::compareIgnoringCase(
						c[i],"KEA_KEYX")) {
			algids[i]=CALG_KEA_KEYX;
		} else if (!charstring::compareIgnoringCase(
						c[i],"MAC")) {
			algids[i]=CALG_MAC;
		} else if (!charstring::compareIgnoringCase(
						c[i],"MD2")) {
			algids[i]=CALG_MD2;
		} else if (!charstring::compareIgnoringCase(
						c[i],"MD4")) {
			algids[i]=CALG_MD4;
		} else if (!charstring::compareIgnoringCase(
						c[i],"MD5")) {
			algids[i]=CALG_MD5;
		} else if (!charstring::compareIgnoringCase(
						c[i],"NO_SIGN")) {
			algids[i]=CALG_NO_SIGN;
		} else if (!charstring::compareIgnoringCase(
						c[i],"OID_INFO_CNG_ONLY")) {
			algids[i]=CALG_OID_INFO_CNG_ONLY;
		} else if (!charstring::compareIgnoringCase(
						c[i],"OID_INFO_PARAMETERS")) {
			algids[i]=CALG_OID_INFO_PARAMETERS;
		} else if (!charstring::compareIgnoringCase(
						c[i],"PCT1_MASTER")) {
			algids[i]=CALG_PCT1_MASTER;
		} else if (!charstring::compareIgnoringCase(
						c[i],"RC2")) {
			algids[i]=CALG_RC2;
		} else if (!charstring::compareIgnoringCase(
						c[i],"RC4")) {
			algids[i]=CALG_RC4;
		} else if (!charstring::compareIgnoringCase(
						c[i],"RC5")) {
			algids[i]=CALG_RC5;
		} else if (!charstring::compareIgnoringCase(
						c[i],"RSA_KEYX")) {
			algids[i]=CALG_RSA_KEYX;
		} else if (!charstring::compareIgnoringCase(
						c[i],"RSA_SIGN")) {
			algids[i]=CALG_RSA_SIGN;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SCHANNEL_ENC_KEY")) {
			algids[i]=CALG_SCHANNEL_ENC_KEY;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SCHANNEL_MAC_KEY")) {
			algids[i]=CALG_SCHANNEL_MAC_KEY;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SCHANNEL_MASTER_HASH")) {
			algids[i]=CALG_SCHANNEL_MASTER_HASH;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SEAL")) {
			algids[i]=CALG_SEAL;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SHA")) {
			algids[i]=CALG_SHA;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SHA1")) {
			algids[i]=CALG_SHA1;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SHA_256")) {
			algids[i]=CALG_SHA_256;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SHA_384")) {
			algids[i]=CALG_SHA_384;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SHA_512")) {
			algids[i]=CALG_SHA_512;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SKIPJACK")) {
			algids[i]=CALG_SKIPJACK;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SSL2_MASTER")) {
			algids[i]=CALG_SSL2_MASTER;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SSL3_MASTER")) {
			algids[i]=CALG_SSL3_MASTER;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SSL3_SHAMD5")) {
			algids[i]=CALG_SSL3_SHAMD5;
		} else if (!charstring::compareIgnoringCase(
						c[i],"TEK")) {
			algids[i]=CALG_TEK;
		} else if (!charstring::compareIgnoringCase(
						c[i],"TLS1_MASTER")) {
			algids[i]=CALG_TLS1_MASTER;
		} else if (!charstring::compareIgnoringCase(
						c[i],"TLS1PRF")) {
			algids[i]=CALG_TLS1PRF;
		}
		delete[] c[i];
	}
	delete[] c;

	if (adddh) {
		algids[ccount]=CALG_DH_EPHEM;
	}

	*agcount=ccount;
	return algids;
}
#endif

bool tlscontext::init() {
	#if defined(RUDIMENTS_HAS_SSL)

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
		// FIXME: I think these leak...
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

		// set the validation depth
		if (retval && pvt->_depth) {
			SSL_CTX_set_verify_depth(pvt->_ctx,pvt->_depth);
		}

		// set whether to validate peer or not
		if (retval && pvt->_validatepeer) {
			SSL_CTX_set_verify(pvt->_ctx,SSL_VERIFY_PEER,NULL);
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
			// FIXME: I think these leak...
			pvt->_bio=BIO_new(BIO_s_fd());
			if (pvt->_fd) {
				BIO_set_fd(pvt->_bio,
						pvt->_fd->getFileDescriptor(),
						BIO_NOCLOSE);
			}

			// create ssl and attach bio
			// FIXME: I think these leak...
			pvt->_ssl=SSL_new(pvt->_ctx);
			SSL_set_bio(pvt->_ssl,pvt->_bio,pvt->_bio);

			pvt->_dirty=false;
		} else {
			SSL_CTX_free(pvt->_ctx);
			pvt->_ctx=NULL;
		}

		// return success or failure
		return retval;

	#elif defined(RUDIMENTS_HAS_SSPI)

		if (!pvt->_dirty) {
			return true;
		}

		tls::initTLS();

		close();

		bool	retval=true;
		DWORD	flags=0;

		// open the cert store, if necessary
		if (!pvt->_mycstore) {
			pvt->_mycstore=CertOpenStore(
					CERT_STORE_PROV_FILENAME_A,
					X509_ASN_ENCODING,
					NULL,
					0,
					pvt->_cert);
			if (!pvt->_mycstore) {
				retval=false;
			}
		}

		// set certificate chain file
		if (retval && !charstring::isNullOrEmpty(pvt->_cert)) {
			pvt->_cctx=new PCCERT_CONTEXT[1];
			pvt->_cctx[0]=CertFindCertificateInStore(
							pvt->_mycstore,
							X509_ASN_ENCODING,
							0,
							CERT_FIND_ANY,
							0,
							NULL);
			if (!pvt->_cctx[0]) {
				retval=false;
			}
			flags|=SCH_CRED_NO_DEFAULT_CREDS;
		} else {
			flags|=SCH_CRED_USE_DEFAULT_CREDS;
		}
							
		// auto-retry mode is enabled by default

		// set private key (and password, if provided)
		if (retval && !charstring::isNullOrEmpty(pvt->_pvtkey)) {
			if (pvt->_pvtkeypwd) {
				// FIXME: how?
			}
			// FIXME: how?
		}

		// set the certificate authority file/path
		if (retval) {
			if (!charstring::isNullOrEmpty(pvt->_cafile)) {
				// FIXME: how?
			} else if (!charstring::isNullOrEmpty(pvt->_capath)) {
				// FIXME: how?
			}
		}

		// set the validation depth
		if (retval && pvt->_depth) {
			// FIXME: how?
		}

		// set whether to validate peer or not
		if (retval && pvt->_validatepeer) {
			flags|=SCH_CRED_AUTO_CRED_VALIDATION;
		} else {
			flags|=SCH_CRED_MANUAL_CRED_VALIDATION;
		}

		// use diffie-hellman key exchange
		bool	usedh=false;
		if (retval && !charstring::isNullOrEmpty(pvt->_kecert)) {
			// FIXME: cert...
			usedh=true;
		}

		// set ciphers
		if (retval && (usedh ||
				!charstring::isNullOrEmpty(pvt->_ciphers))) {
			pvt->_algids=getCipherAlgs(pvt->_ciphers,usedh,
							&pvt->_algidcount);
		}


		// build schannel creds and acquire credentials
		if (retval) {
			bytestring::zero(&pvt->_scred,sizeof(pvt->_scred));
			pvt->_scred.dwVersion=SCHANNEL_CRED_VERSION;
			// FIXME: fix this...
			/*pvt->_scred.cCreds=1;
			pvt->_scred.paCred=pvt->_cctx;
			pvt->_scred.cSupportedAlgs=pvt->_algidcount;
			pvt->_scred.palgSupportedAlgs=pvt->_algids;
			pvt->_scred.dwFlags=flags;*/

			retval=pvt->_gcred.acquireForUser(NULL);

			pvt->_dirty=false;
		}

		// clean up on failure
		if (!retval) {
			if (pvt->_cctx) {
				CertFreeCertificateContext(pvt->_cctx[0]);
				delete[] pvt->_cctx;
				pvt->_cctx=NULL;
			}
			bytestring::zero(&pvt->_scred,sizeof(pvt->_scred));
		}

		// return success or failure
		return retval;
	#else
		return false;
	#endif
}

bool tlscontext::accept() {
	if (!init()) {
		return false;
	}
	#if defined(RUDIMENTS_HAS_SSL)
		int	ret=SSL_accept(pvt->_ssl);
		setError(ret);
		return (ret==1);
	#elif defined(RUDIMENTS_HAS_SSPI)
		return pvt->_gctx.accept();
	#else
		return false;
	#endif
}

bool tlscontext::peerCertificateIsValid() {
	#if defined(RUDIMENTS_HAS_SSL)
		clearError();
		return (pvt->_ssl &&
			SSL_get_verify_result(pvt->_ssl)==X509_V_OK);
	#elif defined(RUDIMENTS_HAS_SSPI)
		// FIXME: how?
		return false;
	#else
		return false;
	#endif
}

tlscertificate *tlscontext::getPeerCertificate() {
	#if defined(RUDIMENTS_HAS_SSL)
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
	#elif defined(RUDIMENTS_HAS_SSPI)
		// FIXME: how?
		return NULL;
	#else
		return NULL;
	#endif
}

ssize_t tlscontext::read(void *buf, ssize_t count) {
	#if defined(RUDIMENTS_HAS_SSL)
		clearError();
		if (!pvt->_ssl) {
			return false;
		}
		int	ret=SSL_read(pvt->_ssl,buf,count);
		setError(ret);
		return ret;
	#elif defined(RUDIMENTS_HAS_SSPI)
		clearError();
		ssize_t	ret=pvt->_gctx.read(buf,count);
		if (ret!=count) {
			setError(ret);
		}
		return ret;
	#else
		return 0;
	#endif
}

ssize_t tlscontext::write(const void *buf, ssize_t count) {
	clearError();
	#if defined(RUDIMENTS_HAS_SSL)
		if (!pvt->_ssl) {
			return -1;
		}
		int	ret=SSL_write(pvt->_ssl,buf,count);
		setError(ret);
		return ret;
	#elif defined(RUDIMENTS_HAS_SSPI)
		clearError();
		ssize_t	ret=pvt->_gctx.write(buf,count);
		if (ret!=count) {
			setError(ret);
		}
		return ret;
	#else
		return 0;
	#endif
}

ssize_t tlscontext::pending() {
	clearError();
	#if defined(RUDIMENTS_HAS_SSL)
		if (!pvt->_ssl) {
			return -1;
		}
		int	ret=SSL_pending(pvt->_ssl);
		setError(ret);
		return (ret==1);
	#elif defined(RUDIMENTS_HAS_SSPI)
		return pvt->_gctx.pending();
	#else
		return 0;
	#endif
}

bool tlscontext::close() {
	#if defined(RUDIMENTS_HAS_SSL)
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
	#elif defined(RUDIMENTS_HAS_SSPI)
		return true;
	#else
		return false;
	#endif
}

void tlscontext::clearError() {
	pvt->_error=0;
	pvt->_errorstr.clear();
	#if defined(RUDIMENTS_HAS_SSL)
		while (ERR_get_error()) {}
	#endif
}

void tlscontext::setError(int32_t ret) {
	#if defined(RUDIMENTS_HAS_SSL)
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
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_error=pvt->_gctx.getMajorStatus();
		pvt->_errorstr.clear();
		pvt->_errorstr.append(pvt->_gctx.getStatus());
	#else
	#endif
}

int32_t tlscontext::getError() {
	return pvt->_error;
}

const char *tlscontext::getErrorString() {
	return pvt->_errorstr.getString();
}



class tlscertificateprivate {
	friend class tlscertificate;
	private:
		#if defined(RUDIMENTS_HAS_SSL)
			X509	*_cert;
			char	_commonname[1024];
		#elif defined(RUDIMENTS_HAS_SSPI)
		#endif
};

tlscertificate::tlscertificate() {
	pvt=new tlscertificateprivate;
	#if defined(RUDIMENTS_HAS_SSL)
		pvt->_cert=NULL;
	#elif defined(RUDIMENTS_HAS_SSPI)
	#endif
}

tlscertificate::~tlscertificate() {
	#if defined(RUDIMENTS_HAS_SSL)
	#elif defined(RUDIMENTS_HAS_SSPI)
	#endif
}

const char *tlscertificate::getCommonName() {
	#if defined(RUDIMENTS_HAS_SSL)
		return (!pvt->_cert ||
			// FIXME: X509_NAME_get_text_by_NID is deprecated
			X509_NAME_get_text_by_NID(
				X509_get_subject_name(pvt->_cert),
				NID_commonName,
				pvt->_commonname,
				sizeof(pvt->_commonname))==-1)?
				NULL:pvt->_commonname;
	#elif defined(RUDIMENTS_HAS_SSPI)
		return NULL;
	#else
		return NULL;
	#endif
}

void tlscertificate::setCertificate(void *cert) {
	#if defined(RUDIMENTS_HAS_SSL)
		pvt->_cert=(X509 *)cert;
	#elif defined(RUDIMENTS_HAS_SSPI)
	#else
	#endif
}
