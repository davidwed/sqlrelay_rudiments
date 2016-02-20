// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#include <rudiments/tls.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/directory.h>
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>
#include <rudiments/error.h>

#if defined(RUDIMENTS_HAS_SSL)
	#include <openssl/ssl.h>
	#include <openssl/err.h>
#elif defined(RUDIMENTS_HAS_SSPI)
	#include <rudiments/gss.h>
	#include <rudiments/bytestring.h>
	#include <schannel.h>
	#define SSPI_ERROR(sstatus)	((sstatus)<0)
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
		bool			_isclient;
		const char		*_cert;
		const char		*_pkpwd;
		const char		*_ciphers;
		bool			_validatepeer;
		const char		*_ca;
		uint32_t		_depth;
		int32_t			_error;
		stringbuffer		_errorstr;
		bool			_dirty;
		filedescriptor		*_fd;
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
};

tlscontext::tlscontext() : securitycontext() {
	pvt=new tlscontextprivate;
	pvt->_isclient=false;
	pvt->_cert=NULL;
	pvt->_pkpwd=NULL;
	pvt->_ciphers=NULL;
	pvt->_validatepeer=false;
	pvt->_ca=NULL;
	pvt->_depth=0;
	pvt->_error=0;
	pvt->_errorstr.clear();
	pvt->_dirty=true;
	pvt->_fd=NULL;
	initContext();
}

tlscontext::~tlscontext() {
	freeContext();
}

void tlscontext::initContext() {
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

		pvt->_gcred.clearDesiredMechanisms();
		pvt->_gcred.addDesiredMechanism(&pvt->_gmech);
		pvt->_gcred.setPackageSpecificData(&pvt->_scred);

		pvt->_gctx.setDesiredMechanism(&pvt->_gmech);
		pvt->_gctx.setDesiredFlags(ISC_REQ_SEQUENCE_DETECT|
						ISC_REQ_REPLAY_DETECT|
						ISC_RET_EXTENDED_ERROR|
						ISC_REQ_STREAM);
		pvt->_gctx.setCredentials(&pvt->_gcred);
	#endif
}

