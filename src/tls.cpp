// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#include <rudiments/tls.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/linkedlist.h>
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

#define DEBUG_TLS 1

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
			HCERTSTORE	_cstore;
			DWORD		_cctxcount;
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
	pvt->_validatepeer=false;
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

		pvt->_cstore=NULL;
		pvt->_cctxcount=0;
		pvt->_cctx=NULL;
		pvt->_algidcount=0;
		pvt->_algids=NULL;
		bytestring::zero(&pvt->_scred,sizeof(pvt->_scred));

		pvt->_gmech.initialize(UNISP_NAME_A);

		pvt->_gcred.addDesiredMechanism(&pvt->_gmech);
		pvt->_gcred.setPackageSpecificData(&pvt->_scred);

		pvt->_gctx.setDesiredMechanism(&pvt->_gmech);
		pvt->_gctx.setDesiredFlags(ISC_REQ_SEQUENCE_DETECT|
						ISC_REQ_REPLAY_DETECT|
						ISC_RET_EXTENDED_ERROR|
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
		// FIXME: move this into a generic clean-up method
		delete[] pvt->_algids;
		if (pvt->_cctx) {
			for (DWORD i=0; i<pvt->_cctxcount; i++) {
				CertFreeCertificateContext(pvt->_cctx[i]);
			}
			delete[] pvt->_cctx;
			pvt->_cctxcount=0;
		}
		if (pvt->_cstore) {
			CertCloseStore(pvt->_cstore,0);
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
	if (!init(true)) {
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
static void getCipherAlgs(const char *ciphers, bool adddh,
				ALG_ID **algids, DWORD *algidcount) {

	#ifdef DEBUG_TLS
		stdoutput.printf("  ciphers {\n");
	#endif

	// split the cipher list on commas
	char		**c;
	uint64_t	ccount;
	charstring::split(ciphers,",",true,&c,&ccount);

	// handle empty lists
	if (!ccount) {
		*algidcount=0;
		*algids=NULL;
		#ifdef DEBUG_TLS
			stdoutput.printf("    cipher count: %d\n",*algidcount);
			stdoutput.printf("  }\n");
		#endif
		return;
	}

	// create an array of ALG_ID's
	uint64_t	algindex=0;
	*algids=new ALG_ID[ccount+((adddh)?1:0)];

	// for each requested algorithm...
	for (uint64_t i=0; i<ccount; i++) {

		// clean up the algorithm name
		charstring::bothTrim(c[i]);
		charstring::upper(c[i]);
		charstring::replace(c[i],'-','_');

		#ifdef DEBUG_TLS
			uint64_t	oldalgindex=algindex;
			stdoutput.printf("    cipher %s - ",c[i]);
		#endif

		// add the corresponding algorithm to the array
		if (!charstring::compareIgnoringCase(c[i],"3DES")) {
			(*algids)[algindex++]=CALG_3DES;
		} else if (!charstring::compareIgnoringCase(
						c[i],"3DES_112")) {
			(*algids)[algindex++]=CALG_3DES_112;
		} else if (!charstring::compareIgnoringCase(
						c[i],"AES")) {
			(*algids)[algindex++]=CALG_AES;
		} else if (!charstring::compareIgnoringCase(
						c[i],"AES_128")) {
			(*algids)[algindex++]=CALG_AES_128;
		} else if (!charstring::compareIgnoringCase(
						c[i],"AES_192")) {
			(*algids)[algindex++]=CALG_AES_192;
		} else if (!charstring::compareIgnoringCase(
						c[i],"AES_256")) {
			(*algids)[algindex++]=CALG_AES_256;
		} else if (!charstring::compareIgnoringCase(
						c[i],"AGREEDKEY_ANY")) {
			(*algids)[algindex++]=CALG_AGREEDKEY_ANY;
		} else if (!charstring::compareIgnoringCase(
						c[i],"CYLINK_MEK")) {
			(*algids)[algindex++]=CALG_CYLINK_MEK;
		} else if (!charstring::compareIgnoringCase(
						c[i],"DES")) {
			(*algids)[algindex++]=CALG_DES;
		} else if (!charstring::compareIgnoringCase(
						c[i],"DESX")) {
			(*algids)[algindex++]=CALG_DESX;
		} else if (!charstring::compareIgnoringCase(
						c[i],"DH_EPHEM")) {
			(*algids)[algindex++]=CALG_DH_EPHEM;
		} else if (!charstring::compareIgnoringCase(
						c[i],"DH_SF")) {
			(*algids)[algindex++]=CALG_DH_SF;
		} else if (!charstring::compareIgnoringCase(
						c[i],"DSS_SIGN")) {
			(*algids)[algindex++]=CALG_DSS_SIGN;
		#ifdef CALG_ECDH
		} else if (!charstring::compareIgnoringCase(
						c[i],"ECDH")) {
			(*algids)[algindex++]=CALG_ECDH;
		#endif
		#ifdef CALG_ECDH_EPHEM
		} else if (!charstring::compareIgnoringCase(
						c[i],"ECDH_EPHEM")) {
			(*algids)[algindex++]=CALG_ECDH_EPHEM;
		#endif
		#ifdef CALG_ECDSA
		} else if (!charstring::compareIgnoringCase(
						c[i],"ECDSA")) {
			(*algids)[algindex++]=CALG_ECDSA;
		#endif
		} else if (!charstring::compareIgnoringCase(
						c[i],"ECMQV")) {
			(*algids)[algindex++]=CALG_ECMQV;
		} else if (!charstring::compareIgnoringCase(
						c[i],"HASH_REPLACE_OWF")) {
			(*algids)[algindex++]=CALG_HASH_REPLACE_OWF;
		} else if (!charstring::compareIgnoringCase(
						c[i],"HUGHES_MD5")) {
			(*algids)[algindex++]=CALG_HUGHES_MD5;
		} else if (!charstring::compareIgnoringCase(
						c[i],"HMAC")) {
			(*algids)[algindex++]=CALG_HMAC;
		} else if (!charstring::compareIgnoringCase(
						c[i],"KEA_KEYX")) {
			(*algids)[algindex++]=CALG_KEA_KEYX;
		} else if (!charstring::compareIgnoringCase(
						c[i],"MAC")) {
			(*algids)[algindex++]=CALG_MAC;
		} else if (!charstring::compareIgnoringCase(
						c[i],"MD2")) {
			(*algids)[algindex++]=CALG_MD2;
		} else if (!charstring::compareIgnoringCase(
						c[i],"MD4")) {
			(*algids)[algindex++]=CALG_MD4;
		} else if (!charstring::compareIgnoringCase(
						c[i],"MD5")) {
			(*algids)[algindex++]=CALG_MD5;
		} else if (!charstring::compareIgnoringCase(
						c[i],"NO_SIGN")) {
			(*algids)[algindex++]=CALG_NO_SIGN;
		} else if (!charstring::compareIgnoringCase(
						c[i],"OID_INFO_CNG_ONLY")) {
			(*algids)[algindex++]=CALG_OID_INFO_CNG_ONLY;
		} else if (!charstring::compareIgnoringCase(
						c[i],"OID_INFO_PARAMETERS")) {
			(*algids)[algindex++]=CALG_OID_INFO_PARAMETERS;
		} else if (!charstring::compareIgnoringCase(
						c[i],"PCT1_MASTER")) {
			(*algids)[algindex++]=CALG_PCT1_MASTER;
		} else if (!charstring::compareIgnoringCase(
						c[i],"RC2")) {
			(*algids)[algindex++]=CALG_RC2;
		} else if (!charstring::compareIgnoringCase(
						c[i],"RC4")) {
			(*algids)[algindex++]=CALG_RC4;
		} else if (!charstring::compareIgnoringCase(
						c[i],"RC5")) {
			(*algids)[algindex++]=CALG_RC5;
		} else if (!charstring::compareIgnoringCase(
						c[i],"RSA_KEYX")) {
			(*algids)[algindex++]=CALG_RSA_KEYX;
		} else if (!charstring::compareIgnoringCase(
						c[i],"RSA_SIGN")) {
			(*algids)[algindex++]=CALG_RSA_SIGN;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SCHANNEL_ENC_KEY")) {
			(*algids)[algindex++]=CALG_SCHANNEL_ENC_KEY;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SCHANNEL_MAC_KEY")) {
			(*algids)[algindex++]=CALG_SCHANNEL_MAC_KEY;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SCHANNEL_MASTER_HASH")) {
			(*algids)[algindex++]=CALG_SCHANNEL_MASTER_HASH;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SEAL")) {
			(*algids)[algindex++]=CALG_SEAL;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SHA")) {
			(*algids)[algindex++]=CALG_SHA;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SHA1")) {
			(*algids)[algindex++]=CALG_SHA1;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SHA_256")) {
			(*algids)[algindex++]=CALG_SHA_256;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SHA_384")) {
			(*algids)[algindex++]=CALG_SHA_384;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SHA_512")) {
			(*algids)[algindex++]=CALG_SHA_512;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SKIPJACK")) {
			(*algids)[algindex++]=CALG_SKIPJACK;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SSL2_MASTER")) {
			(*algids)[algindex++]=CALG_SSL2_MASTER;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SSL3_MASTER")) {
			(*algids)[algindex++]=CALG_SSL3_MASTER;
		} else if (!charstring::compareIgnoringCase(
						c[i],"SSL3_SHAMD5")) {
			(*algids)[algindex++]=CALG_SSL3_SHAMD5;
		} else if (!charstring::compareIgnoringCase(
						c[i],"TEK")) {
			(*algids)[algindex++]=CALG_TEK;
		} else if (!charstring::compareIgnoringCase(
						c[i],"TLS1_MASTER")) {
			(*algids)[algindex++]=CALG_TLS1_MASTER;
		} else if (!charstring::compareIgnoringCase(
						c[i],"TLS1PRF")) {
			(*algids)[algindex++]=CALG_TLS1PRF;
		}

		// clean up
		delete[] c[i];

		#ifdef DEBUG_TLS
			if (algindex==oldalgindex) {
				stdoutput.printf("failed\n");
			} else {
				stdoutput.printf("success\n");
			}
		#endif
	}

	// clean up
	delete[] c;

	// add Diffie-Hellman if requested
	if (adddh) {
		#ifdef DEBUG_TLS
			uint64_t	oldalgindex=algindex;
			stdoutput.printf("    cipher DH_EPHEM - ");
		#endif
		(*algids)[algindex++]=CALG_DH_EPHEM;

		#ifdef DEBUG_TLS
			if (algindex==oldalgindex) {
				stdoutput.printf("failed\n");
			} else {
				stdoutput.printf("success\n");
			}
		#endif
	}

	// copy out the final algorithm count
	*algidcount=algindex;

	#ifdef DEBUG_TLS
		stdoutput.printf("    cipher count: %d\n",*algidcount);
		stdoutput.printf("  }\n");
	#endif
}
#endif

bool tlscontext::init(bool client) {
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
				#if defined(RUDIMENTS_HAS_TLS_METHOD)
					TLS_method()
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

		// clean up from previous runs
		// FIXME: move this into a generic clean-up method
		if (pvt->_algids) {
			delete[] pvt->_algids;
			pvt->_algids=NULL;
			pvt->_algidcount=0;
		}
		if (pvt->_cctx) {
			CertFreeCertificateContext(pvt->_cctx[0]);
			delete[] pvt->_cctx;
			pvt->_cctx=NULL;
			pvt->_cctxcount=0;
		}
		if (pvt->_cstore) {
			CertCloseStore(pvt->_cstore,0);
			pvt->_cstore=NULL;
		}
		bytestring::zero(&pvt->_scred,sizeof(pvt->_scred));

		#ifdef DEBUG_TLS
			stdoutput.printf("schannel_cred {\n");
		#endif

		bool	retval=true;
		DWORD	flags=0;

		// set certificate chain file
		bool	loadedcert=false;
		if (!charstring::isNullOrEmpty(pvt->_cert)) {

			// open the specified certificate store
			pvt->_cstore=CertOpenStore(
					CERT_STORE_PROV_FILENAME_A,
					X509_ASN_ENCODING|
					PKCS_7_ASN_ENCODING,
					NULL,
					0,
					pvt->_cert);

			if (pvt->_cstore) {

				// get the first certificate
				PCCERT_CONTEXT	cctx=
						CertFindCertificateInStore(
							pvt->_cstore,
							X509_ASN_ENCODING|
							PKCS_7_ASN_ENCODING,
							0,
							CERT_FIND_ANY,
							0,
							NULL);
				if (cctx) {

					pvt->_cctx=new PCCERT_CONTEXT[1];
					pvt->_cctx[0]=cctx;
					pvt->_cctxcount=1;

					// it's actually possible for an error
					// to occur but a cert to still be
					// returned, so catch that too
					if (error::getNativeErrorNumber()) {
						retval=false;
					} else {
						loadedcert=true;
					}
				}
			} else {
				retval=false;
			}

			// if a legitimate error occurred, then make it
			// available to be returned by getStatus()
			if (!retval) {
				char	*errstr=error::getNativeErrorString();
				setError(error::getNativeErrorNumber(),errstr);
				delete[] errstr;
			}
		}

		if (retval && !loadedcert) {

			// Either no certificate store was specified or there
			// was no certificate in the store.
			/*if (client) {

				// For clients, just fall back to default creds.
				flags|=SCH_CRED_USE_DEFAULT_CREDS;

			} else {*/

				// For servers, create a self-signed cert...

				// acquire a crypto context
				HCRYPTPROV	cp=NULL;
				if (!CryptAcquireContext(&cp,NULL,
							MS_DEF_PROV,
							PROV_RSA_FULL,
							CRYPT_VERIFYCONTEXT
							#if _WIN32_WINNT>0x0400
							|CRYPT_SILENT
							#endif
							)) {
					retval=false;
				}

				// create an 2048-bit rsa key pair
				// FIXME: how is this used?
				HCRYPTKEY 	key=NULL;
				if (retval && !CryptGenKey(cp,
							AT_SIGNATURE,
							0x08000000,
							&key)) {
					retval=false;
				}

				// encode the subject
				const char	*x500="CN=Test, T=Test";
				BYTE		*buffer=NULL;
				DWORD		buffersize=0;
				if (retval && !CertStrToName(
						X509_ASN_ENCODING,
						x500,CERT_X500_NAME_STR,NULL,
						NULL,&buffersize,NULL)) {
					retval=false;
				}
				if (retval) {
					buffer=new BYTE[buffersize];
					if (!CertStrToName(
						X509_ASN_ENCODING,
						x500,CERT_X500_NAME_STR,NULL,
						buffer,&buffersize,NULL)) {
						retval=false;
					}
				}

				// create the cert
				if (retval) {
					CERT_NAME_BLOB	cnb;
					bytestring::zero(&cnb,sizeof(cnb));
					cnb.pbData=buffer;
					cnb.cbData=buffersize;

					CRYPT_KEY_PROV_INFO	ckpi;
					bytestring::zero(&ckpi,sizeof(ckpi));
					ckpi.pwszContainerName=NULL;
					ckpi.pwszProvName=NULL;
					ckpi.dwProvType=PROV_RSA_FULL;
					ckpi.dwFlags=CRYPT_MACHINE_KEYSET;
					ckpi.cProvParam=0;
					ckpi.rgProvParam=NULL;
					ckpi.dwKeySpec=AT_SIGNATURE;

					CRYPT_ALGORITHM_IDENTIFIER	cai;
					bytestring::zero(&cai,sizeof(cai));
					cai.pszObjId=szOID_RSA_SHA1RSA;

					SYSTEMTIME	expiration;
					GetSystemTime(&expiration);
					expiration.wYear+=10;

					PCCERT_CONTEXT	cctx=
						CertCreateSelfSignCertificate(
								NULL,
								&cnb,0,
								&ckpi,
								&cai,0,
								&expiration,0);
					if (cctx) {
						pvt->_cctx=
							new PCCERT_CONTEXT[1];
						pvt->_cctx[0]=cctx;
						pvt->_cctxcount=1;
						loadedcert=true;
					} else {
						retval=false;
					}
				}

				// clean up...
				if (buffer) {
					delete[] buffer;
				}
				if (key) {
					CryptDestroyKey(key);
				}
				if (cp) {
					CryptReleaseContext(cp,0);
				}

				// catch errors
				if (!retval) {
					char	*errstr=
						error::getNativeErrorString();
					setError(error::getNativeErrorNumber(),
									errstr);
					delete[] errstr;
				}
			//}
		}

		if (retval && loadedcert) {
			flags|=SCH_CRED_NO_DEFAULT_CREDS;
		}

if (pvt->_cctx[0]) {
	HCRYPTPROV_OR_NCRYPT_KEY_HANDLE	kh=NULL;
	DWORD				keyspec;
	BOOL				flag=FALSE;
	if (!CryptAcquireCertificatePrivateKey(pvt->_cctx[0],
						0,NULL,&kh,&keyspec,&flag)) {
		stdoutput.printf("failed to load private key from cert\n");
	} else {
		stdoutput.printf("loaded private key from cert successfully\n");
	}
}

		// FIXME: A client can use default creds, but a server must
		// have a valid certificate loaded.  Just setting
		// SCH_CRED_USE_DEFAULT_CREDS isn't sufficient.  Both sides
		// will eventually fail with SEC_E_ALGORITHM_MISMATCH.

		#ifdef DEBUG_TLS
			stdoutput.printf("  cert store: %s - (%s)\n",
					pvt->_cert,
					(pvt->_cstore)?"valid":"invalid");
			stdoutput.printf("  cert count: %d\n",pvt->_cctxcount);
			stdoutput.printf("  use default creds: %s\n",
					(flags&SCH_CRED_USE_DEFAULT_CREDS)?
					"yes":"no");
		#endif

		// set private key (and password, if provided)
		if (retval && !charstring::isNullOrEmpty(pvt->_pvtkey)) {
			if (pvt->_pvtkeypwd) {
				// FIXME: how?
			}
			// FIXME: CryptAcquireCertificatePrivateKey
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

		// set whether to auto-validate peer or not
		// (force this true for the server)
		if (retval && (!client || pvt->_validatepeer)) {
			flags|=SCH_CRED_AUTO_CRED_VALIDATION;
		} else {
			flags|=SCH_CRED_MANUAL_CRED_VALIDATION;
		}

		// use diffie-hellman key exchange
		// (disable this on the client side)
		bool	usedh=false;
		if (retval && !client &&
			!charstring::isNullOrEmpty(pvt->_kecert)) {
			// FIXME: cert...
			usedh=true;
		}

		// set ciphers
		if (retval && (usedh ||
				!charstring::isNullOrEmpty(pvt->_ciphers))) {
			getCipherAlgs(pvt->_ciphers,usedh,
					&pvt->_algids,&pvt->_algidcount);
		}

		#ifdef DEBUG_TLS
			stdoutput.printf("  auto-validation: %s\n",
					(flags&SCH_CRED_AUTO_CRED_VALIDATION)?
					"yes":"no");
			stdoutput.printf("  dh key exchange: %s\n",
					(usedh)?"yes":"no");
			stdoutput.printf("}\n");
		#endif


		// build schannel creds and acquire credentials
		if (retval) {

			// configure schannel creds
			bytestring::zero(&pvt->_scred,sizeof(pvt->_scred));
			pvt->_scred.dwVersion=SCHANNEL_CRED_VERSION;
			pvt->_scred.cCreds=pvt->_cctxcount;
			pvt->_scred.paCred=pvt->_cctx;
			pvt->_scred.cSupportedAlgs=pvt->_algidcount;
			pvt->_scred.palgSupportedAlgs=pvt->_algids;
			pvt->_scred.dwFlags=flags;
			pvt->_scred.grbitEnabledProtocols=SP_PROT_TLS1_2;

			// acquire credentials
			if (client) {
				retval=pvt->_gcred.acquireForUser(NULL);
			} else {
				retval=pvt->_gcred.acquireForService(NULL);
			}

			if (retval) {
				pvt->_dirty=false;
			} else {
				pvt->_error=pvt->_gcred.getMajorStatus();
				pvt->_errorstr.clear();
				pvt->_errorstr.append(pvt->_gcred.getStatus());
			}
		}

		// clean up on failure
		if (!retval) {
			// FIXME: move this into a generic clean-up method
			if (pvt->_algids) {
				delete[] pvt->_algids;
				pvt->_algids=NULL;
				pvt->_algidcount=0;
			}
			if (pvt->_cctx) {
				CertFreeCertificateContext(pvt->_cctx[0]);
				delete[] pvt->_cctx;
				pvt->_cctx=NULL;
				pvt->_cctxcount=0;
			}
			if (pvt->_cstore) {
				CertCloseStore(pvt->_cstore,0);
				pvt->_cstore=NULL;
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
	if (!init(false)) {
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
		setError(pvt->_gctx.getMajorStatus(),pvt->_gctx.getStatus());
	#else
	#endif
}

void tlscontext::setError(int32_t err, const char *errstr) {
	pvt->_error=err;
	pvt->_errorstr.clear();
	pvt->_errorstr.append(errstr);
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
