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
	#include <openssl/x509v3.h>
#elif defined(RUDIMENTS_HAS_SSPI)
	#include <rudiments/gss.h>
	#include <rudiments/bytestring.h>
	#include <schannel.h>
	#define SSPI_ERROR(sstatus)	((sstatus)<0)

	// MSVC 2008- doesn't define HCRYPTPROV_OR_NCRYPT_KEY_HANDLE
	#if _MSC_VER <= 1500
		typedef	ULONG_PTR	HCRYPTPROV_OR_NCRYPT_KEY_HANDLE;
	#endif
#endif

#ifdef RUDIMENTS_HAVE_LIMITS_H
	#include <limits.h>
#endif

// if SSIZE_MAX is undefined, choose a good safe value
// that should even work on 16-bit systems
#ifndef SSIZE_MAX
	#define SSIZE_MAX 16383
#endif

#if defined(RUDIMENTS_HAS_SSPI)
// FIXME: move to charstring class
static WCHAR *asciiToUnicode(const CHAR *in) {

	int32_t	size=MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,in,-1,NULL,0);
	if (!size) {
		return NULL;
	}

	WCHAR	*out=new WCHAR[size];
	if (!MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,in,-1,out,size)) {
		delete[] out;
		out=NULL;
	}
	return out;
}
#endif

//#define DEBUG_TLS 1

threadmutex	tls::_tlsmutex;
bool		tls::_initialized=false;

bool tls::supported() {
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
		const char		*_version;
		const char		*_cert;
		const char		*_pk;
		const char		*_pkpwd;
		const char		*_ciphers;
		bool			_validatepeer;
		uint16_t		_depth;
		const char		*_ca;
		int32_t			_error;
		stringbuffer		_errorstr;
		bool			_dirty;
		filedescriptor		*_fd;
		#if defined(RUDIMENTS_HAS_SSL)
			SSL_CTX		*_ctx;
			SSL		*_ssl;
			BIO		*_bio;
			X509		*_peercert;
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
			HCERTSTORE	_castore;
			HCERTSTORE	_syscastore;
			HCERTSTORE	_sysrootstore;
			PCERT_CONTEXT	_peercert;
		#endif
};

tlscontext::tlscontext() : securitycontext() {
	pvt=new tlscontextprivate;
	pvt->_isclient=false;
	pvt->_version=NULL;
	pvt->_cert=NULL;
	pvt->_pk=NULL;
	pvt->_pkpwd=NULL;
	#ifdef RUDIMENTS_DEFAULT_CIPHER_PROFILE_SYSTEM
	pvt->_ciphers="PROFILE=SYSTEM";
	#else
	pvt->_ciphers=NULL;
	#endif
	pvt->_validatepeer=false;
	pvt->_depth=9;
	pvt->_ca=NULL;
	pvt->_error=0;
	pvt->_errorstr.clear();
	pvt->_dirty=true;
	pvt->_fd=NULL;
	initContext();
	#if defined(RUDIMENTS_HAS_SSPI)
		pvt->_syscastore=NULL;
		pvt->_sysrootstore=NULL;
	#endif
}

tlscontext::~tlscontext() {
	freeContext();
	#if defined(RUDIMENTS_HAS_SSL)
		if (pvt->_peercert) {
			X509_free(pvt->_peercert);
		}
	#elif defined(RUDIMENTS_HAS_SSPI)
		if (pvt->_syscastore) {
			CertCloseStore(pvt->_syscastore,0);
		}
		if (pvt->_sysrootstore) {
			CertCloseStore(pvt->_sysrootstore,0);
		}
	#endif
	delete pvt;
}

#if defined(RUDIMENTS_HAS_SSPI)
static PCCERT_CONTEXT defaultcctx[1]={
	NULL
};
#endif

void tlscontext::initContext() {
	#if defined(RUDIMENTS_HAS_SSL)
		pvt->_bio=NULL;
		pvt->_ssl=NULL;
		pvt->_ctx=NULL;
		pvt->_peercert=NULL;
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_cstore=NULL;
		pvt->_cctxcount=0;
		pvt->_cctx=defaultcctx;
		pvt->_algidcount=0;
		pvt->_algids=NULL;
		pvt->_castore=NULL;
		pvt->_peercert=NULL;

		bytestring::zero(&pvt->_scred,sizeof(pvt->_scred));

		pvt->_gmech.initialize(UNISP_NAME_A);

		pvt->_gcred.clearDesiredMechanisms();
		pvt->_gcred.addDesiredMechanism(&pvt->_gmech);
		pvt->_gcred.setPackageSpecificData(&pvt->_scred);

		pvt->_gctx.setDesiredMechanism(&pvt->_gmech);
		pvt->_gctx.setCredentials(&pvt->_gcred);
		pvt->_gctx.setTokenFormat(GSSTOKENFORMAT_TLS);
	#endif
}

void tlscontext::initSubContext() {
	#if defined(RUDIMENTS_HAS_SSL)
		if (pvt->_ctx) {
			// create bio and set the file descriptor
			pvt->_bio=BIO_new(BIO_s_fd());
			BIO_set_fd(pvt->_bio,
				(pvt->_fd)?pvt->_fd->getFileDescriptor():-1,
				BIO_NOCLOSE);

			// create ssl and attach bio
			pvt->_ssl=SSL_new(pvt->_ctx);
			SSL_set_bio(pvt->_ssl,pvt->_bio,pvt->_bio);
		}
	#endif
}