void tlscontext::freeContext() {
	#if defined(RUDIMENTS_HAS_SSL)
		if (pvt->_ctx) {
			if (pvt->_ssl) {
				// SSL_free frees the BIO too
				SSL_free(pvt->_ssl);
			}
			SSL_CTX_free(pvt->_ctx);
		}
	#elif defined(RUDIMENTS_HAS_SSPI)
		delete[] pvt->_algids;
		if (pvt->_cctx) {
			for (DWORD i=0; i<pvt->_cctxcount; i++) {
				CertFreeCertificateContext(pvt->_cctx[i]);
			}
			delete[] pvt->_cctx;
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

void tlscontext::setPrivateKeyPassword(const char *password) {
	pvt->_dirty=true;
	pvt->_pkpwd=password;
}

const char *tlscontext::getPrivateKeyPassword() {
	return pvt->_pkpwd;
}

void tlscontext::setCiphers(const char *ciphers) {
	pvt->_dirty=true;
	pvt->_ciphers=ciphers;
}

const char *tlscontext::getCiphers() {
	return pvt->_ciphers;
}

void tlscontext::setCertificateAuthority(const char *ca) {
	pvt->_dirty=true;
	pvt->_ca=ca;
}

const char *tlscontext::getCertificateAuthority() {
	return pvt->_ca;
}

void tlscontext::setValidationDepth(uint32_t depth) {
	pvt->_dirty=true;
	pvt->_depth=depth;
}

uint32_t tlscontext::getValidationDepth() {
	return pvt->_depth;
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
	if (!reInit(true)) {
		return false;
	}
	#if defined(RUDIMENTS_HAS_SSL)
		int	ret=SSL_connect(pvt->_ssl);
		setError(ret);
		return (ret==1);
	#elif defined(RUDIMENTS_HAS_SSPI)
		bool	ret=pvt->_gctx.connect();
		setError(ret);
		if (ret && !pvt->_isclient && pvt->_validatepeer) {
			// FIXME: manually validate the peer certificate...
			// CertGetCertificateChain
			// CertVerifyCertificateChain
		}
		return ret;
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
static void getCipherAlgs(const char *ciphers,
				ALG_ID **algids, DWORD *algidcount) {

	#ifdef DEBUG_TLS
		stdoutput.printf("  ciphers {\n");
	#endif

	// split the cipher list on commas
	char		**c;
	uint64_t	ccount;
	charstring::split(ciphers,",",true,&c,&ccount);

	// create an array of ALG_ID's
	uint64_t	algindex=0;
	*algids=new ALG_ID[ccount+1];

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

	// use diffie-hellman key exchange
	#ifdef DEBUG_TLS
		stdoutput.printf("    cipher DH_EPHEM - success\n");
	#endif
	(*algids)[algindex++]=CALG_DH_EPHEM;

	// copy out the final algorithm count
	*algidcount=algindex;

	#ifdef DEBUG_TLS
		stdoutput.printf("    cipher count: %d\n",*algidcount);
		stdoutput.printf("  }\n");
	#endif
}
#endif

bool tlscontext::reInit(bool isclient) {

	if (!pvt->_dirty) {
		return true;
	}

	tls::initTLS();

	close();

	// re-init from previous runs
	freeContext();
	initContext();

	pvt->_isclient=isclient;

	bool	retval=true;

	#ifdef DEBUG_TLS
		stdoutput.printf("tls context {\n");
	#endif

	#if defined(RUDIMENTS_HAS_SSL)

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

		// load certificate chain file and private key,
		// using a password, if supplied
		#ifdef DEBUG_TLS
		bool	certloaded=false;
		#endif
		if (!charstring::isNullOrEmpty(pvt->_cert)) {
			if (pvt->_pkpwd) {
				SSL_CTX_set_default_passwd_cb(
					pvt->_ctx,passwdCallback);
				SSL_CTX_set_default_passwd_cb_userdata(
					pvt->_ctx,(void *)pvt->_pkpwd);
			}
			if (SSL_CTX_use_certificate_chain_file(
						pvt->_ctx,pvt->_cert)!=1 ||
				SSL_CTX_use_PrivateKey_file(
						pvt->_ctx,pvt->_cert,
						SSL_FILETYPE_PEM)!=1 ||
				SSL_CTX_check_private_key(pvt->_ctx)!=1) {
				retval=false;
			}
			#ifdef DEBUG_TLS
			certloaded=retval;
			#endif
		}

		// set ciphers
		if (retval && !charstring::isNullOrEmpty(pvt->_ciphers)) {
			if (SSL_CTX_set_cipher_list(
					pvt->_ctx,pvt->_ciphers)!=1) {
				retval=false;
			}
		}

		// use ephemeral diffie-hellman key exchange
		SSL_CTX_set_options(pvt->_ctx,SSL_OP_SINGLE_DH_USE);

		// set the certificate authority
		if (retval && !charstring::isNullOrEmpty(pvt->_ca)) {

			pvt->_validatepeer=true;

			// file or directory?
			directory	d;
			bool	ispath=d.open(pvt->_ca);
			d.close();

			if (SSL_CTX_load_verify_locations(
					pvt->_ctx,
					(ispath)?NULL:pvt->_ca,
					(ispath)?pvt->_ca:NULL)!=1) {
				retval=false;
			}
		}

		// set the validation depth
		if (retval && pvt->_depth) {
			SSL_CTX_set_verify_depth(pvt->_ctx,pvt->_depth);
		}

		// set whether to validate peer or not
		if (retval) {
			SSL_CTX_set_verify(pvt->_ctx,
				(pvt->_validatepeer)?
					(SSL_VERIFY_PEER|
					SSL_VERIFY_FAIL_IF_NO_PEER_CERT):
					SSL_VERIFY_NONE,
				NULL);
		}

		#ifdef DEBUG_TLS
			stdoutput.printf("  cert chain: %s - (%s) - (%s)\n",
					pvt->_cert,pvt->_pkpwd,
					(certloaded)?"valid":"invalid");
			stdoutput.printf("  ciphers: %s\n",pvt->_ciphers);
			stdoutput.printf("  ca: %s\n",pvt->_ca);
			stdoutput.printf("  depth: %d\n",pvt->_depth);
		#endif

		// build "bio" and "ssl"
		if (retval) {
			// these must be created here rather than above,
			// apparently modifying the context after creating an
			// SSL prevents the SSL from working properly...

			// create bio and set the file descriptor
			pvt->_bio=BIO_new(BIO_s_fd());
			BIO_set_fd(pvt->_bio,
				(pvt->_fd)?pvt->_fd->getFileDescriptor():-1,
				BIO_NOCLOSE);

			// create ssl and attach bio
			pvt->_ssl=SSL_new(pvt->_ctx);
			SSL_set_bio(pvt->_ssl,pvt->_bio,pvt->_bio);

			pvt->_dirty=false;
		}

	#elif defined(RUDIMENTS_HAS_SSPI)

		DWORD	flags=0;

		// set certificate chain file
		// FIXME: private key password)
		if (!charstring::isNullOrEmpty(pvt->_cert)) {

			// don't use default creds
			if (isclient) {
				flags|=SCH_CRED_NO_DEFAULT_CREDS;
			}

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

					// verify that the cert
					// contained a private key
					HCRYPTPROV_OR_NCRYPT_KEY_HANDLE	kh=NULL;
					DWORD	keytype;
					BOOL	mustfree=FALSE;
					CryptAcquireCertificatePrivateKey(
								cctx,
								0,
								NULL,
								&kh,
								&keytype,
								&mustfree);

					pvt->_cctx=new PCCERT_CONTEXT[1];
					pvt->_cctx[0]=cctx;
					pvt->_cctxcount=1;
				}
			}

			// If CertOpenStoreFailed, CertFindCertificateInStore
			// failed (but still returned a cert, which it can
			// apparently do), or CryptAcquireCertificatePrivateKey
			// failed to find a private key in, then an error
			// will be set.  Catch that error here to decide
			// whether this process succeeded or not.
			if (error::getNativeErrorNumber()) {
				setNativeError();
				retval=false;
			}

		} else if (isclient) {

			// no cert specified...

			// for clients, just fall back to default creds
			flags|=SCH_CRED_USE_DEFAULT_CREDS;

		} else {

			// no cert specified...

			// for servers, create a self-signed cert...

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
					pvt->_cctx=new PCCERT_CONTEXT[1];
					pvt->_cctx[0]=cctx;
					pvt->_cctxcount=1;
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
				setNativeError();
			}
		}

		// set the certificate authority
		if (retval && !charstring::isNullOrEmpty(pvt->_ca)) {

			pvt->_validatepeer=true;

			// file or directory?
			directory	d;
			bool	ispath=d.open(pvt->_ca);
			d.close();

			// FIXME: how?
		}

		// set the validation depth
		if (retval && pvt->_depth) {
			// FIXME: how?
		}

		if (retval && isclient) {
			// Set whether to validate the peer or not.
			// These flags are only valid on the client-side.
			// On the server side, if we want to validate the
			// peer, we'll have to do it manually.
			// FIXME: SCH_CRED_AUTO_CRED_VALIDATION currently
			// causes the connect to fail with SEC_E_WRONG_PRINCIPAL
			flags|=(pvt->_validatepeer)?
					SCH_CRED_AUTO_CRED_VALIDATION:
					SCH_CRED_MANUAL_CRED_VALIDATION;
		}

		#ifdef DEBUG_TLS
			stdoutput.printf("  cert chain: %s - (%s) - (%s)\n",
					pvt->_cert,pvt->_pkpwd,
					(pvt->_cctxcount)?"valid":"invalid");
			if (pvt->_cctx) {
				tlscertificate	cert;
				cert.setCertificate(pvt->_cctx[0]->pCertInfo);
			}
			stdoutput.printf("  use default creds: %s\n",
					(flags&SCH_CRED_USE_DEFAULT_CREDS)?
					"1":"0");
		#endif

		// set ciphers (automatically adds ephemeral diffie-hellman)
		if (retval) {
			getCipherAlgs(pvt->_ciphers,
					&pvt->_algids,&pvt->_algidcount);
		}

		#ifdef DEBUG_TLS
			stdoutput.printf("  ca: %s\n",pvt->_ca);
			stdoutput.printf("  depth: %d\n",pvt->_depth);
		#endif

		// build schannel creds and acquire credentials...
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
			if (isclient) {
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
	#else
		retval=false;
	#endif

	#ifdef DEBUG_TLS
		stdoutput.printf("}\n");
	#endif

	// re-init on failure
	if (!retval) {
		freeContext();
		initContext();
	}

	// return success or failure
	return retval;
}

bool tlscontext::accept() {
	if (!reInit(false)) {
		return false;
	}
	#if defined(RUDIMENTS_HAS_SSL)
		// FIXME: if accept fails with peer did not return a
		// certificate, then all subsequent accepts fail with the same
		// error, though it appears to recover from other errors
		int	ret=SSL_accept(pvt->_ssl);
		setError(ret);
		return (ret==1);
	#elif defined(RUDIMENTS_HAS_SSPI)
		bool	ret=pvt->_gctx.accept();
		setError(ret);
		return ret;
	#else
		return false;
	#endif
}

tlscertificate *tlscontext::getPeerCertificate() {
	clearError();
	#ifdef DEBUG_TLS
		stdoutput.printf("get peer certificate {\n");
	#endif
	tlscertificate	*tlscert=NULL;
	#if defined(RUDIMENTS_HAS_SSL)
		if (pvt->_ssl) {
			X509	*cert=SSL_get_peer_certificate(pvt->_ssl);
			if (cert) {
				tlscert=new tlscertificate();
				tlscert->setCertificate((void *)cert);
			}
		}
	#elif defined(RUDIMENTS_HAS_SSPI)
		PCERT_CONTEXT	cert=NULL;
		SECURITY_STATUS	sstatus=QueryContextAttributes(
					(_SecHandle *)pvt->_gctx.getContext(),
					SECPKG_ATTR_REMOTE_CERT_CONTEXT,
					&cert);
		if (SSPI_ERROR(sstatus)) {
			#ifdef DEBUG_TLS
				stdoutput.write("  failed "
					"(QueryContextAttributes("
					"SECPKG_ATTR_REMOTE_CERT_CONTEXT))\n");
			#endif
			// FIXME: there should be a cleaner way to do this
			pvt->_gctx.setMajorStatus(sstatus);
			setError(pvt->_gctx.getMajorStatus(),
					pvt->_gctx.getStatus());
		} else {
			tlscert=new tlscertificate();
			tlscert->setCertificate((void *)cert->pCertInfo);
		}
	#endif
	#ifdef DEBUG_TLS
		stdoutput.printf("}\n");
	#endif
	return tlscert;
}

ssize_t tlscontext::read(void *buf, ssize_t count) {
	clearError();
	#if defined(RUDIMENTS_HAS_SSL)
		if (!pvt->_ssl) {
			return RESULT_ERROR;
		}
		int	ret=SSL_read(pvt->_ssl,buf,count);
		setError(ret);
		return ret;
	#elif defined(RUDIMENTS_HAS_SSPI)
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
			return RESULT_ERROR;
		}
		int	ret=SSL_write(pvt->_ssl,buf,count);
		setError(ret);
		return ret;
	#elif defined(RUDIMENTS_HAS_SSPI)
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
			return RESULT_ERROR;
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
	clearError();
	#if defined(RUDIMENTS_HAS_SSL)
		int	ret=1;
		if (pvt->_ssl) {
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
		if (ret>0) {
			return;
		}
		setError(pvt->_gctx.getMajorStatus(),pvt->_gctx.getStatus());
	#else
	#endif
}

void tlscontext::setNativeError() {
	#if defined(RUDIMENTS_HAS_SSPI)
		pvt->_errorstr.clear();
		pvt->_error=error::getNativeErrorNumber();
		char	*errstr=error::getNativeErrorString();
		pvt->_errorstr.append(errstr);
		delete[] errstr;
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
		uint32_t	_version;
		uint64_t	_serialnumber;
		char		*_sigalg;
		char		*_issuer;
		datetime	_validfrom;
		datetime	_validto;
		char		*_subject;
		char		*_commonname;
		char		*_pkalg;
		unsigned char	*_pk;
		uint64_t	_pklen;
		uint64_t	_pkbits;
		// FIXME: issuer unique id
		// FIXME: subject unique id
		// FIXME: extensions
};

tlscertificate::tlscertificate() {
	pvt=new tlscertificateprivate;
	initCertificate();
}

tlscertificate::~tlscertificate() {
	freeCertificate();
}

void tlscertificate::initCertificate() {
	pvt->_version=0;
	pvt->_serialnumber=0;
	pvt->_sigalg=NULL;
	pvt->_issuer=NULL;
	pvt->_subject=NULL;
	pvt->_commonname=NULL;
	pvt->_pkalg=NULL;
	pvt->_pk=NULL;
	pvt->_pklen=0;
	pvt->_pkbits=0;
}

void tlscertificate::freeCertificate() {
	#if defined(RUDIMENTS_HAS_SSL)
		free(pvt->_issuer);
		free(pvt->_subject);
	#elif defined(RUDIMENTS_HAS_SSPI)
		delete[] pvt->_issuer;
		delete[] pvt->_subject;
	#endif
	delete[] pvt->_sigalg;
	delete[] pvt->_pkalg;
	delete[] pvt->_pk;
}

uint32_t tlscertificate::getVersion() {
	return pvt->_version;
}

uint64_t tlscertificate::getSerialNumber() {
	return pvt->_serialnumber;
}

const char *tlscertificate::getSignatureAlgorithm() {
	return pvt->_sigalg;
}

const char *tlscertificate::getIssuer() {
	return pvt->_issuer;
}

const char *tlscertificate::getSubject() {
	return pvt->_subject;
}

const char *tlscertificate::getCommonName() {
	return pvt->_commonname;
}

const char *tlscertificate::getPrivateKeyAlgorithm() {
	return pvt->_pkalg;
}

const unsigned char *tlscertificate::getPrivateKey() {
	return pvt->_pk;
}

uint64_t tlscertificate::getPrivateKeyByteLength() {
	return pvt->_pklen;
}

uint64_t tlscertificate::getPrivateKeyBitLength() {
	return pvt->_pkbits;
}

void tlscertificate::setCertificate(void *cert) {

	freeCertificate();
	initCertificate();

	#if defined(RUDIMENTS_HAS_SSL)

		X509	*c=(X509 *)cert;

		// FIXME: version
		// FIXME: serial number
		// FIXME: signature algorithm

		// get the issuer
		X509_NAME	*issuer=X509_get_issuer_name(c);
		pvt->_issuer=X509_NAME_oneline(issuer,NULL,0);

		// FIXME: valid-from
		// FIXME: valid-to

		// get the subject
		X509_NAME	*subject=X509_get_subject_name(c);
		pvt->_subject=X509_NAME_oneline(subject,NULL,0);

		// FIXME: public key info
		// FIXME: issuer unique id
		// FIXME: subject unique id
		// FIXME: extensions

	#elif defined(RUDIMENTS_HAS_SSPI)

		CERT_INFO	*c=(CERT_INFO *)cert;

		// get the version
		switch (c->dwVersion) {
			case CERT_V2:
				pvt->_version=2;
			case CERT_V3:
				pvt->_version=3;
			default:
				pvt->_version=1;
		}

		// get the serial number
		pvt->_serialnumber=0;
		for (DWORD i=0; i<c->SerialNumber.cbData && i<4; i++) {
			pvt->_serialnumber=(pvt->_serialnumber<<8)|
				(uint64_t)(c->SerialNumber.pbData[i]);
		}

		// signature algorithm
		pvt->_sigalg=charstring::duplicate(
				c->SignatureAlgorithm.pszObjId);
		// FIXME: signature algorithm parameters

		// get the issuer
		DWORD	size=CertNameToStr(X509_ASN_ENCODING,
					&c->Issuer,
					CERT_X500_NAME_STR,NULL,0);
		pvt->_issuer=new char[size];
		CertNameToStr(X509_ASN_ENCODING,
					&c->Issuer,
					CERT_X500_NAME_STR,
					pvt->_issuer,size);

		// get valid-from
		// (see datetime::getSystemDateAndTime)
		uint64_t	t=
		(((((uint64_t)c->NotBefore.dwHighDateTime)<<32)|
				c->NotBefore.dwLowDateTime)/10)-
				11644473600000000ULL;
		pvt->_validfrom.initialize(t/1000000,t%1000000);

		// get valid-to
		t=(((((uint64_t)c->NotAfter.dwHighDateTime)<<32)|
				c->NotAfter.dwLowDateTime)/10)-
				11644473600000000ULL;
		pvt->_validto.initialize(t/1000000,t%1000000);

		// get the subject
		size=CertNameToStr(X509_ASN_ENCODING,
				&c->Subject,
				CERT_X500_NAME_STR,NULL,0);
		pvt->_subject=new char[size];
		CertNameToStr(X509_ASN_ENCODING,
				&c->Subject,
				CERT_X500_NAME_STR,
				pvt->_subject,size);

		// public key algorithm
		pvt->_pkalg=charstring::duplicate(
			c->SubjectPublicKeyInfo.Algorithm.pszObjId);
		// FIXME: public key algorithm parameters

		// public key...
		pvt->_pklen=c->SubjectPublicKeyInfo.
						PublicKey.cbData;
		pvt->_pk=(unsigned char *)
			bytestring::duplicate(
			c->SubjectPublicKeyInfo.PublicKey.pbData,
			pvt->_pklen);
		pvt->_pkbits=(pvt->_pklen*8)-c->SubjectPublicKeyInfo.
							PublicKey.cUnusedBits;

		// FIXME: issuer unique id
		// FIXME: subject unique id
		// FIXME: extensions
	#else
	#endif

	// get the common name
	const char	*cn=charstring::findFirst(pvt->_subject,"CN=");
	if (cn) {
		pvt->_commonname=charstring::duplicate(cn+3);
		char	*end=charstring::findFirstOrEnd(pvt->_commonname,",");
		if (end) {
			*end='\0';
		}
	}

	#ifdef DEBUG_TLS
		stdoutput.printf("  certificate {\n");
		stdoutput.printf("    version: %d\n",pvt->_version);
		stdoutput.printf("    serial number: %lld\n",
						pvt->_serialnumber);
		stdoutput.printf("    signature algorithm: %s\n",pvt->_sigalg);
		stdoutput.printf("    issuer: %s\n",pvt->_issuer);
		stdoutput.printf("    valid-from: %s\n",
						pvt->_validfrom.getString());
		stdoutput.printf("    valid-to: %s\n",
						pvt->_validto.getString());
		stdoutput.printf("    subject: %s\n",pvt->_subject);
		stdoutput.printf("    public key algorithm: %s\n",pvt->_pkalg);
		stdoutput.printf("    public key: ");
		stdoutput.safePrint(pvt->_pk,(pvt->_pklen<5)?pvt->_pklen:5);
		stdoutput.printf("...\n");
		stdoutput.printf("    public key bits: %lld\n",pvt->_pkbits);
		stdoutput.printf("    common name: %s\n",pvt->_commonname);
		stdoutput.printf("  }\n");
	#endif
}