void tlscontext::freeContext() {
	#if defined(RUDIMENTS_HAS_SSL)
		if (pvt->_ctx) {
			freeSubContext();
			SSL_CTX_free(pvt->_ctx);
		}
	#elif defined(RUDIMENTS_HAS_SSPI)
		if (pvt->_castore) {
			CertCloseStore(pvt->_castore,0);
		}
		delete[] pvt->_algids;
		if (pvt->_cctx!=defaultcctx) {
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

void tlscontext::freeSubContext() {
	#if defined(RUDIMENTS_HAS_SSL)
		if (pvt->_ssl) {
			// SSL_free frees the BIO too
			SSL_free(pvt->_ssl);
		}
	#endif
}

void tlscontext::setProtocolVersion(const char *version) {
	pvt->_dirty=true;
	pvt->_version=version;
}

const char *tlscontext::getProtocolVersion() {
	return pvt->_version;
}

void tlscontext::setCertificateChainFile(const char *filename) {
	pvt->_dirty=true;
	pvt->_cert=filename;
}

const char *tlscontext::getCertificateChainFile() {
	return pvt->_cert;
}

void tlscontext::setPrivateKeyFile(const char *filename) {
	pvt->_dirty=true;
	pvt->_pk=filename;
}

const char *tlscontext::getPrivateKeyFile() {
	return pvt->_pk;
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
	#ifdef RUDIMENTS_DEFAULT_CIPHER_PROFILE_SYSTEM
	pvt->_ciphers=(charstring::isNullOrEmpty(ciphers))?
					"PROFILE=SYSTEM":ciphers;
	#else
	pvt->_ciphers=ciphers;
	#endif
}

const char *tlscontext::getCiphers() {
	return pvt->_ciphers;
}

void tlscontext::setValidatePeer(bool validatepeer) {
	pvt->_dirty=true;
	pvt->_validatepeer=validatepeer;
}

bool tlscontext::getValidatePeer() {
	return pvt->_validatepeer;
}

void tlscontext::setValidationDepth(uint16_t depth) {
	pvt->_dirty=true;
	pvt->_depth=(depth>0 && depth<=9)?depth:9;
}

uint16_t tlscontext::getValidationDepth() {
	return pvt->_depth;
}

void tlscontext::setCertificateAuthority(const char *ca) {
	pvt->_dirty=true;
	pvt->_ca=ca;
}

const char *tlscontext::getCertificateAuthority() {
	return pvt->_ca;
}

void tlscontext::setFileDescriptor(filedescriptor *fd) {
	pvt->_dirty=true;
	pvt->_fd=fd;
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
		if (ret<1) {
			// reinit the ssl and bio
			freeSubContext();
			initSubContext();
		} else if (pvt->_validatepeer && !isPeerCertValid()) {
			ret=0;
		}
		return (ret==1);
	#elif defined(RUDIMENTS_HAS_SSPI)
		bool	ret=pvt->_gctx.connect();
		setError(ret);
		if (ret && pvt->_validatepeer) {
			ret=isPeerCertValid();
		}
		return ret;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool tlscontext::isPeerCertValid() {

	#ifdef DEBUG_TLS
		stdoutput.write("is peer cert valid {\n");
	#endif

	// make sure the peer cert has been loaded
	if (!loadPeerCert()) {
		#ifdef DEBUG_TLS
			stdoutput.write(" failed (loadPeerCert)\n}\n");
		#endif
		return false;
	}

	#if defined(RUDIMENTS_HAS_SSL)

		if (SSL_get_verify_result(pvt->_ssl)!=X509_V_OK) {
			#ifdef DEBUG_TLS
				stdoutput.write(" failed "
					"(SSL_get_verify_result)\n}\n");
			#endif
			return false;
		}

	#elif defined(RUDIMENTS_HAS_SSPI)

		// build list of stores to iterate through:
		// * provided store
		// * system ca store
		// * system root store
		HCERTSTORE	stores[4];
		uint16_t	i=0;
		if (pvt->_castore) {
			stores[i++]=pvt->_castore;
		}
		if (pvt->_syscastore) {
			stores[i++]=pvt->_syscastore;
		}
		if (pvt->_sysrootstore) {
			stores[i++]=pvt->_syscastore;
		}
		stores[i++]=NULL;

		// Run through each cert in the chain.  If we reach the end
		// of the chain and the last cert was self-signed, then
		// the first cert (the peer cert) is valid...

		// start with the peer cert
		PCCERT_CONTEXT	cert=pvt->_peercert;
		uint16_t	depth=0;
		for (;;) {

			#ifdef DEBUG_TLS
				stdoutput.printf("  depth: %d...\n",depth);
				tlscertificate	c;
				c.setCertificate((void *)cert->pCertInfo);
			#endif

			PCCERT_CONTEXT	issuer=NULL;
			bool		selfsignedfound=false;

			// for each cert, look for an issuing cert in the
			// provided ca store, system ca store and system root
			// store...
			for (HCERTSTORE *store=stores; *store; store++) {

				#ifdef DEBUG_TLS
					stdoutput.printf("  store loop...\n");
				#endif

				// get the issuing cert of the current cert...
				PCCERT_CONTEXT	prevcert=NULL;
				do {

					#ifdef DEBUG_TLS
						stdoutput.printf(
							"    cert loop...\n");
					#endif

					// set some flags...
					DWORD	flags=
						CERT_STORE_SIGNATURE_FLAG|
						CERT_STORE_TIME_VALIDITY_FLAG|
						CERT_STORE_REVOCATION_FLAG;

					// get the issuer
					issuer=
					CertGetIssuerCertificateFromStore(
								*store,
								cert,
								prevcert,
								&flags);
				
					#ifdef DEBUG_TLS
					stdoutput.printf("      issuing cert ");
					if (!issuer) {
						stdoutput.printf("not ");
					}
					stdoutput.printf("found\n");
					#endif

					if (issuer) {
						// if the issuing cert has
						// expired or has been revoked
						// then spin back and try
						// again...
						if (flags&
						CERT_STORE_TIME_VALIDITY_FLAG) {
							#ifdef DEBUG_TLS
							stdoutput.printf(
								"      "
								"expired\n");
							#endif
							prevcert=issuer;
						} else if (!(flags&
						CERT_STORE_NO_CRL_FLAG)) {
							#ifdef DEBUG_TLS
							stdoutput.printf(
								"      "
								"revoked\n");
							#endif
							prevcert=issuer;
						}
					}
				} while (prevcert);

				// if we found a valid issuing cert then we
				// don't need to keep looking for valid issuing
				// certs in other stores
				if (issuer) {
					break;
				}

				// if the current cert has no issuing cert
				// because it is self-signed then we also don't
				// need to keep looking for valid issuing certs
				// in other stores
				if (GetLastError()==CRYPT_E_SELF_SIGNED) {
					selfsignedfound=true;
					#ifdef DEBUG_TLS
					stdoutput.printf("      self signed\n");
					#endif
					break;
				}
			}

			// if we found a self-signed cert then we're done,
			// success
			if (selfsignedfound) {
				break;
			}
			
			// if no valid issuing cert was found then we're also
			// done, failure
			if (!issuer) {
				#ifdef DEBUG_TLS
					stdoutput.write("  failed "
					"(no valid issuer found)\n}\n");
				#endif
				setNativeError();
				return false;
			}

			// we found a valid issuing cert and need to keep
			// stepping through the chain...

			// increment and check depth
			depth++;
			if (depth>pvt->_depth) {
				// we have exceeded the allowed
				// validation depth
				#ifdef DEBUG_TLS
					stdoutput.write(
						"  failed (depth)\n}\n");
				#endif
				return false;
			}

			// free the previous cert
			// (unless it was the original peer cert)
			if (cert!=pvt->_peercert) {
				CertFreeCertificateContext(cert);
			}

			// move on
			cert=issuer;
		}
	#else
		#ifdef DEBUG_TLS
			stdoutput.write(" failed (not supported)\n}\n");
		#endif
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif

	#ifdef DEBUG_TLS
		stdoutput.write("  success\n}\n");
	#endif

	return true;
}

#if defined(RUDIMENTS_HAS_SSL)
static int passwdCallback(char *buf, int size, int rwflag, void *userdata) {
	charstring::copy(buf,(char *)userdata,size);
	buf[size-1]=(char)NULL;
	return charstring::length(buf);
}
#endif 

#if defined(RUDIMENTS_HAS_SSPI)
static void getCipherAlgs(const char *ciphers, bool isclient,
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
		#ifdef CALG_AES
		} else if (!charstring::compareIgnoringCase(
						c[i],"AES")) {
			(*algids)[algindex++]=CALG_AES;
		#endif
		#ifdef CALG_AES_128
		} else if (!charstring::compareIgnoringCase(
						c[i],"AES_128")) {
			(*algids)[algindex++]=CALG_AES_128;
		#endif
		#ifdef CALG_AES_192
		} else if (!charstring::compareIgnoringCase(
						c[i],"AES_192")) {
			(*algids)[algindex++]=CALG_AES_192;
		#endif
		#ifdef CALG_AES_256
		} else if (!charstring::compareIgnoringCase(
						c[i],"AES_256")) {
			(*algids)[algindex++]=CALG_AES_256;
		#endif
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
		#ifdef CALG_ECMQV
		} else if (!charstring::compareIgnoringCase(
						c[i],"ECMQV")) {
			(*algids)[algindex++]=CALG_ECMQV;
		#endif
		#ifdef CALG_HASH_REPLACE_OWF
		} else if (!charstring::compareIgnoringCase(
						c[i],"HASH_REPLACE_OWF")) {
			(*algids)[algindex++]=CALG_HASH_REPLACE_OWF;
		#endif
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
		#ifdef CALG_NO_SIGN
		} else if (!charstring::compareIgnoringCase(
						c[i],"NO_SIGN")) {
			(*algids)[algindex++]=CALG_NO_SIGN;
		#endif
		#ifdef CALG_OID_INFO_CNG_ONLY
		} else if (!charstring::compareIgnoringCase(
						c[i],"OID_INFO_CNG_ONLY")) {
			(*algids)[algindex++]=CALG_OID_INFO_CNG_ONLY;
		#endif
		#ifdef CALG_OID_INFO_PARAMETERS
		} else if (!charstring::compareIgnoringCase(
						c[i],"OID_INFO_PARAMETERS")) {
			(*algids)[algindex++]=CALG_OID_INFO_PARAMETERS;
		#endif
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
		#ifdef CALG_SHA_256
		} else if (!charstring::compareIgnoringCase(
						c[i],"SHA_256")) {
			(*algids)[algindex++]=CALG_SHA_256;
		#endif
		#ifdef CALG_SHA_384
		} else if (!charstring::compareIgnoringCase(
						c[i],"SHA_384")) {
			(*algids)[algindex++]=CALG_SHA_384;
		#endif
		#ifdef CALG_SHA_512
		} else if (!charstring::compareIgnoringCase(
						c[i],"SHA_512")) {
			(*algids)[algindex++]=CALG_SHA_512;
		#endif
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

	// add diffie-hellman key exchange on server
	if (!isclient) {
		#ifdef DEBUG_TLS
			stdoutput.printf("    cipher DH_EPHEM - success\n");
		#endif
		(*algids)[algindex++]=CALG_DH_EPHEM;
	}

	// copy out the final algorithm count
	*algidcount=algindex;

	#ifdef DEBUG_TLS
		stdoutput.printf("    cipher count: %d\n",*algidcount);
		stdoutput.printf("  }\n");
	#endif
}
#endif

#ifdef RUDIMENTS_HAS_SSPI
static DWORD getLocation(const char *provider) {
	if (!charstring::compare(provider,
				"LOCAL_MACHINE")) {
    		return CERT_SYSTEM_STORE_LOCAL_MACHINE;
	} else if (!charstring::compare(provider,
				"CURRENT_SERVICE")) {
    		return CERT_SYSTEM_STORE_CURRENT_SERVICE;
	} else if (!charstring::compare(provider,
				"SERVICES")) {
    		return CERT_SYSTEM_STORE_SERVICES;
	} else if (!charstring::compare(provider,
				"USERS")) {
    		return CERT_SYSTEM_STORE_USERS;
	} else if (!charstring::compare(provider,
				"CURRENT_USER_GROUP_POLICY")) {
    		return CERT_SYSTEM_STORE_CURRENT_USER_GROUP_POLICY;
	} else if (!charstring::compare(provider,
				"LOCAL_MACHINE_GROUP_POLICY")) {
    		return CERT_SYSTEM_STORE_LOCAL_MACHINE_GROUP_POLICY;
	} else if (!charstring::compare(provider,
				"LOCAL_MACHINE_ENTERPRISE")) {
    		return CERT_SYSTEM_STORE_LOCAL_MACHINE_ENTERPRISE;
	}
    	return CERT_SYSTEM_STORE_CURRENT_USER;
}
#endif

bool tlscontext::reInit(bool isclient) {

	// free/clear any old peer certificate
	#if defined(RUDIMENTS_HAS_SSL)
		if (pvt->_peercert) {
			X509_free(pvt->_peercert);
		}
		pvt->_peercert=NULL;
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_peercert=NULL;
	#endif

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

		// decide on the protocol version
		const SSL_METHOD	*method=NULL;
		if (!charstring::compareIgnoringCase(
					pvt->_version,"SSL2")) {
			#if defined(RUDIMENTS_HAS_SSLV2_METHOD)
			if (isclient) {
				method=SSLv2_client_method();
			} else {
				method=SSLv2_server_method();
			}
			#endif
		} else if (!charstring::compareIgnoringCase(
					pvt->_version,"SSL3")) {
			#if defined(RUDIMENTS_HAS_SSLV3_METHOD)
			if (isclient) {
				method=SSLv3_client_method();
			} else {
				method=SSLv3_server_method();
			}
			#endif
		} else if (!charstring::compareIgnoringCase(
					pvt->_version,"TLS1")) {
			#if defined(RUDIMENTS_HAS_TLSV1_METHOD)
			if (isclient) {
				method=TLSv1_client_method();
			} else {
				method=TLSv1_server_method();
			}
			#endif
		} else if (!charstring::compareIgnoringCase(
					pvt->_version,"TLS1.1")) {
			#if defined(RUDIMENTS_HAS_TLSV1_1_METHOD)
			if (isclient) {
				method=TLSv1_1_client_method();
			} else {
				method=TLSv1_1_server_method();
			}
			#endif
		} else if (!charstring::compareIgnoringCase(
					pvt->_version,"TLS1.2")) {
			#if defined(RUDIMENTS_HAS_TLSV1_2_METHOD)
			if (isclient) {
				method=TLSv1_2_client_method();
			} else {
				method=TLSv1_2_server_method();
			}
			#endif
		}
		if (!method) {
			if (isclient) {
				#if defined(RUDIMENTS_HAS_TLS_METHOD)
					method=TLS_client_method()
				#elif defined(RUDIMENTS_HAS_SSLV23_METHOD)
					method=SSLv23_client_method()
				#elif defined(RUDIMENTS_HAS_SSLV2_METHOD)
					method=SSLv2_client_method()
				#endif
				;
			} else {
				#if defined(RUDIMENTS_HAS_TLS_METHOD)
					method=TLS_server_method()
				#elif defined(RUDIMENTS_HAS_SSLV23_METHOD)
					method=SSLv23_server_method()
				#elif defined(RUDIMENTS_HAS_SSLV2_METHOD)
					method=SSLv2_server_method()
				#endif
				;
			}
		}

		// create the context
		// (some versions take a non-const SSL_METHOD *)
		pvt->_ctx=SSL_CTX_new((SSL_METHOD *)method);

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
						pvt->_ctx,
						(pvt->_pk)?pvt->_pk:pvt->_cert,
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
					pvt->_ctx,
					pvt->_ciphers)!=1) {
				retval=false;
			}
		}

		// use ephemeral diffie-hellman key exchange
		SSL_CTX_set_options(pvt->_ctx,SSL_OP_SINGLE_DH_USE);

		// set the certificate authority
		if (retval && !charstring::isNullOrEmpty(pvt->_ca)) {

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
		if (retval) {
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
			stdoutput.printf("  version: %s\n",pvt->_version);
			stdoutput.printf("  cert chain: %s\n",pvt->_cert);
			stdoutput.printf("  private key: %s\n",
					(pvt->_pk)?pvt->_pk:pvt->_cert);
			stdoutput.printf("  cert/pk loaded/valid: %s\n",
					(certloaded)?"yes":"no");
			stdoutput.printf("  ciphers: %s\n",pvt->_ciphers);
			stdoutput.printf("  depth: %d\n",pvt->_depth);
			stdoutput.printf("  ca: %s\n",pvt->_ca);
		#endif

		// build "bio" and "ssl"
		if (retval) {
			// this must be done here rather than above, as
			// apparently modifying the context after creating
			// an SSL prevents the SSL from working properly...
			initSubContext();

			pvt->_dirty=false;
		}

	#elif defined(RUDIMENTS_HAS_SSPI)

		// intialize flags...
		// 
		// On the client side, there's no way to specify a ca cert
		// store to use during automatic validation of the server's
		// certificate.  Thus, to support self-signed certificates,
		// certificates must be manually validated.
		// 
		// The flags SCH_CRED_MANUAL_CRED_VALIDATION and
		// ISC_REQ_MANUAL_CRED_VALIDATION are used together to
		// disable automatic validatation of server certificates.
		// These flags MUST be used together.  If one or the other is
		// missing then either automatic validation doesn't actually
		// get disabled, or initializing the security context fails
		// with SEC_E_UNSUPPORTED_FUNCTION.
		// 
		// Analogous flags do not need to be set on the server side
		// because servers never automatically validate client
		// certificates, and don't even request them by default.
		DWORD		credflags=(isclient)?
					SCH_CRED_MANUAL_CRED_VALIDATION:0;
		// FIXME: add SCH_CRED_REVOCATION_CHAIN_CHECK?
		uint32_t	ctxflags=(isclient)?
					(ISC_REQ_SEQUENCE_DETECT|
						ISC_REQ_REPLAY_DETECT|
						ISC_RET_EXTENDED_ERROR|
						ISC_REQ_MANUAL_CRED_VALIDATION|
						ISC_REQ_STREAM):
					(ASC_REQ_SEQUENCE_DETECT|
						ASC_REQ_REPLAY_DETECT|
						ASC_RET_EXTENDED_ERROR|
						ASC_REQ_STREAM);

		// decide on the protocol version
		DWORD	method=0;
		if (!charstring::compareIgnoringCase(
					pvt->_version,"SSL2")) {
			if (isclient) {
				#ifdef SP_PROT_SSL2_CLIENT
				method=SP_PROT_SSL2_CLIENT;
				#endif
			} else {
				#ifdef SP_PROT_SSL2_SERVER
				method=SP_PROT_SSL2_SERVER;
				#endif
			}
		} else if (!charstring::compareIgnoringCase(
					pvt->_version,"SSL3")) {
			if (isclient) {
				#ifdef SP_PROT_SSL3_CLIENT
				method=SP_PROT_SSL3_CLIENT;
				#endif
			} else {
				#ifdef SP_PROT_SSL3_SERVER
				method=SP_PROT_SSL3_SERVER;
				#endif
			}
		} else if (!charstring::compareIgnoringCase(
					pvt->_version,"TLS1")) {
			if (isclient) {
				#ifdef SP_PROT_TLS1_CLIENT
				method=SP_PROT_TLS1_CLIENT;
				#endif
			} else {
				#ifdef SP_PROT_TLS1_SERVER
				method=SP_PROT_TLS1_SERVER;
				#endif
			}
		} else if (!charstring::compareIgnoringCase(
					pvt->_version,"TLS1.1")) {
			if (isclient) {
				#ifdef SP_PROT_TLS1_1_CLIENT
				method=SP_PROT_TLS1_1_CLIENT;
				#endif
			} else {
				#ifdef SP_PROT_TLS1_1_SERVER
				method=SP_PROT_TLS1_1_SERVER;
				#endif
			}
		} else if (!charstring::compareIgnoringCase(
					pvt->_version,"TLS1.2")) {
			if (isclient) {
				#ifdef SP_PROT_TLS1_2_CLIENT
				method=SP_PROT_TLS1_2_CLIENT;
				#endif
			} else {
				#ifdef SP_PROT_TLS1_2_SERVER
				method=SP_PROT_TLS1_2_SERVER;
				#endif
			}
		}
		if (!method) {
			if (isclient) {
				method=
				(0
				#ifdef SP_PROT_TLS1_2_CLIENT
					|SP_PROT_TLS1_2_CLIENT
				#endif
				#ifdef SP_PROT_TLS1_1_CLIENT
					|SP_PROT_TLS1_1_CLIENT
				#endif
				#ifdef SP_PROT_TLS1_CLIENT
					|SP_PROT_TLS1_CLIENT
				#endif
				#ifdef SP_PROT_SSL3_CLIENT
					|SP_PROT_SSL3_CLIENT
				#endif
				#if !defined(SP_PROT_TLS1_2_CLIENT) && \
					!defined(SP_PROT_TLS1_1_CLIENT) && \
					!defined(SP_PROT_TLS1_CLIENT) && \
					!defined(SP_PROT_SSL3_CLIENT)
					|SP_PROT_SSL2_CLIENT
				#endif
				);
			} else {
				method=
				(0
				#ifdef SP_PROT_TLS1_2_SERVER
					|SP_PROT_TLS1_2_SERVER
				#endif
				#ifdef SP_PROT_TLS1_2_SERVER
					|SP_PROT_TLS1_1_SERVER
				#endif
				#ifdef SP_PROT_TLS1_2_SERVER
					|SP_PROT_TLS1_SERVER
				#endif
				#ifdef SP_PROT_TLS1_2_SERVER
					|SP_PROT_SSL3_SERVER
				#endif
				#if !defined(SP_PROT_TLS1_2_SERVER) && \
					!defined(SP_PROT_TLS1_1_SERVER) && \
					!defined(SP_PROT_TLS1_SERVER) && \
					!defined(SP_PROT_SSL3_SERVER)
					|SP_PROT_SSL2_SERVER
				#endif
				);
			}
		}

		// set the certificate and private key
		if (!charstring::isNullOrEmpty(pvt->_cert)) {

			// cert specified...

			// don't use default creds
			if (isclient) {
				credflags|=SCH_CRED_NO_DEFAULT_CREDS;
			}

			// open the specified certificate store...

			// first look for a file named pvt->_cert
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

				// FIXME: support private key password

				// FIXME: This automatically loads the private
				// key for .pkx files, but not for .pem files,
				// causing CryptAcquireCertificatePrivateKey
				// below to fail.

				if (cctx) {

					// verify that the cert
					// contained a private key
					HCRYPTPROV_OR_NCRYPT_KEY_HANDLE	kh=NULL;
					CryptAcquireCertificatePrivateKey(
								cctx,
								0,
								NULL,
								&kh,
								NULL,
								NULL);

					pvt->_cctx=new PCCERT_CONTEXT[1];
					pvt->_cctx[0]=cctx;
					pvt->_cctxcount=1;
				}

			} else {

				// clear any error set by the previous call to
				// CertOpenStore()
				error::clearNativeError();

				// if there was no cert file with the specified
				// name, then look for the cert in the system
				// store...

				// assume that pvt->_cert is formatted like:
				// * location:store:subject
				// or
				// * store:subject
				// 	(location presumed to be
				//	CERT_SYSTEM_STORE_CURRENT_USER)
				// or
				// * subject
				// 	(location presumed to be
				//	CERT_SYSTEM_STORE_CURRENT_USER,
				//	store presumed to be "MY")

				// split/process pvt->_cert 
				char		**parts=NULL;
				uint64_t	partcount=0;
				charstring::split(pvt->_cert,":",true,
							&parts,&partcount);
				DWORD	location=CERT_SYSTEM_STORE_CURRENT_USER;
				const char	*store="MY";
				const char	*subject="";
				if (partcount>2) {
					location=getLocation(parts[0]);
					store=parts[1];
					subject=parts[2];
				} else if (partcount==2) {
					store=parts[0];
					subject=parts[1];
				} else if (partcount==1) {
					subject=parts[0];
				}

				// first look for the specified store
				pvt->_cstore=CertOpenStore(
						CERT_STORE_PROV_SYSTEM_A,
						0,
						NULL,
						location,
						store);

				if (pvt->_cstore) {

					// get the cert who's subject
					// includes the specified string
					WCHAR	*subjectw=
					asciiToUnicode(subject);
					PCCERT_CONTEXT	cctx=
					CertFindCertificateInStore(
							pvt->_cstore,
							X509_ASN_ENCODING|
							PKCS_7_ASN_ENCODING,
							0,
							CERT_FIND_SUBJECT_STR,
							subjectw,
							NULL);
					delete[] subjectw;

					// FIXME: support private key password

					if (cctx) {

						// verify that the cert
						// contained a private key
					HCRYPTPROV_OR_NCRYPT_KEY_HANDLE	kh=NULL;
					CryptAcquireCertificatePrivateKey(
								cctx,
								0,
								NULL,
								&kh,
								NULL,
								NULL);

						pvt->_cctx=
						new PCCERT_CONTEXT[1];
						pvt->_cctx[0]=cctx;
						pvt->_cctxcount=1;
					}
				}

				// clean up 
				for (uint64_t i=0; i<partcount; i++) {
					delete[] parts[i];
				}
				delete[] parts;
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
			credflags|=SCH_CRED_USE_DEFAULT_CREDS;
		}

		// set the certificate authority
		if (retval && !charstring::isNullOrEmpty(pvt->_ca)) {

			// FIXME: support directories...

			// open the specified certificate authority store
			pvt->_castore=CertOpenStore(
					CERT_STORE_PROV_FILENAME_A,
					X509_ASN_ENCODING|
					PKCS_7_ASN_ENCODING,
					NULL,
					0,
					pvt->_ca);

			if (!pvt->_castore) {

				// clear any error set by the previous call to
				// CertOpenStore()
				error::clearNativeError();

				// if there was no ca file with the specified
				// name, then look for the ca in the system
				// store...

				// assume that pvt->_ca is formatted like:
				// * location:store
				// or
				// * store:subject
				// 	(location presumed to be
				//	CERT_SYSTEM_STORE_CURRENT_USER)

				// split/process pvt->_ca 
				char		**parts=NULL;
				uint64_t	partcount=0;
				charstring::split(pvt->_ca,":",true,
							&parts,&partcount);
				DWORD	location=CERT_SYSTEM_STORE_CURRENT_USER;
				const char	*store="MY";
				if (partcount>1) {
					location=getLocation(parts[0]);
					store=parts[1];
				} else if (partcount==1) {
					store=parts[0];
				}

				// open the specified certificate
				// authority store
				pvt->_castore=CertOpenStore(
						CERT_STORE_PROV_SYSTEM_A,
						0,
						NULL,
						location,
						store);

				// clean up 
				for (uint64_t i=0; i<partcount; i++) {
					delete[] parts[i];
				}
				delete[] parts;
			}

			if (!pvt->_castore) {
				setNativeError();
				retval=false;
			}
		}

		// set whether to validate peer or not
		if (retval && pvt->_validatepeer && !isclient) {
			ctxflags|=ASC_RET_MUTUAL_AUTH;
		}

		// open system CA and ROOT stores
		if (retval && !pvt->_syscastore) {
			pvt->_syscastore=CertOpenSystemStore(NULL,"CA");
			if (!pvt->_syscastore) {
				setNativeError();
				retval=false;
			}
		}
		if (retval && !pvt->_sysrootstore) {
			pvt->_sysrootstore=CertOpenSystemStore(NULL,"ROOT");
			if (!pvt->_sysrootstore) {
				setNativeError();
				retval=false;
			}
		}

		#ifdef DEBUG_TLS
			stdoutput.printf("  version: %s\n",pvt->_version);
			stdoutput.printf("  cert chain: %s - (%s) - (%s)\n",
					pvt->_cert,pvt->_pkpwd,
					(pvt->_cctxcount)?"valid":"invalid");
			if (pvt->_cctx!=defaultcctx) {
				tlscertificate	cert;
				cert.setCertificate(pvt->_cctx[0]->pCertInfo);
			}
			stdoutput.printf("  use default creds: %s\n",
					(credflags&SCH_CRED_USE_DEFAULT_CREDS)?
					"1":"0");
		#endif

		// set ciphers
		// (automatically adds ephemeral diffie-hellman on server)
		if (retval) {
			getCipherAlgs(pvt->_ciphers,isclient,
					&pvt->_algids,&pvt->_algidcount);
		}

		#ifdef DEBUG_TLS
			stdoutput.printf("  depth: %d\n",pvt->_depth);
			stdoutput.printf("  ca: %s\n",pvt->_ca);
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
			pvt->_scred.grbitEnabledProtocols=method;
			pvt->_scred.dwFlags=credflags;

			// set file descriptor
			pvt->_gctx.setFileDescriptor(pvt->_fd);

			// set security context flags
			pvt->_gctx.setDesiredFlags(ctxflags);

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
		int	ret=SSL_accept(pvt->_ssl);
		setError(ret);
		if (ret<1) {
			// reinit the ssl and bio
			freeSubContext();
			initSubContext();
		} else if (pvt->_validatepeer && !isPeerCertValid()) {
			ret=0;
		}
		return (ret==1);
	#elif defined(RUDIMENTS_HAS_SSPI)
		bool	ret=pvt->_gctx.accept();
		setError(ret);
		if (ret && pvt->_validatepeer) {
			ret=isPeerCertValid();
		}
		return ret;
	#else
		RUDIMENTS_SET_ENOSYS
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
		if (pvt->_ssl && loadPeerCert()) {
			tlscert=new tlscertificate();
			tlscert->setCertificate((void *)pvt->_peercert);
		}
	#elif defined(RUDIMENTS_HAS_SSPI)
		if (loadPeerCert()) {
			tlscert=new tlscertificate();
			tlscert->setCertificate(
				(void *)pvt->_peercert->pCertInfo);
		}
	#endif
	#ifdef DEBUG_TLS
		stdoutput.printf("}\n");
	#endif
	return tlscert;
}

bool tlscontext::loadPeerCert() {
	#if defined(RUDIMENTS_HAS_SSL)
		if (pvt->_peercert) {
			return true;
		}
		pvt->_peercert=SSL_get_peer_certificate(pvt->_ssl);
		return (pvt->_peercert!=NULL);
	#elif defined(RUDIMENTS_HAS_SSPI)
		if (pvt->_peercert) {
			return true;
		}
		pvt->_peercert=NULL;
		SECURITY_STATUS	sstatus=QueryContextAttributes(
					(_SecHandle *)pvt->_gctx.getContext(),
					SECPKG_ATTR_REMOTE_CERT_CONTEXT,
					&pvt->_peercert);
		if (SSPI_ERROR(sstatus)) {
			#ifdef DEBUG_TLS
				stdoutput.write(" failed "
					"(QueryContextAttributes("
					"SECPKG_ATTR_REMOTE_CERT_CONTEXT))\n");
			#endif
			setError(sstatus,gss::getSspiStatusString(sstatus));
			pvt->_peercert=NULL;
		}
		return (pvt->_peercert!=NULL);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
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
		RUDIMENTS_SET_ENOSYS
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
		RUDIMENTS_SET_ENOSYS
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
		RUDIMENTS_SET_ENOSYS
		return 0;
	#endif
}

bool tlscontext::close() {
	clearError();
	#if defined(RUDIMENTS_HAS_SSL)
		// FIXME: It ought to be ok to do this, but doing it on either
		// side causes subsequent SSL_connects to fail and disabling
		// the session cache doesn't help.
		/*int	ret=1;
		if (pvt->_ssl) {
			ret=SSL_shutdown(pvt->_ssl);
			if (!ret) {
				clearError();
				ret=SSL_shutdown(pvt->_ssl);
			}
			setError(ret);
		}
		return (ret==1);*/
		return true;
	#elif defined(RUDIMENTS_HAS_SSPI)
		// FIXME: send the other side a close-notify message
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

ssize_t tlscontext::getSizeMax() {
	#if defined(RUDIMENTS_HAS_SSPI)
		return pvt->_gctx.getSizeMax();
	#else
		return SSIZE_MAX;
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
		if (!pvt->_errorstr.getStringLength()) {
			const char	*str="";
			switch (SSL_get_error(pvt->_ssl,ret)) {
				case SSL_ERROR_NONE:
					str="SSL_ERROR_NONE";
					break;
				case SSL_ERROR_ZERO_RETURN:
					str="SSL_ERROR_ZERO_RETURN";
					break;
				case SSL_ERROR_WANT_READ:
					str="SSL_ERROR_WANT_READ";
					break;
				case SSL_ERROR_WANT_WRITE:
					str="SSL_ERROR_WANT_WRITE";
					break;
				case SSL_ERROR_WANT_CONNECT:
					str="SSL_ERROR_WANT_CONNECT";
					break;
				#ifdef SSL_ERROR_WANT_ACCEPT
				case SSL_ERROR_WANT_ACCEPT:
					str="SSL_ERROR_WANT_ACCEPT";
					break;
				#endif
				case SSL_ERROR_WANT_X509_LOOKUP:
					str="SSL_ERROR_WANT_X509_LOOKUP";
					break;
				#ifdef SSL_ERROR_WANT_ASYNC
				case SSL_ERROR_WANT_ASYNC:
					str="SSL_ERROR_WANT_ASYNC";
					break;
				#endif
				case SSL_ERROR_SYSCALL:
					str="SSL_ERROR_SYSCALL";
					break;
				case SSL_ERROR_SSL:
					str="SSL_ERROR_SSL";
					break;
				default:
					str="";
			}
			pvt->_error=ret;
			pvt->_errorstr.append(str);
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
		linkedlist< char * >	_san;
};

tlscertificate::tlscertificate() {
	pvt=new tlscertificateprivate;
	initCertificate();
}

tlscertificate::~tlscertificate() {
	freeCertificate();
	delete pvt;
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
	for (linkedlistnode< char * > *node=pvt->_san.getFirst();
					node; node=node->getNext()) {
		delete[] node->getValue();
	}
	pvt->_san.clear();
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

datetime *tlscertificate::getValidFrom() {
	return &pvt->_validfrom;
}

datetime *tlscertificate::getValidTo() {
	return &pvt->_validto;
}

const char *tlscertificate::getSubject() {
	return pvt->_subject;
}

const char *tlscertificate::getCommonName() {
	return pvt->_commonname;
}

const char *tlscertificate::getPublicKeyAlgorithm() {
	return pvt->_pkalg;
}

const unsigned char *tlscertificate::getPublicKey() {
	return pvt->_pk;
}

uint64_t tlscertificate::getPublicKeyByteLength() {
	return pvt->_pklen;
}

uint64_t tlscertificate::getPublicKeyBitLength() {
	return pvt->_pkbits;
}

linkedlist< char * > *tlscertificate::getSubjectAlternateNames() {
	return &pvt->_san;
}

#if defined(RUDIMENTS_HAS_SSL)
void asn1timeToDateTime(datetime *dt, ASN1_TIME *asn1t) {
	const char	*asn1str=(const char *)asn1t->data;
	char		str[22];
	if (asn1t->type==V_ASN1_UTCTIME) {
		// mm/
		str[0]=asn1str[2];
		str[1]=asn1str[3];
		str[2]='/';
		// dd/
		str[3]=asn1str[4];
		str[4]=asn1str[5];
		str[5]='/';
		// yyyy
		if (asn1str[0]>='7') {
			str[6]='1';
			str[7]='9';
		} else {
			str[6]='2';
			str[7]='0';
		}
		str[8]=asn1str[0];
		str[9]=asn1str[1];
	} else {
		// mm/
		str[0]=asn1str[4];
		str[1]=asn1str[5];
		str[2]='/';
		// dd/
		str[3]=asn1str[6];
		str[4]=asn1str[7];
		str[5]='/';
		// yyyy
		str[6]=asn1str[0];
		str[7]=asn1str[1];
		str[8]=asn1str[2];
		str[9]=asn1str[3];
	}
	str[10]=' ';
	// hh:
	str[11]=asn1str[8];
	str[12]=asn1str[9];
	str[13]=':';
	// mm:
	str[14]=asn1str[10];
	str[15]=asn1str[11];
	str[16]=':';
	// ss:
	str[17]='0';
	str[18]='0';
	str[19]=' ';
	str[20]=asn1str[12];
	str[21]='\0';
	dt->initialize(str);
}
#endif

#ifdef RUDIMENTS_HAS_SSPI
// FIXME: move to charstring class
static CHAR *unicodeToAscii(const WCHAR *in) {

	BOOL	useddefaultchar;
	int32_t	size=WideCharToMultiByte(CP_ACP,0,in,-1,NULL,0,NULL,NULL);
	if (!size) {
		return NULL;
	}

	CHAR	*out=new char[size];
	if (!WideCharToMultiByte(CP_ACP,0,in,-1,out,size,
						"?",&useddefaultchar)) {
		delete[] out;
		out=NULL;
	}
	return out;
}
#endif

void tlscertificate::setCertificate(void *cert) {

	freeCertificate();
	initCertificate();

	#if defined(RUDIMENTS_HAS_SSL)

		X509	*c=(X509 *)cert;

		// get the version
		pvt->_version=X509_get_version(c);

		// get serial number
		pvt->_serialnumber=ASN1_INTEGER_get(X509_get_serialNumber(c));

		// get signature algorithm
		pvt->_sigalg=new char[256];
		OBJ_obj2txt(pvt->_sigalg,256,
			#ifdef RUDIMENTS_HAS_X509_GET_SIGNATURE_NID
				OBJ_nid2obj(X509_get_signature_nid(c)),
			#else
				c->sig_alg->algorithm,
			#endif
				0);
		// FIXME: signature algorithm parameters

		// get the issuer
		X509_NAME	*issuer=X509_get_issuer_name(c);
		pvt->_issuer=X509_NAME_oneline(issuer,NULL,0);

		// get valid-from
		asn1timeToDateTime(&pvt->_validfrom,X509_get_notBefore(c));

		// get valid-to
		asn1timeToDateTime(&pvt->_validto,X509_get_notAfter(c));

		// get the subject
		X509_NAME	*subject=X509_get_subject_name(c);
		pvt->_subject=X509_NAME_oneline(subject,NULL,0);

		// get the public key algorithm
		EVP_PKEY	*pubkey=X509_get_pubkey(c);
		pvt->_pkalg=charstring::duplicate(
					(pubkey)?
					OBJ_nid2ln(
					#ifdef RUDIMENTS_HAS_EVP_PKEY_BASE_ID
						EVP_PKEY_base_id(pubkey)
					#else
						pubkey->type
					#endif
					):NULL);
		// FIXME: public key algorithm parameters

		// get the public key
		pvt->_pklen=EVP_PKEY_size(pubkey);
		pvt->_pk=(unsigned char *)bytestring::duplicate(
					#ifdef RUDIMENTS_HAS_EVP_PKEY_GET0
						EVP_PKEY_get0(pubkey)
					#else
						pubkey->pkey.ptr
					#endif
					,pvt->_pklen);
		pvt->_pkbits=EVP_PKEY_bits(pubkey);
		EVP_PKEY_free(pubkey);

		// FIXME: issuer unique id
		// FIXME: subject unique id

		// get the subject alternate names
		STACK_OF(GENERAL_NAME)	*names=(STACK_OF(GENERAL_NAME) *)
						X509_get_ext_d2i(c,
							NID_subject_alt_name,
							NULL,NULL);
		if (names) {
			for (int i=0; i<sk_GENERAL_NAME_num(names); i++) {

				const GENERAL_NAME	*cur=
					sk_GENERAL_NAME_value(names,i);

				// FIXME: get other extensions
				if (cur->type==GEN_DNS) {
					ASN1_IA5STRING	*dnsia5;
				#ifdef RUDIMENTS_HAS_SSL_DNSNAME
					dnsia5=cur->d.dNSName;
				#else
					dnsia5=cur->d.ia5;
				#endif
				#ifdef RUDIMENTS_HAS_ASN1_STRING_GET0_DATA
					const unsigned char	*dnsname=
						ASN1_STRING_get0_data(dnsia5);
				#else
					char *dnsname=(char *)
						ASN1_STRING_data(dnsia5);
				#endif
					size_t	asn1len=
						ASN1_STRING_length(dnsia5);
					size_t	strlen=
						charstring::length(dnsname);
					if (asn1len==strlen) {
						pvt->_san.append(
							charstring::duplicate(
							(const char *)dnsname));
					}
				}
			}

			sk_GENERAL_NAME_pop_free(names,GENERAL_NAME_free);
		}

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
				// Visual C++ 2002- doesn't like the ULL suffix
				#if defined(_MSC_VER) && (_MSC_VER <= 1300)
					11644473600000000
				#else
					11644473600000000ULL
				#endif
				;
		pvt->_validfrom.initialize(t/1000000,t%1000000);

		// get valid-to
		t=(((((uint64_t)c->NotAfter.dwHighDateTime)<<32)|
				c->NotAfter.dwLowDateTime)/10)-
				// Visual C++ 2002- doesn't like the ULL suffix
				#if defined(_MSC_VER) && (_MSC_VER <= 1300)
					11644473600000000
				#else
					11644473600000000ULL
				#endif
				;
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

		// get the public key algorithm
		pvt->_pkalg=charstring::duplicate(
				c->SubjectPublicKeyInfo.Algorithm.pszObjId);
		// FIXME: public key algorithm parameters

		// get the public key
		pvt->_pklen=c->SubjectPublicKeyInfo.PublicKey.cbData;
		pvt->_pk=(unsigned char *)bytestring::duplicate(
				c->SubjectPublicKeyInfo.PublicKey.pbData,
				pvt->_pklen);
		pvt->_pkbits=(pvt->_pklen*8)-
				c->SubjectPublicKeyInfo.PublicKey.cUnusedBits;

		// FIXME: issuer unique id
		// FIXME: subject unique id

		// get the subject alternate names
		for (DWORD i=0; i<c->cExtension; i++) {

			CERT_EXTENSION	*ext=&c->rgExtension[i];

			// FIXME: get other extensions
			if (!charstring::compare(ext->pszObjId,
						szOID_SUBJECT_ALT_NAME) ||
				!charstring::compare(ext->pszObjId,
						szOID_SUBJECT_ALT_NAME2)) {

				DWORD	anisize=0;
				if (CryptDecodeObject(
					X509_ASN_ENCODING,
					ext->pszObjId,
					ext->Value.pbData,
					ext->Value.cbData,
					0,NULL,&anisize)==FALSE) {
					continue;
				}

				CERT_ALT_NAME_INFO	*ani=
					(CERT_ALT_NAME_INFO *)
						new unsigned char[anisize];
				if (CryptDecodeObject(
					X509_ASN_ENCODING,
					ext->pszObjId,
					ext->Value.pbData,
					ext->Value.cbData,
					0,ani,&anisize)==FALSE) {
					delete[] (unsigned char *)ani;
					continue;
				}
					
				for (DWORD j=0; j<ani->cAltEntry; j++) {
					CERT_ALT_NAME_ENTRY	*ane=
							&ani->rgAltEntry[j];
					if (ane->dwAltNameChoice==
						CERT_ALT_NAME_DNS_NAME) {
						pvt->_san.append(
							unicodeToAscii(
							ane->pwszDNSName));
					}
				}

				delete[] (unsigned char *)ani;
			}
		}
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
		stdoutput.printf("    public key length: %lld\n",pvt->_pklen);
		stdoutput.printf("    public key bits: %lld\n",pvt->_pkbits);
		stdoutput.printf("    common name: %s\n",pvt->_commonname);
		stdoutput.printf("    subject alternate names:\n");
		for (linkedlistnode< char * > *node=pvt->_san.getFirst();
						node; node=node->getNext()) {
			stdoutput.printf("        %s\n",node->getValue());
		}
		stdoutput.printf("  }\n");
	#endif
}
