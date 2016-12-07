// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#include <rudiments/charstring.h>
#include <rudiments/character.h>
#include <rudiments/environment.h>
#include <rudiments/filedescriptor.h>
#include <rudiments/bytestring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/linkedlist.h>
#include <rudiments/stdio.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/error.h>
#include <rudiments/gss.h>

//#define DEBUG_GSS 1
//#define DEBUG_GSS_WRAP 1
//#define DEBUG_GSS_SEND 1
//#define DEBUG_GSS_RECEIVE 1

#if defined(RUDIMENTS_HAS_GSS)

	#ifdef RUDIMENTS_HAS_GSSAPI_GSSAPI_EXT_H
		// for gss_str_to_oid
		#include <gssapi/gssapi_ext.h>
	#endif

	#ifndef RUDIMENTS_HAS_GSS_STR_TO_OID
		// for local gss_str_to_oid implementation
		#include "gssoid.cpp"
	#endif

	#ifndef RUDIMENTS_HAS_GSS_OID_TO_STR
		// for local gss_oid_to_str implementation
		// FIXME: ...
	#endif

	#ifndef RUDIMENTS_HAS_GSS_RELEASE_OID
		// for local gss_release_oid implementation
		// FIXME: ...
	#endif

	#ifndef RUDIMENTS_HAS_GSS_NAME_TYPES
		#ifdef RUDIMENTS_HAS_GSSAPI_GSSAPI_GENERIC_H
			// for gss_nt_* identifiers
			#include <gssapi/gssapi_generic.h>
		#endif
		#define GSS_C_NT_HOSTBASED_SERVICE gss_nt_service_name
		#define GSS_C_NT_USER_NAME gss_nt_user_name
	#endif

	#define	GSS_SIZE_MAX 65536

#elif defined(RUDIMENTS_HAS_SSPI)

	#include <schannel.h>
	#define SSPI_ERROR(sstatus)	((sstatus)<0)

	enum gss_mech_t {
		GSS_MECH_KRB=0,
		GSS_MECH_SCHANNEL,
		GSS_MECH_OTHER
	};

#else
	// for UINT_MAX
	#ifdef RUDIMENTS_HAVE_LIMITS_H
		#include <limits.h>
	#endif
#endif

#define TOKEN_FLAGS_TYPE_INITIATE	(1<<0)
#define TOKEN_FLAGS_TYPE_ACCEPT		(1<<1)
#define TOKEN_FLAGS_TYPE_DATA		(1<<2)

// FIXME: do something with these...
#define TOKEN_FLAGS_TYPE_MIC		(1<<3)
#define TOKEN_FLAGS_VERIFY		(1<<4)
#define TOKEN_FLAGS_ENCRYPTED		(1<<5)

class gssprivate {
	friend class gss;
		char	**_mechs;
};

gss::gss() {
	pvt=new gssprivate;
	pvt->_mechs=NULL;
}

gss::~gss() {
	clear();
}

const char * const *gss::getAvailableMechanisms() {

	clear();

	#ifdef DEBUG_GSS
		stdoutput.printf("Available Mechanisms {\n");
	#endif

	#if defined(RUDIMENTS_HAS_GSS)

		OM_uint32		major;
		OM_uint32		minor;
		gss_OID_set		mechs;
		major=gss_indicate_mechs(&minor,&mechs);
		if (major==GSS_S_COMPLETE && mechs->count) {

			pvt->_mechs=new char *[mechs->count+1];

			gssmechanism	scratch;

			for (size_t i=0; i<mechs->count; i++) {
				scratch.initialize(&mechs->elements[i]);
				pvt->_mechs[i]=charstring::duplicate(
							scratch.getString());
				#ifdef DEBUG_GSS
					stdoutput.printf("  %s\n",
							scratch.getString());
				#endif
			}

			pvt->_mechs[mechs->count]=NULL;

			gss_release_oid_set(&minor,&mechs);
		}

	#elif defined(RUDIMENTS_HAS_SSPI)

		ULONG		packagecount;
		PSecPkgInfo	packages;
		EnumerateSecurityPackages(&packagecount,&packages);
		pvt->_mechs=new char *[packagecount+1];
		for (ULONG i=0; i<packagecount; i++) {
			pvt->_mechs[i]=charstring::duplicate(packages[i].Name);
			#ifdef DEBUG_GSS
				stdoutput.printf("  %s\n",pvt->_mechs[i]);
			#endif
		}
		pvt->_mechs[packagecount]=NULL;

	#endif

	#ifdef DEBUG_GSS
		stdoutput.printf("}\n");
	#endif

	return pvt->_mechs;
}

void gss::clear() {
	if (pvt->_mechs) {
		for (char **m=pvt->_mechs; *m; m++) {
			delete[] *m;
		}
		delete[] pvt->_mechs;
		pvt->_mechs=NULL;
	}
}

bool gss::supportsGSS() {
	#if defined(RUDIMENTS_HAS_GSS)
		return true;
	#elif defined(RUDIMENTS_HAS_SSPI)
		return true;
	#else
		return false;
	#endif
}

const char *gss::getSspiStatusString(uint32_t status) {
	#if defined(RUDIMENTS_HAS_SSPI)
		switch (status) {
			case SEC_E_INSUFFICIENT_MEMORY:
				return "SEC_E_INSUFFICIENT_MEMORY";
				break;
			case SEC_E_INVALID_HANDLE:
				return "SEC_E_INVALID_HANDLE";
				break;
			case SEC_E_UNSUPPORTED_FUNCTION:
				return "SEC_E_UNSUPPORTED_FUNCTION";
				break;
			case SEC_E_TARGET_UNKNOWN:
				return "SEC_E_TARGET_UNKNOWN";
				break;
			case SEC_E_INTERNAL_ERROR:
				return "SEC_E_INTERNAL_ERROR";
				break;
			case SEC_E_SECPKG_NOT_FOUND:
				return "SEC_E_SECPKG_NOT_FOUND";
				break;
			case SEC_E_NOT_OWNER:
				return "SEC_E_NOT_OWNER";
				break;
			case SEC_E_CANNOT_INSTALL:
				return "SEC_E_CANNOT_INSTALL";
				break;
			case SEC_E_INVALID_TOKEN:
				return "SEC_E_INVALID_TOKEN";
				break;
			case SEC_E_CANNOT_PACK:
				return "SEC_E_CANNOT_PACK";
				break;
			case SEC_E_QOP_NOT_SUPPORTED:
				return "SEC_E_QOP_NOT_SUPPORTED";
				break;
			case SEC_E_NO_IMPERSONATION:
				return "SEC_E_NO_IMPERSONATION";
				break;
			case SEC_E_LOGON_DENIED:
				return "SEC_E_LOGON_DENIED";
				break;
			case SEC_E_UNKNOWN_CREDENTIALS:
				return "SEC_E_UNKNOWN_CREDENTIALS";
				break;
			case SEC_E_NO_CREDENTIALS:
				return "SEC_E_NO_CREDENTIALS";
				break;
			case SEC_E_MESSAGE_ALTERED:
				return "SEC_E_MESSAGE_ALTERED";
				break;
			case SEC_E_OUT_OF_SEQUENCE:
				return "SEC_E_OUT_OF_SEQUENCE";
				break;
			case SEC_E_NO_AUTHENTICATING_AUTHORITY:
				return "SEC_E_NO_AUTHENTICATING_AUTHORITY";
				break;
			case SEC_I_CONTINUE_NEEDED:
				return "SEC_I_CONTINUE_NEEDED";
				break;
			case SEC_I_COMPLETE_NEEDED:
				return "SEC_I_COMPLETE_NEEDED";
				break;
			case SEC_I_COMPLETE_AND_CONTINUE:
				return "SEC_I_COMPLETE_AND_CONTINUE";
				break;
			case SEC_I_LOCAL_LOGON:
				return "SEC_I_LOCAL_LOGON";
				break;
			case SEC_E_BAD_PKGID:
				return "SEC_E_BAD_PKGID";
				break;
			case SEC_E_CONTEXT_EXPIRED:
				return "SEC_E_CONTEXT_EXPIRED";
				break;
			case SEC_I_CONTEXT_EXPIRED:
				return "SEC_I_CONTEXT_EXPIRED";
				break;
			case SEC_E_INCOMPLETE_MESSAGE:
				return "SEC_E_INCOMPLETE_MESSAGE";
				break;
			case SEC_E_INCOMPLETE_CREDENTIALS:
				return "SEC_E_INCOMPLETE_CREDENTIALS";
				break;
			case SEC_E_BUFFER_TOO_SMALL:
				return "SEC_E_BUFFER_TOO_SMALL";
				break;
			case SEC_I_INCOMPLETE_CREDENTIALS:
				return "SEC_I_INCOMPLETE_CREDENTIALS";
				break;
			case SEC_I_RENEGOTIATE:
				return "SEC_I_RENEGOTIATE";
				break;
			case SEC_E_WRONG_PRINCIPAL:
				return "SEC_E_WRONG_PRINCIPAL";
				break;
			case SEC_I_NO_LSA_CONTEXT:
				return "SEC_I_NO_LSA_CONTEXT";
				break;
			case SEC_E_TIME_SKEW:
				return "SEC_E_TIME_SKEW";
				break;
			case SEC_E_UNTRUSTED_ROOT:
				return "SEC_E_UNTRUSTED_ROOT";
				break;
			case SEC_E_ILLEGAL_MESSAGE:
				return "SEC_E_ILLEGAL_MESSAGE";
				break;
			case SEC_E_CERT_UNKNOWN:
				return "SEC_E_CERT_UNKNOWN";
				break;
			case SEC_E_CERT_EXPIRED:
				return "SEC_E_CERT_EXPIRED";
				break;
			case SEC_E_ENCRYPT_FAILURE:
				return "SEC_E_ENCRYPT_FAILURE";
				break;
			case SEC_E_DECRYPT_FAILURE:
				return "SEC_E_DECRYPT_FAILURE";
				break;
			case SEC_E_ALGORITHM_MISMATCH:
				return "SEC_E_ALGORITHM_MISMATCH";
				break;
			case SEC_E_SECURITY_QOS_FAILED:
				return "SEC_E_SECURITY_QOS_FAILED";
				break;
			case SEC_E_UNFINISHED_CONTEXT_DELETED:
				return "SEC_E_UNFINISHED_CONTEXT_DELETED";
				break;
			case SEC_E_NO_TGT_REPLY:
				return "SEC_E_NO_TGT_REPLY";
				break;
			case SEC_E_NO_IP_ADDRESSES:
				return "SEC_E_NO_IP_ADDRESSES";
				break;
			case SEC_E_WRONG_CREDENTIAL_HANDLE:
				return "SEC_E_WRONG_CREDENTIAL_HANDLE";
				break;
			case SEC_E_CRYPTO_SYSTEM_INVALID:
				return "SEC_E_CRYPTO_SYSTEM_INVALID";
				break;
			case SEC_E_MAX_REFERRALS_EXCEEDED:
				return "SEC_E_MAX_REFERRALS_EXCEEDED";
				break;
			case SEC_E_MUST_BE_KDC:
				return "SEC_E_MUST_BE_KDC";
				break;
			case SEC_E_STRONG_CRYPTO_NOT_SUPPORTED:
				return "SEC_E_STRONG_CRYPTO_NOT_SUPPORTED";
				break;
			case SEC_E_TOO_MANY_PRINCIPALS:
				return "SEC_E_TOO_MANY_PRINCIPALS";
				break;
			case SEC_E_NO_PA_DATA:
				return "SEC_E_NO_PA_DATA";
				break;
			case SEC_E_PKINIT_NAME_MISMATCH:
				return "SEC_E_PKINIT_NAME_MISMATCH";
				break;
			case SEC_E_SMARTCARD_LOGON_REQUIRED:
				return "SEC_E_SMARTCARD_LOGON_REQUIRED";
				break;
			case SEC_E_SHUTDOWN_IN_PROGRESS:
				return "SEC_E_SHUTDOWN_IN_PROGRESS";
				break;
			case SEC_E_KDC_INVALID_REQUEST:
				return "SEC_E_KDC_INVALID_REQUEST";
				break;
			case SEC_E_KDC_UNABLE_TO_REFER:
				return "SEC_E_KDC_UNABLE_TO_REFER";
				break;
			case SEC_E_KDC_UNKNOWN_ETYPE:
				return "SEC_E_KDC_UNKNOWN_ETYPE";
				break;
			case SEC_E_UNSUPPORTED_PREAUTH:
				return "SEC_E_UNSUPPORTED_PREAUTH";
				break;
			case SEC_E_DELEGATION_REQUIRED:
				return "SEC_E_DELEGATION_REQUIRED";
				break;
			case SEC_E_BAD_BINDINGS:
				return "SEC_E_BAD_BINDINGS";
				break;
			case SEC_E_MULTIPLE_ACCOUNTS:
				return "SEC_E_MULTIPLE_ACCOUNTS";
				break;
			case SEC_E_NO_KERB_KEY:
				return "SEC_E_NO_KERB_KEY";
				break;
			case SEC_E_CERT_WRONG_USAGE:
				return "SEC_E_CERT_WRONG_USAGE";
				break;
			case SEC_E_DOWNGRADE_DETECTED:
				return "SEC_E_DOWNGRADE_DETECTED";
				break;
			case SEC_E_SMARTCARD_CERT_REVOKED:
				return "SEC_E_SMARTCARD_CERT_REVOKED";
				break;
			case SEC_E_ISSUING_CA_UNTRUSTED:
				return "SEC_E_ISSUING_CA_UNTRUSTED";
				break;
			case SEC_E_REVOCATION_OFFLINE_C:
				return "SEC_E_REVOCATION_OFFLINE_C";
				break;
			case SEC_E_PKINIT_CLIENT_FAILURE:
				return "SEC_E_PKINIT_CLIENT_FAILURE";
				break;
			case SEC_E_SMARTCARD_CERT_EXPIRED:
				return "SEC_E_SMARTCARD_CERT_EXPIRED";
				break;
			case SEC_E_NO_S4U_PROT_SUPPORT:
				return "SEC_E_NO_S4U_PROT_SUPPORT";
				break;
			case SEC_E_CROSSREALM_DELEGATION_FAILURE:
				return "SEC_E_CROSSREALM_DELEGATION_FAILURE";
				break;
			case SEC_E_REVOCATION_OFFLINE_KDC:
				return "SEC_E_REVOCATION_OFFLINE_KDC";
				break;
			case SEC_E_ISSUING_CA_UNTRUSTED_KDC:
				return "SEC_E_ISSUING_CA_UNTRUSTED_KDC";
				break;
			case SEC_E_KDC_CERT_EXPIRED:
				return "SEC_E_KDC_CERT_EXPIRED";
				break;
			case SEC_E_KDC_CERT_REVOKED:
				return "SEC_E_KDC_CERT_REVOKED";
				break;
			case SEC_I_SIGNATURE_NEEDED:
				return "SEC_I_SIGNATURE_NEEDED";
				break;
			case SEC_E_INVALID_PARAMETER:
				return "SEC_E_INVALID_PARAMETER";
				break;
			case SEC_E_DELEGATION_POLICY:
				return "SEC_E_DELEGATION_POLICY";
				break;
			case SEC_E_POLICY_NLTM_ONLY:
				return "SEC_E_POLICY_NLTM_ONLY";
				break;
			case SEC_I_NO_RENEGOTIATION:
				return "SEC_I_NO_RENEGOTIATION";
				break;
			case SEC_E_NO_CONTEXT:
				return "SEC_E_NO_CONTEXT";
				break;
			case SEC_E_PKU2U_CERT_FAILURE:
				return "SEC_E_PKU2U_CERT_FAILURE";
				break;
			case SEC_E_MUTUAL_AUTH_FAILED:
				return "SEC_E_MUTUAL_AUTH_FAILED";
				break;
			case SEC_E_OK:
				return "SEC_E_OK";
				break;
			default:
				return "";
		}
	#else
		return NULL;
	#endif
}


class gssmechanismprivate {
	friend class gssmechanism;
	private:
		char	*_str;

		#if defined(RUDIMENTS_HAS_GSS)
			gss_OID	_oid;
		#else
			void	*_oid;
		#endif
};

gssmechanism::gssmechanism() {
	pvt=new gssmechanismprivate;
	pvt->_str=NULL;
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_oid=GSS_C_NO_OID;
	#else
		pvt->_oid=NULL;
	#endif
}

gssmechanism::~gssmechanism() {
	clear();
	delete pvt;
}

bool gssmechanism::initialize(const char *str) {

	clear();

	if (charstring::isNullOrEmpty(str)) {
		return true;
	}

	pvt->_str=charstring::duplicate(str);

	#if defined(RUDIMENTS_HAS_GSS)
		gss_buffer_desc	mechbuffer;
		mechbuffer.value=(void *)str;
		mechbuffer.length=charstring::length(str);

		OM_uint32		major;
		OM_uint32		minor;
		major=gss_str_to_oid(&minor,&mechbuffer,&pvt->_oid);
		return (major==GSS_S_COMPLETE);
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_oid=(void *)charstring::duplicate(str);
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool gssmechanism::initialize(const void *oid) {

	clear();

	#if defined(RUDIMENTS_HAS_GSS)
		if ((gss_OID)oid==GSS_C_NO_OID) {
			return true;
		}

		OM_uint32		major;
		OM_uint32		minor;

		// create a string from the oid
		gss_buffer_desc	mechbuffer;
		major=gss_oid_to_str(&minor,(gss_OID)oid,&mechbuffer);
		if (major!=GSS_S_COMPLETE) {
			return false;
		}

		pvt->_str=new char[mechbuffer.length+1];
		charstring::copy(pvt->_str,
				(char *)mechbuffer.value,mechbuffer.length);
		pvt->_str[mechbuffer.length]='\0';

		gss_release_buffer(&minor,&mechbuffer);

		// create a new oid from the string
		// (you'd think there'd be a clean way to just copy
		// an oid, but there doesn't appear to be)
		mechbuffer.value=(void *)pvt->_str;
		mechbuffer.length=charstring::length(pvt->_str);

		major=gss_str_to_oid(&minor,&mechbuffer,&pvt->_oid);
		return (major==GSS_S_COMPLETE);

	#elif defined(RUDIMENTS_HAS_SSPI)
		if (charstring::isNullOrEmpty((const char *)oid)) {
			return true;
		}
		pvt->_oid=(void *)charstring::duplicate((const char *)oid);
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

void gssmechanism::clear() {
	#if defined(RUDIMENTS_HAS_GSS)
		if (pvt->_oid!=GSS_C_NO_OID) {
			OM_uint32	minor;
			gss_release_oid(&minor,&pvt->_oid);
		}
		pvt->_oid=GSS_C_NO_OID;
	#elif defined(RUDIMENTS_HAS_SSPI)
		delete[] pvt->_oid;
		pvt->_oid=NULL;
	#endif

	delete[] pvt->_str;
	pvt->_str=NULL;
}

const char *gssmechanism::getString() {
	return pvt->_str;
}

const void *gssmechanism::getObjectId() {
	return pvt->_oid;
}


class gsscredentialsprivate {
	friend class gsscredentials;
	private:
		#if defined(RUDIMENTS_HAS_GSS)
			OM_uint32		_major;
			OM_uint32		_minor;
		#elif defined(RUDIMENTS_HAS_SSPI)
			SECURITY_STATUS		_sstatus;
		#else
			uint32_t		_major;
			uint32_t		_minor;
		#endif

		stringbuffer			_status;

		const char			*_name;

		#if defined(RUDIMENTS_HAS_GSS)
			OM_uint32		_desiredlifetime;
			OM_uint32		_actuallifetime;
		#elif defined(RUDIMENTS_HAS_SSPI)
			TimeStamp		_actuallifetime;
		#else
			uint32_t		_desiredlifetime;
			uint32_t		_actuallifetime;
		#endif

		#if defined(RUDIMENTS_HAS_GSS)
			gss_OID_set		_desiredmechanisms;
			gss_OID_set		_actualmechanisms;

			gss_cred_usage_t	_credusage;
			gss_cred_id_t		_credentials;
		#elif defined(RUDIMENTS_HAS_SSPI)
			ULONG			_credusage;
			CredHandle		_credentials;
		#endif

		bool				_acquired;
		void				*_psd;

		linkedlist< gssmechanism * >	_dmlist;
		linkedlist< gssmechanism * >	_amlist;
};

gsscredentials::gsscredentials() {
	pvt=new gsscredentialsprivate;

	#if defined(RUDIMENTS_HAS_SSPI)
		pvt->_sstatus=0;
	#else
		pvt->_major=0;
		pvt->_minor=0;
	#endif

	pvt->_name=NULL;

	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_desiredlifetime=GSS_C_INDEFINITE;
		pvt->_actuallifetime=0;
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_actuallifetime.u.LowPart=0;
		pvt->_actuallifetime.u.HighPart=0;
	#else
		pvt->_desiredlifetime=UINT_MAX;
		pvt->_actuallifetime=0;
	#endif

	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_desiredmechanisms=GSS_C_NO_OID_SET;
		pvt->_actualmechanisms=GSS_C_NO_OID_SET;

		pvt->_credusage=GSS_C_BOTH;
		pvt->_credentials=GSS_C_NO_CREDENTIAL;
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_credusage=SECPKG_CRED_BOTH;
		bytestring::zero(&pvt->_credentials,sizeof(pvt->_credentials));
	#endif
	pvt->_acquired=false;
	pvt->_psd=NULL;
}

gsscredentials::~gsscredentials() {
	close();
	delete pvt;
}

bool gsscredentials::setKeytab(const char *keytab) {
	#if defined(RUDIMENTS_HAS_GSS)
		return (keytab)?
			environment::setValue("KRB5_KTNAME",keytab):
			environment::remove("KRB5_KTNAME");
	#elif defined(RUDIMENTS_HAS_SSPI)
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

const char *gsscredentials::getKeytab() {
	#if defined(RUDIMENTS_HAS_GSS)
		return environment::getValue("KRB5_KTNAME");
	#elif defined(RUDIMENTS_HAS_SSPI)
		return NULL;
	#else
		return NULL;
	#endif
}

void gsscredentials::setDesiredLifetime(uint32_t desiredlifetime) {
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_desiredlifetime=desiredlifetime;
	#elif defined(RUDIMENTS_HAS_SSPI)
		// do nothing...
	#else
		pvt->_desiredlifetime=desiredlifetime;
	#endif
}

uint32_t gsscredentials::getDesiredLifetime() {
	#if defined(RUDIMENTS_HAS_GSS)
		return pvt->_desiredlifetime;
	#elif defined(RUDIMENTS_HAS_SSPI)
		return 0;
	#else
		return pvt->_desiredlifetime;
	#endif
}

void gsscredentials::addDesiredMechanism(gssmechanism *mech) {
	pvt->_dmlist.append(mech);
}

void gsscredentials::removeDesiredMechanism(gssmechanism *mech) {
	pvt->_dmlist.removeAll(mech);
}

bool gsscredentials::inDesiredMechanisms(gssmechanism *mech) {

	// just return false for degenerate mechs
	#if defined(RUDIMENTS_HAS_GSS)
		if (!mech || (gss_OID)mech->getObjectId()==GSS_C_NO_OID) {
			return false;
		}
	#elif defined(RUDIMENTS_HAS_SSPI)
		if (!mech || !mech->getObjectId()) {
			return false;
		}
	#else
		if (!mech || !mech->getObjectId()) {
			return false;
		}
	#endif

	// just return false for degenerate lists
	if (!pvt->_dmlist.getLength()) {
		return false;
	}

	// look for the mech in the list
	for (linkedlistnode< gssmechanism * > *node=
					pvt->_dmlist.getFirst();
					node; node=node->getNext()) {
		if (!charstring::compare(
				node->getValue()->getString(),
				mech->getString())) {
			return true;
		}
	}
	return false;
}

void gsscredentials::clearDesiredMechanisms() {
	pvt->_dmlist.clear();
}

uint64_t gsscredentials::getDesiredMechanismCount() {
	return pvt->_dmlist.getLength();
}

gssmechanism *gsscredentials::getDesiredMechanism(uint64_t index) {
	uint64_t	i=0;
	for (linkedlistnode< gssmechanism * > *node=
					pvt->_dmlist.getFirst();
					node; node=node->getNext()) {
		if (i==index) {
			return node->getValue();
		}
		i++;
	}
	return NULL;
}

bool gsscredentials::acquireForService(const char *name) {
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_credusage=GSS_C_ACCEPT;
		return acquire(name,charstring::length(name),
						GSS_C_NT_HOSTBASED_SERVICE);
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_credusage=SECPKG_CRED_INBOUND;
		return acquire(name,0,NULL);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool gsscredentials::acquireForUser(const char *name) {
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_credusage=GSS_C_INITIATE;
		return acquire(name,charstring::length(name),
						GSS_C_NT_USER_NAME);
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_credusage=SECPKG_CRED_OUTBOUND;
		return acquire(name,0,NULL);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

void gsscredentials::setPackageSpecificData(void *psd) {
	pvt->_psd=psd;
}

bool gsscredentials::acquire(const char *name,
				size_t namelen,
				const void *nametype) {

	// release any previously acquired credentials
	close();

	bool	retval=false;

	#if defined(RUDIMENTS_HAS_GSS)

		#ifdef DEBUG_GSS
			stdoutput.printf("acquire credentials\n(%s/",name);
			if ((gss_OID)nametype==
					GSS_C_NT_HOSTBASED_SERVICE) {
				stdoutput.write("GSS_C_NT_HOSTBASED_SERVICE");
			}
			if ((gss_OID)nametype==(gss_OID)GSS_C_NT_USER_NAME) {
				stdoutput.write("GSS_C_NT_USER_NAME");
			}
			stdoutput.write(") - ");
		#endif

		OM_uint32	major;
		OM_uint32	minor;

		// keep track of the name for nametypes
		// where the name is a string
		if ((gss_OID)nametype==
				GSS_C_NT_HOSTBASED_SERVICE ||
			(gss_OID)nametype==
				GSS_C_NT_USER_NAME) {
			pvt->_name=name;
		}

		// Acquire credentials associated with "name"
		// where "name" is type "nametype"...

		// by default, we'll use "no name"
		gss_name_t	desiredname=GSS_C_NO_NAME;

		if (!charstring::isNullOrEmpty(name)) {

			// if a name was provided then use it...
			gss_buffer_desc	namebuffer;
			namebuffer.value=(void *)name;
			namebuffer.length=namelen+1;

			// create an "internal form" struct from the name...
			pvt->_major=gss_import_name(&pvt->_minor,
							&namebuffer,
							(gss_OID)nametype,
							&desiredname);
			if (pvt->_major!=GSS_S_COMPLETE) {
				#ifdef DEBUG_GSS
					stdoutput.write("failed "
							"(import name)\n\n");
				#endif
				return false;
			}
		}

		// assemble desired mechs...

		// release the old set and mark it nonexistent
		if (pvt->_desiredmechanisms!=GSS_C_NO_OID_SET) {
			gss_release_oid_set(&minor,&pvt->_desiredmechanisms);
			pvt->_desiredmechanisms=GSS_C_NO_OID_SET;
		}

		// populate the set from the list of mechs
		for (linkedlistnode< gssmechanism * > *node=
						pvt->_dmlist.getFirst();
						node; node=node->getNext()) {

			gss_OID	mechoid=
				(gss_OID)node->getValue()->getObjectId();

			// skip degenerate mechs
			if (mechoid==GSS_C_NO_OID) {
				continue;
			}

			// if the set doesn't exist already, then create it
			if (pvt->_desiredmechanisms==GSS_C_NO_OID_SET) {
				major=gss_create_empty_oid_set(&minor,
						&pvt->_desiredmechanisms);
				if (major!=GSS_S_COMPLETE) {
					pvt->_desiredmechanisms=
						GSS_C_NO_OID_SET;
					break;
				}
			}

			// add the mech
			gss_add_oid_set_member(&minor,mechoid,
						&pvt->_desiredmechanisms);
			// FIXME: what if this fails?
		}

		// acquire the credentials associated with the name...
		pvt->_major=gss_acquire_cred(&pvt->_minor,
						desiredname,
						pvt->_desiredlifetime,
						pvt->_desiredmechanisms,
						pvt->_credusage,
						&pvt->_credentials,
						&pvt->_actualmechanisms,
						&pvt->_actuallifetime);

		// success/failure
		retval=(pvt->_major==GSS_S_COMPLETE);

		// save the actual mechanisms
		if (retval &&
			pvt->_actualmechanisms!=GSS_C_NO_OID_SET &&
			pvt->_actualmechanisms->count) {

			for (uint64_t i=0;
				i<pvt->_actualmechanisms->count; i++) {

				gssmechanism	*mech=new gssmechanism;
				mech->initialize(
					&pvt->_actualmechanisms->elements[i]);
				pvt->_amlist.append(mech);
			}
		}

		// clean up
		if (desiredname!=GSS_C_NO_NAME) {
			gss_release_name(&minor,&desiredname);
		}
		if (pvt->_desiredmechanisms!=GSS_C_NO_OID_SET) {
			gss_release_oid_set(&minor,&pvt->_desiredmechanisms);
			pvt->_desiredmechanisms=GSS_C_NO_OID_SET;
		}

	#elif defined(RUDIMENTS_HAS_SSPI)

		#ifdef DEBUG_GSS
			stdoutput.printf("acquire credentials\n(%s/",name);
			if (pvt->_credusage==SECPKG_CRED_INBOUND) {
				stdoutput.write("inbound");
			} else if (pvt->_credusage==SECPKG_CRED_OUTBOUND) {
				stdoutput.write("outbound");
			} else {
				stdoutput.write("both");
			}
			stdoutput.write(") - ");
		#endif

		// keep track of the name
		pvt->_name=name;

		// try each desired mechanism...
		bool		first=true;
		const char	*mechname=NULL;
		linkedlistnode< gssmechanism * > *node=pvt->_dmlist.getFirst();
		for (;;) {

			// break at the end of the list of desired mechanisms
			// 
			// Although... We need to run through at least once,
			// even if no desired mechanisms were specified.  So,
			// make an exception in that case.
			if (!first && !node) {
				break;
			}

			// skip degenerate mechs
			// 
			// Although... We need to run through at least once,
			// even if no desired mechanisms were specified.  So,
			// make an exception in that case.
			if (!first && !charstring::isNullOrEmpty(
					node->getValue()->getString())) {
				continue;
			}

			// clean up to try again, if we're
			// not currenly on the first try
			if (!first) {
				FreeCredentialHandle(&pvt->_credentials);
			}

			// attempt to use the specified mech...
			mechname=(node)?node->getValue()->getString():NULL;

			// ...but fall back to the "Negotiate" mech
			// This will only happen if no mechs were specified,
			// or if only degenerate mechs were specified.
			if (!mechname) {
				mechname=TEXT("Negotiate");
			}
			#ifdef DEBUG_GSS
				stdoutput.printf("%s... ",mechname);
			#endif

			// acquire the credentials associated with the name...
			pvt->_sstatus=AcquireCredentialsHandle(
							(LPSTR)name,
							(SEC_CHAR *)mechname,
							pvt->_credusage,
							NULL,
							pvt->_psd,
							NULL,
							NULL,
							&pvt->_credentials,
							&pvt->_actuallifetime);

			// success/failure
			retval=!SSPI_ERROR(pvt->_sstatus);
			if (retval) {
				break;
			}

			// if we made it here, then we're
			// not still on the first try...
			first=false;

			// try the next mechanism
			if (node) {
				 node=node->getNext();
			}
		}

		// save the actual mechanism
		gssmechanism	*mech=new gssmechanism;
		mech->initialize(mechname);
		pvt->_amlist.append(mech);
	#else
		retval=false;
	#endif

	pvt->_acquired=retval;

	#ifdef DEBUG_GSS
	if (retval) {
		stdoutput.write("success...\n\n");
		stdoutput.write("Credentials {\n");
		stdoutput.printf("  name: %s\n",pvt->_name);
		#if !defined(RUDIMENTS_HAS_SSPI)
			stdoutput.printf("  desired lifetime: %d\n",
						pvt->_desiredlifetime);
			stdoutput.printf("  actual lifetime: %d\n",
						pvt->_actuallifetime);
		#endif
		stdoutput.write("  desired mechanisms:\n");
		for (linkedlistnode< gssmechanism * > *node=
					pvt->_dmlist.getFirst();
					node; node=node->getNext()) {
			stdoutput.printf("    %s\n",
				node->getValue()->getString());
		}
		stdoutput.write("  actual mechanisms:\n");
		for (linkedlistnode< gssmechanism * > *node=
					pvt->_amlist.getFirst();
					node; node=node->getNext()) {
			stdoutput.printf("    %s\n",
				node->getValue()->getString());
		}
		stdoutput.write("  credentials usage: ");
		#if defined(RUDIMENTS_HAS_GSS)
			if (pvt->_credusage==GSS_C_BOTH) {
				stdoutput.write("both\n");
			} else if (pvt->_credusage==GSS_C_INITIATE) {
				stdoutput.write("initiate\n");
			} else if (pvt->_credusage==GSS_C_ACCEPT) {
				stdoutput.write("accept\n");
			}
		#elif defined(RUDIMENTS_HAS_SSPI)
			if (pvt->_credusage==SECPKG_CRED_BOTH) {
				stdoutput.write("both\n");
			} else if (pvt->_credusage==SECPKG_CRED_OUTBOUND) {
				stdoutput.write("initiate\n");
			} else if (pvt->_credusage==SECPKG_CRED_INBOUND) {
				stdoutput.write("accept\n");
			}
		#endif
		stdoutput.write("}\n");
	} else {
		stdoutput.write("failed\n\n");
	}
	#endif

	return retval;
}

bool gsscredentials::acquired() {
	return pvt->_acquired;
}

void gsscredentials::close() {

	// release the credentials
	#if defined(RUDIMENTS_HAS_GSS)
		OM_uint32	minor;
		gss_release_cred(&minor,&pvt->_credentials);
	#elif defined(RUDIMENTS_HAS_SSPI)
		FreeCredentialHandle(&pvt->_credentials);
	#endif

	// reset the name
	pvt->_name=NULL;

	// reset the "actuals"
	#if defined(RUDIMENTS_HAS_GSS)
		if (pvt->_actualmechanisms!=GSS_C_NO_OID_SET) {
			OM_uint32	minor;
			gss_release_oid_set(&minor,&pvt->_actualmechanisms);
			pvt->_actualmechanisms=GSS_C_NO_OID_SET;
		}
	#endif
	for (linkedlistnode< gssmechanism * > *node=
					pvt->_amlist.getFirst();
					node; node=node->getNext()) {
		delete node->getValue();
	}
	pvt->_amlist.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_actuallifetime=0;
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_actuallifetime.u.LowPart=0;
		pvt->_actuallifetime.u.HighPart=0;
	#else
		pvt->_actuallifetime=UINT_MAX;
	#endif
	pvt->_acquired=false;
}

const char *gsscredentials::getName() {
	return pvt->_name;
}

uint32_t gsscredentials::getActualLifetime() {
	#if defined(RUDIMENTS_HAS_GSS)
		return pvt->_actuallifetime;
	#elif defined(RUDIMENTS_HAS_SSPI)
		// this loses precision...
		return (uint32_t)pvt->_actuallifetime.QuadPart;
	#else
		return pvt->_actuallifetime;
	#endif
}

bool gsscredentials::inActualMechanisms(gssmechanism *mech) {

	// just return false for degenerate mechs
	#if defined(RUDIMENTS_HAS_GSS)
		if (!mech || (gss_OID)mech->getObjectId()==GSS_C_NO_OID) {
			return false;
		}
	#elif defined(RUDIMENTS_HAS_SSPI)
		if (!mech || !mech->getObjectId()) {
			return false;
		}
	#else
		if (!mech || !mech->getObjectId()) {
			return false;
		}
	#endif

	// just return false for degenerate lists
	if (!pvt->_amlist.getLength()) {
		return false;
	}

	// look for the mech in the list
	for (linkedlistnode< gssmechanism * > *node=
					pvt->_amlist.getFirst();
					node; node=node->getNext()) {
		if (!charstring::compare(
				node->getValue()->getString(),
				mech->getString())) {
			return true;
		}
	}
	return false;
}

uint64_t gsscredentials::getActualMechanismCount() {
	return pvt->_amlist.getLength();
}

gssmechanism *gsscredentials::getActualMechanism(uint64_t index) {
	uint64_t	i=0;
	for (linkedlistnode< gssmechanism * > *node=
					pvt->_amlist.getFirst();
					node; node=node->getNext()) {
		if (i==index) {
			return node->getValue();
		}
		i++;
	}
	return NULL;
}

const void *gsscredentials::getCredentials() {
	#if defined(RUDIMENTS_HAS_GSS)
		return pvt->_credentials;
	#elif defined(RUDIMENTS_HAS_SSPI)
		return (pvt->_acquired)?&pvt->_credentials:NULL;
	#else
		RUDIMENTS_SET_ENOSYS
		return NULL;
	#endif
}

uint32_t gsscredentials::getMajorStatus() {
	#if defined(RUDIMENTS_HAS_GSS)
		return pvt->_major;
	#elif defined(RUDIMENTS_HAS_SSPI)
		return pvt->_sstatus;
	#else
		return 0;
	#endif
}

uint32_t gsscredentials::getMinorStatus() {
	#if defined(RUDIMENTS_HAS_GSS)
		return pvt->_minor;
	#elif defined(RUDIMENTS_HAS_SSPI)
		return 0;
	#else
		return 0;
	#endif
}

const char *gsscredentials::getStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_status.append("GSS - major:\n");
		setStatus(pvt->_major,GSS_C_GSS_CODE);
		pvt->_status.append("GSS - minor:\n");
		setStatus(pvt->_minor,GSS_C_GSS_CODE);
		pvt->_status.append("MECH - major:\n");
		setStatus(pvt->_major,GSS_C_MECH_CODE);
		pvt->_status.append("MECH - minor:\n");
		setStatus(pvt->_minor,GSS_C_MECH_CODE);
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_status.append("SSPI - status:\n");
		setStatus(pvt->_sstatus,0);
	#endif
	return pvt->_status.getString();
}

const char *gsscredentials::getGSSMajorStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		setStatus(pvt->_major,GSS_C_GSS_CODE);
	#elif defined(RUDIMENTS_HAS_SSPI)
		setStatus(pvt->_sstatus,0);
	#endif
	return pvt->_status.getString();
}

const char *gsscredentials::getGSSMinorStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		setStatus(pvt->_minor,GSS_C_GSS_CODE);
	#elif defined(RUDIMENTS_HAS_SSPI)
		setStatus(pvt->_sstatus,0);
	#endif
	return pvt->_status.getString();
}

const char *gsscredentials::getMechanismMajorStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		setStatus(pvt->_major,GSS_C_MECH_CODE);
	#elif defined(RUDIMENTS_HAS_SSPI)
		setStatus(pvt->_sstatus,0);
	#endif
	return pvt->_status.getString();
}

const char *gsscredentials::getMechanismMinorStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		setStatus(pvt->_minor,GSS_C_MECH_CODE);
	#elif defined(RUDIMENTS_HAS_SSPI)
		setStatus(pvt->_sstatus,0);
	#endif
	return pvt->_status.getString();
}

void gsscredentials::setStatus(uint32_t status, int32_t type) {

	#if defined(RUDIMENTS_HAS_GSS)
		gss_buffer_desc	statusbuffer;

		OM_uint32	msgctx=0;
		do {
			OM_uint32	major;
			OM_uint32	minor;
			major=gss_display_status(&minor,
						status,
						type,
						GSS_C_NO_OID,
						&msgctx,
						&statusbuffer);
			if (major!=GSS_S_COMPLETE) {
				break;
			}

			pvt->_status.append((unsigned char *)
						statusbuffer.value,
						statusbuffer.length);
			pvt->_status.append('\n');

			gss_release_buffer(&minor,&statusbuffer);
		} while (msgctx);

	#elif defined(RUDIMENTS_HAS_SSPI)

		const char	*str=gss::getSspiStatusString(status);
		pvt->_status.append(str)->append("\n");
		if (error::getNativeErrorNumber()) {
			char	*errstr=error::getNativeErrorString();
			pvt->_status.append(errstr)->append("\n");
			delete[] errstr;
		}
	#endif
}

class gsscontextprivate {
	friend class gsscontext;
	private:
		#if defined(RUDIMENTS_HAS_GSS)
			OM_uint32	_major;
			OM_uint32	_minor;
		#elif defined(RUDIMENTS_HAS_SSPI)
			SECURITY_STATUS	_sstatus;
		#else
			uint32_t	_major;
			uint32_t	_minor;
		#endif

		stringbuffer		_status;

		gsscredentials		*_credentials;

		filedescriptor		*_fd;

		#if defined(RUDIMENTS_HAS_GSS)
			OM_uint32	_desiredlifetime;
			OM_uint32	_actuallifetime;
		#elif defined(RUDIMENTS_HAS_SSPI)
			TimeStamp	_actuallifetime;
			DWORD		_maxmsgsize;
			DWORD		_streamheadersize;
			DWORD		_streamtrailersize;
			DWORD		_trailersize;
			DWORD		_blksize;
			bool		_freecredentials;
			gss_mech_t	_mech;
		#else
			uint32_t	_desiredlifetime;
			uint32_t	_actuallifetime;
		#endif

		gssmechanism		*_desiredmechanism;
		gssmechanism		_actualmechanism;

		#if defined(RUDIMENTS_HAS_GSS)
			OM_uint32	_desiredflags;
			OM_uint32	_actualflags;
		#elif defined(RUDIMENTS_HAS_SSPI)
			ULONG		_desiredflags;
			ULONG		_actualflags;
		#else
			uint32_t	_desiredflags;
			uint32_t	_actualflags;
		#endif

		const char		*_service;
		size_t			_servicelength;

		#if defined(RUDIMENTS_HAS_GSS)
			gss_ctx_id_t	_context;
		#elif defined(RUDIMENTS_HAS_SSPI)
			_SecHandle 	_context;
		#else
			void		*_context;
		#endif

		char			*_initiator;

		char			*_acceptor;

		bool			_isinitiator;
		bool			_isopen;

		bytebuffer		_readbuffer;
		uint64_t		_readbufferpos;

		gsstokenformat_t	_tokenformat;
};

gsscontext::gsscontext() : securitycontext() {
	pvt=new gsscontextprivate;
	pvt->_credentials=NULL;
	pvt->_fd=NULL;
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_desiredlifetime=GSS_C_INDEFINITE;
		pvt->_actuallifetime=0;
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_maxmsgsize=0;
		pvt->_streamheadersize=0;
		pvt->_streamtrailersize=0;
		pvt->_trailersize=0;
		pvt->_blksize=0;
		pvt->_mech=GSS_MECH_OTHER;
		pvt->_actuallifetime.u.LowPart=0;
		pvt->_actuallifetime.u.HighPart=0;
		pvt->_freecredentials=false;
	#else
		pvt->_desiredlifetime=UINT_MAX;
		pvt->_actuallifetime=0;
	#endif
	pvt->_desiredmechanism=NULL;
	pvt->_desiredflags=0;
	pvt->_actualflags=0;
	pvt->_service=NULL;
	pvt->_servicelength=0;
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_context=GSS_C_NO_CONTEXT;
	#elif defined(RUDIMENTS_HAS_SSPI)
		bytestring::zero(&pvt->_context,sizeof(pvt->_context));
	#endif
	pvt->_initiator=NULL;
	pvt->_acceptor=NULL;
	pvt->_isinitiator=false;
	pvt->_isopen=false;
	pvt->_readbufferpos=0;
	pvt->_tokenformat=GSSTOKENFORMAT_KRB;
}

gsscontext::~gsscontext() {
	close();
	delete pvt;
}

void gsscontext::setCredentials(gsscredentials *credentials) {
	#if defined(RUDIMENTS_HAS_SSPI)
		if (pvt->_freecredentials) {
			delete pvt->_credentials;
		}
		pvt->_freecredentials=false;
	#endif
	pvt->_credentials=credentials;
}

gsscredentials *gsscontext::getCredentials() {
	#if defined(RUDIMENTS_HAS_SSPI)
		if (pvt->_freecredentials) {
			return NULL;
		}
	#endif
	return pvt->_credentials;
}

void gsscontext::setFileDescriptor(filedescriptor *fd) {
	pvt->_fd=fd;
}

filedescriptor *gsscontext::getFileDescriptor() {
	return pvt->_fd;
}

void gsscontext::setDesiredLifetime(uint32_t desiredlifetime) {
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_desiredlifetime=desiredlifetime;
	#elif defined(RUDIMENTS_HAS_SSPI)
		// do nothing...
	#else
		pvt->_desiredlifetime=desiredlifetime;
	#endif
}

uint32_t gsscontext::getDesiredLifetime() {
	#if defined(RUDIMENTS_HAS_GSS)
		return pvt->_desiredlifetime;
	#elif defined(RUDIMENTS_HAS_SSPI)
		return 0;
	#else
		return pvt->_desiredlifetime;
	#endif
}

void gsscontext::setDesiredMechanism(gssmechanism *desiredmechanism) {
	pvt->_desiredmechanism=desiredmechanism;
}

gssmechanism *gsscontext::getDesiredMechanism() {
	return pvt->_desiredmechanism;
}

void gsscontext::setDesiredFlags(uint32_t desiredflags) {
	pvt->_desiredflags=desiredflags;
}

void gsscontext::setDesiredFlags(const char *desiredflags) {

	char		**df;
	uint64_t	dfcount;
	charstring::split(desiredflags,",",true,&df,&dfcount);

	pvt->_desiredflags=0;
	for (uint64_t i=0; i<dfcount; i++) {
		#ifdef GSS_C_DELEG_FLAG
		if (!charstring::compare(df[i],"GSS_C_DELEG_FLAG")) {
			pvt->_desiredflags|=GSS_C_DELEG_FLAG;
		}
		#endif
		#ifdef GSS_C_MUTUAL_FLAG
		if (!charstring::compare(df[i],"GSS_C_MUTUAL_FLAG")) {
			pvt->_desiredflags|=GSS_C_MUTUAL_FLAG;
		}
		#endif
		#ifdef GSS_C_REPLAY_FLAG
		if (!charstring::compare(df[i],"GSS_C_REPLAY_FLAG")) {
			pvt->_desiredflags|=GSS_C_REPLAY_FLAG;
		}
		#endif
		#ifdef GSS_C_SEQUENCE_FLAG
		if (!charstring::compare(df[i],"GSS_C_SEQUENCE_FLAG")) {
			pvt->_desiredflags|=GSS_C_SEQUENCE_FLAG;
		}
		#endif
		#ifdef GSS_C_CONF_FLAG
		if (!charstring::compare(df[i],"GSS_C_CONF_FLAG")) {
			pvt->_desiredflags|=GSS_C_CONF_FLAG;
		}
		#endif
		#if !defined(RUDIMENTS_HAS_SSPI)
			#ifdef GSS_C_INTEG_FLAG
			if (!charstring::compare(df[i],
						"GSS_C_INTEG_FLAG")) {
				pvt->_desiredflags|=GSS_C_INTEG_FLAG;
			}
			#endif
			#ifdef GSS_C_ANON_FLAG
			if (!charstring::compare(df[i],
						"GSS_C_ANON_FLAG")) {
				pvt->_desiredflags|=GSS_C_ANON_FLAG;
			}
			#endif
			#ifdef GSS_C_PROT_READY_FLAG
			if (!charstring::compare(df[i],
						"GSS_C_PROT_READY_FLAG")) {
				pvt->_desiredflags|=GSS_C_PROT_READY_FLAG;
			}
			#endif
			#ifdef GSS_C_TRANS_FLAG
			if (!charstring::compare(df[i],
						"GSS_C_TRANS_FLAG")) {
				pvt->_desiredflags|=GSS_C_TRANS_FLAG;
			}
			#endif
			#ifdef GSS_C_DELEG_POLICY_FLAG
			if (!charstring::compare(df[i],
						"GSS_C_DELEG_POLICY_FLAG")) {
				pvt->_desiredflags|=GSS_C_DELEG_POLICY_FLAG;
			}
			#endif
		#endif
		delete[] df[i];
	}
	delete[] df;
}

uint32_t gsscontext::getDesiredFlags() {
	return pvt->_desiredflags;
}

void gsscontext::setService(const char *service) {
	pvt->_service=service;
	pvt->_servicelength=charstring::length(service);
}

const char *gsscontext::getService() {
	return pvt->_service;
}

bool gsscontext::connect() {
	#if defined(RUDIMENTS_HAS_GSS)
		return initiate(pvt->_service,pvt->_servicelength,
						GSS_C_NT_HOSTBASED_SERVICE);
	#elif defined(RUDIMENTS_HAS_SSPI)
		return initiate(pvt->_service,0,NULL);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

void *gsscontext::getContext() {
	#if defined(RUDIMENTS_HAS_SSPI)
		return (void *)&pvt->_context;
	#else
		return NULL;
	#endif
}

void gsscontext::setTokenFormat(gsstokenformat_t tokenformat) {
	pvt->_tokenformat=tokenformat;
}

bool gsscontext::getMaxMessageSize(const char *mechname) {

	#if defined(RUDIMENTS_HAS_SSPI)
		if (!pvt->_maxmsgsize) {

			#ifdef DEBUG_GSS
				stdoutput.printf("get max message "
						"size for mech %s...\n",
						mechname);
			#endif

			PSecPkgInfo	pkginfo;
			pvt->_sstatus=QuerySecurityPackageInfo(
						(LPSTR)mechname,&pkginfo);
			if (SSPI_ERROR(pvt->_sstatus))	{
				#ifdef DEBUG_GSS
					stdoutput.printf(
						"failed "
						"(QuerySecurityPackageInfo)"
						"\n%s\n",
						getStatus());
				#endif
				return false;
			}
			pvt->_maxmsgsize=pkginfo->cbMaxToken;
			FreeContextBuffer(pkginfo);

			#ifdef DEBUG_GSS
				stdoutput.printf("max message size: %d\n",
							pvt->_maxmsgsize);
			#endif
		}
	#endif
	return true;
}

bool gsscontext::initiate(const char *name,
				size_t namelen,
				const void *nametype) {

	// release any previously initialized context
	close();

	#ifdef DEBUG_GSS
		stdoutput.write("initiate context...\n");
	#endif

	// There is currently no option to no wrap/encrypt or unwrap/decrypt
	// in this class, so confidentiality should always be requested.
	// If it's not, SSPI will return SEC_E_UNSUPPORTED_FUNCTION when
	// calling EncryptMessage/DecryptMessage.  MIT GSSPI doesn't appear to
	// mind if you call gss_wrap/gss_unwrap, but who knows what other
	// implementations might do.  Just to be safe, we'll force
	// confidentiality, whether it was requested or not.
	pvt->_desiredflags|=GSS_C_CONF_FLAG;

	#if defined(RUDIMENTS_HAS_GSS)

		bool	error=false;

		OM_uint32	minor;

		// by default, we'll use "no name"
		gss_name_t	desiredname=GSS_C_NO_NAME;

		if (!charstring::isNullOrEmpty(name)) {

			// if a name was provided then use it...
			gss_buffer_desc	namebuffer;
			namebuffer.value=(void *)name;
			namebuffer.length=namelen+1;

			// create an "internal form" struct from the name...
			pvt->_major=gss_import_name(&pvt->_minor,
							&namebuffer,
							(gss_OID)nametype,
							&desiredname);
			if (pvt->_major!=GSS_S_COMPLETE) {
				#ifdef DEBUG_GSS
					stdoutput.write("failed "
							"(import_name)\n\n");
				#endif
				return false;
			}
		}

		// initialize the context...
		gss_buffer_desc	inputtoken;
		inputtoken.length=0;
		gss_buffer_desc	outputtoken;

		gss_cred_id_t	credentials=
			(pvt->_credentials)?
			(gss_cred_id_t)pvt->_credentials->getCredentials():
			GSS_C_NO_CREDENTIAL;

		gss_OID	desiredmechoid=
			(pvt->_desiredmechanism)?
			(gss_OID)pvt->_desiredmechanism->getObjectId():
			GSS_C_NO_OID;
		gss_OID	actualmechoid=GSS_C_NO_OID;

		for (;;) {

			// attempt to init the context
			pvt->_major=gss_init_sec_context(
						&pvt->_minor,
						credentials,
						&pvt->_context,
						desiredname,
						desiredmechoid,
						pvt->_desiredflags,
						pvt->_desiredlifetime,
						GSS_C_NO_CHANNEL_BINDINGS,
						&inputtoken,
						&actualmechoid,
						&outputtoken,
						&pvt->_actualflags,
						&pvt->_actuallifetime);

			// free inputtoken if necessary
			if (inputtoken.length) {
				delete[] (unsigned char *)inputtoken.value;
			}

			// bail on error
			if (GSS_ERROR(pvt->_major)) {
				#ifdef DEBUG_GSS
					stdoutput.printf(
						"failed "
						"(init_sec_context)\n%s\n",
						getStatus());
				#endif
				close();
				error=true;
				break;
			}

			// send token to peer, if necessary
			if (outputtoken.length) {
				if (sendToken(TOKEN_FLAGS_TYPE_INITIATE,
						outputtoken.value,
						outputtoken.length)!=
						(ssize_t)outputtoken.length) {
					#ifdef DEBUG_GSS
						stdoutput.write(
							"failed (send)\n\n");
					#endif
					gss_release_buffer(&minor,
							&outputtoken);
					close();
					error=true;
					break;
				}
			}

			// clean up
			gss_release_buffer(&minor,&outputtoken);

			// receive token from peer,
			// into inputtoken, if necessary
			if (pvt->_major==GSS_S_CONTINUE_NEEDED) {

				uint32_t	flags=0;
				inputtoken.value=NULL;
				inputtoken.length=0;
				if (receiveToken(&flags,
						&inputtoken.value,
						&inputtoken.length)<=0 ||
					!checkFlags(flags,
						TOKEN_FLAGS_TYPE_ACCEPT)) {
					#ifdef DEBUG_GSS
						stdoutput.write(
							"failed (receive)\n\n");
					#endif
					delete[] (unsigned char *)
							inputtoken.value;
					gss_release_buffer(&minor,&outputtoken);
					close();
					error=true;
					break;
				}

			} else {

				// populate actual mechanism
				pvt->_actualmechanism.initialize(actualmechoid);

				// break out if we've completed the process
				break;
			}
		}

		// clean up
		if (desiredname!=GSS_C_NO_NAME) {
			gss_release_name(&minor,&desiredname);
		}

		// bail on error
		if (error || pvt->_major!=GSS_S_COMPLETE) {
			return false;
		}

	#elif defined(RUDIMENTS_HAS_SSPI)

		bool	error=false;

		// get maxiumum message size...
		// At this point, we don't definitively know what mech
		// (security package) we'll be using, so we'll get the
		// max message size from the Negotiate mech, which will
		// be the max message size for all mechs.
		if (!getMaxMessageSize("Negotiate")) {
			return false;
		}

		// input buffer
		BYTE	*inbuf=NULL;
		DWORD	inbufsize=0;

		SecBuffer         inputtoken[2];
		inputtoken[0].BufferType=SECBUFFER_TOKEN;

		inputtoken[1].BufferType=SECBUFFER_EMPTY;
		inputtoken[1].cbBuffer=0;
		inputtoken[1].pvBuffer=NULL;

		SecBufferDesc     inputtokendesc;
		inputtokendesc.ulVersion=SECBUFFER_VERSION;
		inputtokendesc.cBuffers=2;
		inputtokendesc.pBuffers=inputtoken;

		// output buffer
		BYTE	*outbuf=new BYTE[pvt->_maxmsgsize];

		SecBuffer         outputtoken;
		outputtoken.BufferType=SECBUFFER_TOKEN;
		outputtoken.pvBuffer=outbuf;

		SecBufferDesc     outputtokendesc;
		outputtokendesc.ulVersion=SECBUFFER_VERSION;
		outputtokendesc.cBuffers=1;
		outputtokendesc.pBuffers=&outputtoken;

		// get credentials
		// GSSAPI acquires credentials implicitly if none are specified,
		// but SSPI does not.  Fortunately, acquiring credentials for a
		// NULL user grabs the current user's credentials.
		if (!pvt->_credentials) {
			pvt->_credentials=new gsscredentials();
			pvt->_credentials->addDesiredMechanism(
						pvt->_desiredmechanism);
			pvt->_freecredentials=true;
		}
		if (!pvt->_credentials->acquired()) {
			pvt->_credentials->acquireForUser(NULL);
		} 
		CredHandle	*credentials=
			(CredHandle *)pvt->_credentials->getCredentials();

		for (;;) {

			outputtoken.cbBuffer=pvt->_maxmsgsize;

			if (inbuf) {
				inputtoken[0].cbBuffer=inbufsize;
				inputtoken[0].pvBuffer=inbuf;
			}

			// attempt to init the context
			pvt->_sstatus=InitializeSecurityContext(
						credentials,
						(inbuf)?&pvt->_context:NULL,
						(SEC_CHAR *)name,
						pvt->_desiredflags,
						0,
						SECURITY_NETWORK_DREP,
						(inbuf)?&inputtokendesc:NULL,
						0,
						&pvt->_context,
						&outputtokendesc,
						&pvt->_actualflags,
						&pvt->_actuallifetime);

			// clean up
			delete[] inbuf;

			// bail on error
			if (SSPI_ERROR(pvt->_sstatus)) {
				#ifdef DEBUG_GSS
					stdoutput.printf(
						"failed "
						"(InitializeSecurityContext)"
						"\n%s\n",
						getStatus());
				#endif
				close();
				error=true;
				break;
			}

			// complete the token, if needed
			if (pvt->_sstatus==SEC_I_COMPLETE_NEEDED ||
				pvt->_sstatus==SEC_I_COMPLETE_AND_CONTINUE) {

				pvt->_sstatus=CompleteAuthToken(
							&pvt->_context,
							&outputtokendesc);

				if (SSPI_ERROR(pvt->_sstatus)) {
					#ifdef DEBUG_GSS
						stdoutput.printf(
							"failed "
							"(CompleteAuthToken)"
							"\n%s\n",
							getStatus());
					#endif
					close();
					error=true;
					break;
				}
			}

			// send token to peer, if necessary
			if (outputtoken.cbBuffer) {
				if (sendToken(TOKEN_FLAGS_TYPE_INITIATE,
						outbuf,
						outputtoken.cbBuffer)!=
						(ssize_t)outputtoken.cbBuffer) {
					#ifdef DEBUG_GSS
						stdoutput.write(
							"failed (send)\n\n");
					#endif
					close();
					error=true;
					break;
				}
			}

			// receive token from peer,
			// into inputtoken, if necessary
			if (pvt->_sstatus==SEC_I_CONTINUE_NEEDED ||
				pvt->_sstatus==SEC_I_COMPLETE_AND_CONTINUE) {

				void	*inbuftemp=NULL;
				size_t	inbufsizetemp=0;
				uint32_t	flags=0;
				ssize_t		result=
					receiveToken(&flags,
							&inbuftemp,
							&inbufsizetemp);
				inbuf=(BYTE *)inbuftemp;
				inbufsize=inbufsizetemp;
				if (result<=0 ||
					!checkFlags(flags,
						TOKEN_FLAGS_TYPE_ACCEPT)) {
					#ifdef DEBUG_GSS
						stdoutput.write(
							"failed (receive)\n\n");
					#endif
					delete[] inbuf;
					close();
					error=true;
					break;
				}

			} else {

				// FIXME: for Schannel (and maybe other
				// providers), it's possible for
				// InitializeSecurityContext to have returned
				// SEC_I_INCOMPLETE_CREDENTIALS.
				// Handle that...

				// break out if we've completed the process
				break;
			}
		}

		// handle "extra" data
		if (inputtoken[1].BufferType==SECBUFFER_EXTRA) {
			// FIXME: For Schannel (and maybe other providers),
			// some "extra" data may have been returned from this
			// process, usually (though not always) because a
			// renegotiation occurred.  Handle that...

			FreeContextBuffer(inputtoken[1].pvBuffer);
		}

		// actual mechanism will be populated by inquire() below...

		// clean up
		delete[] outbuf;

		// bail on error
		if (error || pvt->_sstatus!=SEC_E_OK) {
			return false;
		}

		// we are the initiator
		pvt->_isinitiator=true;
	#endif

	#ifdef DEBUG_GSS
		stdoutput.write("success\n\n");
	#endif

	// get additional info about the context
	return inquire();
}

#ifdef DEBUG_GSS
static void printFlags(uint32_t flags) {
	#ifdef GSS_C_DELEG_FLAG
		if (flags&GSS_C_DELEG_FLAG) {
			stdoutput.printf(
				"    GSS_C_DELEG_FLAG\n");
		}
	#endif
	#ifdef GSS_C_MUTUAL_FLAG
		if (flags&GSS_C_MUTUAL_FLAG) {
			stdoutput.printf(
				"    GSS_C_MUTUAL_FLAG\n");
		}
	#endif
	#ifdef GSS_C_REPLAY_FLAG
		if (flags&GSS_C_REPLAY_FLAG) {
			stdoutput.printf(
				"    GSS_C_REPLAY_FLAG\n");
		}
	#endif
	#ifdef GSS_C_SEQUENCE_FLAG
		if (flags&GSS_C_SEQUENCE_FLAG) {
			stdoutput.printf(
			"    GSS_C_SEQUENCE_FLAG\n");
		}
	#endif
	#ifdef GSS_C_CONF_FLAG
		if (flags&GSS_C_CONF_FLAG) {
			stdoutput.printf(
				"    GSS_C_CONF_FLAG\n");
		}
	#endif
	#if !defined(RUDIMENTS_HAS_SSPI)
		#ifdef GSS_C_INTEG_FLAG
			if (flags&GSS_C_INTEG_FLAG) {
				stdoutput.printf(
					"    GSS_C_INTEG_FLAG\n");
			}
		#endif
		#ifdef GSS_C_ANON_FLAG
			if (flags&GSS_C_ANON_FLAG) {
				stdoutput.printf(
					"    GSS_C_ANON_FLAG\n");
			}
		#endif
		#ifdef GSS_C_PROT_READY_FLAG
			if (flags&GSS_C_PROT_READY_FLAG) {
				stdoutput.printf(
					"    GSS_C_PROT_READY_FLAG\n");
			}
		#endif
		#ifdef GSS_C_TRANS_FLAG
			if (flags&GSS_C_TRANS_FLAG) {
				stdoutput.printf(
					"    GSS_C_TRANS_FLAG\n");
			}
		#endif
		#ifdef GSS_C_DELEG_POLICY_FLAG
			if (flags&GSS_C_DELEG_POLICY_FLAG) {
				stdoutput.printf(
					"    GSS_C_DELEG_POLICY_FLAG\n");
			}
		#endif
	#endif
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

static const char *schannels[]={
			UNISP_NAME_A,
			SSL2SP_NAME_A,
			SSL3SP_NAME_A,
			TLS1SP_NAME_A,
			PCT1SP_NAME_A,
			SCHANNEL_NAME_A,
			NULL
			};
#endif

bool gsscontext::inquire() {

	bool	retval=false;

	#ifdef DEBUG_GSS
		stdoutput.write("inquire context - ");
	#endif

	#if defined(RUDIMENTS_HAS_GSS)

		// inquire...
		gss_name_t	initiator;
		gss_name_t	acceptor;
		int		isinitiator;
		int		isopen;
		pvt->_major=gss_inquire_context(&pvt->_minor,
						pvt->_context,
						&initiator,
						&acceptor,
						NULL,
						NULL,
						NULL,
						&isinitiator,
						&isopen);
		if (pvt->_major!=GSS_S_COMPLETE) {
			#ifdef DEBUG_GSS
				stdoutput.write("failed "
					"(gss_inquire_context)\n\n");
			#endif
			// FIXME: release initiator?
			// FIXME: release acceptor?
			return false;
		}


		OM_uint32	minor;

		// expose initiator
		gss_buffer_desc	initiatorbuffer;
		pvt->_major=gss_display_name(&pvt->_minor,
						initiator,
						&initiatorbuffer,
						NULL);
		if (pvt->_major!=GSS_S_COMPLETE) {
			#ifdef DEBUG_GSS
				stdoutput.write("failed "
					"(display_name(initiator))\n\n");
			#endif
			return false;
		}

		delete[] pvt->_initiator;
		pvt->_initiator=new char[initiatorbuffer.length+1];
		charstring::copy(pvt->_initiator,
					(char *)initiatorbuffer.value,
					initiatorbuffer.length);
		pvt->_initiator[initiatorbuffer.length]='\0';
	
		gss_release_name(&minor,&initiator);
		gss_release_buffer(&minor,&initiatorbuffer);


		// expose acceptor
		gss_buffer_desc	acceptorbuffer;
		pvt->_major=gss_display_name(&pvt->_minor,
						acceptor,
						&acceptorbuffer,
						NULL);
		if (pvt->_major!=GSS_S_COMPLETE) {
			#ifdef DEBUG_GSS
				stdoutput.write("failed "
					"(display_name(acceptor))\n\n");
			#endif
			return false;
		}

		delete[] pvt->_acceptor;
		pvt->_acceptor=new char[acceptorbuffer.length+1];
		charstring::copy(pvt->_acceptor,
					(char *)acceptorbuffer.value,
					acceptorbuffer.length);
		pvt->_acceptor[acceptorbuffer.length]='\0';
	
		gss_release_name(&minor,&acceptor);
		gss_release_buffer(&minor,&acceptorbuffer);


		// expose isinitiator
		pvt->_isinitiator=isinitiator;


		// exposeisopen
		pvt->_isopen=isopen;


		// return success/failure
		retval=(pvt->_major==GSS_S_COMPLETE);

	#elif defined(RUDIMENTS_HAS_SSPI)
		
		// set initiator and acceptor
		if (pvt->_isinitiator) {

			// try to get the native names
			char	*clientname=NULL;
			char	*servername=NULL;
			SecPkgContext_NativeNames	nn;
			pvt->_sstatus=QueryContextAttributes(
						&pvt->_context,
						SECPKG_ATTR_NATIVE_NAMES,
						&nn);
			if (pvt->_sstatus!=SEC_E_UNSUPPORTED_FUNCTION) {
				if (SSPI_ERROR(pvt->_sstatus))	{
					#ifdef DEBUG_GSS
					stdoutput.write("failed "
					"(QueryContextAttributes("
					"SECPKG_ATTR_NATIVE_NAMES))\n\n");
					#endif
					return false;
				}
				clientname=charstring::duplicate(
							nn.sClientName);
				servername=charstring::duplicate(
							nn.sServerName);
				FreeContextBuffer(nn.sClientName);
				FreeContextBuffer(nn.sServerName);
			} else {
				// reset status
				pvt->_sstatus=SEC_E_OK;
			}

			// set initiator
			delete[] pvt->_initiator;
			if (clientname) {
				pvt->_initiator=clientname;
			} else if (pvt->_credentials) {
				pvt->_initiator=charstring::duplicate(
						pvt->_credentials->getName());
			} else {
				pvt->_initiator=NULL;
			}

			// set acceptor
			delete[] pvt->_acceptor;
			if (servername) {
				pvt->_acceptor=servername;
			} else {
				pvt->_acceptor=charstring::duplicate(
							pvt->_service);
			}

		} else {

			// get the user name
			char	*username=NULL;
			SecPkgContext_Names	n;
			pvt->_sstatus=QueryContextAttributes(
						&pvt->_context,
						SECPKG_ATTR_NAMES,
						&n);
			if (pvt->_sstatus!=SEC_E_UNSUPPORTED_FUNCTION) {
				if (SSPI_ERROR(pvt->_sstatus))	{
					#ifdef DEBUG_GSS
					stdoutput.write("failed "
					"(QueryContextAttributes("
					"SECPKG_ATTR_NAMES))\n\n");
					#endif
					return false;
				}
				username=charstring::duplicate(n.sUserName);
			} else {
				// FIXME: what to do???
				// reset status
				pvt->_sstatus=SEC_E_OK;
			}

			// try to get the target
			char	*target=NULL;
			#ifdef SECPKG_ATTR_CLIENT_SPECIFIED_TARGET
			SecPkgContext_ClientSpecifiedTarget	csp;
			pvt->_sstatus=QueryContextAttributes(
					&pvt->_context,
					SECPKG_ATTR_CLIENT_SPECIFIED_TARGET,
					&csp);
			if (pvt->_sstatus!=SEC_E_UNSUPPORTED_FUNCTION &&
				pvt->_sstatus!=SEC_E_TARGET_UNKNOWN) {
				if (SSPI_ERROR(pvt->_sstatus))	{
					#ifdef DEBUG_GSS
					stdoutput.write("failed "
					"(QueryContextAttributes("
					"SECPKG_ATTR_CLIENT_"
					"SPECIFIED_TARGET))\n\n");
					#endif
					return false;
				}
				target=unicodeToAscii((WCHAR *)csp.sTargetName);
			} else {
				// FIXME: what to do???
				// reset status
				pvt->_sstatus=SEC_E_OK;
			}
			#else
				// FIXME: what to do???
			#endif

			// set initiator
			delete[] pvt->_initiator;
			pvt->_initiator=username;

			// set acceptor
			delete[] pvt->_acceptor;
			if (target) {
				pvt->_acceptor=target;
			} else if (pvt->_credentials) {
				pvt->_acceptor=charstring::duplicate(
						pvt->_credentials->getName());
			} else {
				pvt->_acceptor=NULL;
			}
		}


		// get actual mechanism...
		// If we specified the Negotiate mechanism, then we need to get
		// the actual mechanism that was eventually agreed upon.  We'll
		// also get whether the process was complete or not and use that
		// for our "isopen" status.
		// If we specified a mechanism other than Negotiate then the
		// query will fail.  To handle that case, we'll default to
		// the requested mech and isopen=true.
		const char	*mech=(pvt->_desiredmechanism)?
					pvt->_desiredmechanism->getString():
					"Negotiate";
		bool		isopen=true;
		SecPkgContext_NegotiationInfo	neginfo;
		pvt->_sstatus=QueryContextAttributes(
					&pvt->_context,
					SECPKG_ATTR_NEGOTIATION_INFO,
					&neginfo);
		if (pvt->_sstatus!=SEC_E_UNSUPPORTED_FUNCTION) {
			if (SSPI_ERROR(pvt->_sstatus))	{
				#ifdef DEBUG_GSS
					stdoutput.write("failed "
					"(QueryContextAttributes("
					"SECPKG_ATTR_NEGOTIATION_INFO))\n\n");
				#endif
				return false;
			}
			mech=neginfo.PackageInfo->Name;
			isopen=(neginfo.NegotiationState==
					SECPKG_NEGOTIATION_COMPLETE);
		}

		// set actual mechanism
		pvt->_actualmechanism.initialize(mech);

		// are we using kerberos or schannel?
		if (!charstring::compareIgnoringCase(mech,"Kerberos")) {
			pvt->_mech=GSS_MECH_KRB;
		} else if (charstring::inSetIgnoringCase(mech,schannels)) {
			pvt->_mech=GSS_MECH_SCHANNEL;
		}

		// set isopen
		pvt->_isopen=isopen;

		// clean up
		if (pvt->_sstatus==SEC_E_UNSUPPORTED_FUNCTION) {
			// reset status
			pvt->_sstatus=SEC_E_OK;
		} else {
			FreeContextBuffer(neginfo.PackageInfo);
		}


		// get trailer and block sizes
		SecPkgContext_Sizes	sizes;
		pvt->_sstatus=QueryContextAttributes(
					&pvt->_context,
					SECPKG_ATTR_SIZES,
					&sizes);
		if (SSPI_ERROR(pvt->_sstatus)) {
			#ifdef DEBUG_GSS
				stdoutput.write("failed "
				"(QueryContextAttributes("
				"SECPKG_ATTR_SIZES))\n\n");
			#endif
			return false;
		}
		pvt->_trailersize=sizes.cbSecurityTrailer;
		pvt->_blksize=sizes.cbBlockSize;

		// get stream header and trailer sizes (Schannel only)
		if (pvt->_mech==GSS_MECH_SCHANNEL) {
			SecPkgContext_StreamSizes	streamsizes;
			pvt->_sstatus=QueryContextAttributes(
						&pvt->_context,
						SECPKG_ATTR_STREAM_SIZES,
						&streamsizes);
			if (SSPI_ERROR(pvt->_sstatus)) {
				#ifdef DEBUG_GSS
					stdoutput.write("failed "
					"(QueryContextAttributes("
					"SECPKG_ATTR_STREAM_SIZES))\n\n");
				#endif
				return false;
			}
			pvt->_streamheadersize=streamsizes.cbHeader;
			pvt->_streamtrailersize=streamsizes.cbTrailer;
		}

		retval=true;

	#endif

	#ifdef DEBUG_GSS
		stdoutput.write("success...\n\n");
		stdoutput.write("Context {\n");
		stdoutput.printf("  file descriptor: %d\n",
			(pvt->_fd)?pvt->_fd->getFileDescriptor():-1);
		#if !defined(RUDIMENTS_HAS_SSPI)
			stdoutput.printf("  desired lifetime: %d\n",
						pvt->_desiredlifetime);
			stdoutput.printf("  actual lifetime: %d\n",
						pvt->_actuallifetime);
		#endif
		stdoutput.printf("  desired mechanism: %s\n",
			(pvt->_desiredmechanism &&
				pvt->_desiredmechanism->getString())?
			pvt->_desiredmechanism->getString():"(none)");
		stdoutput.printf("  actual mechanism: %s\n",
			pvt->_actualmechanism.getString());
		stdoutput.write("  desired flags:\n");
		printFlags(pvt->_desiredflags);
		stdoutput.write("  actual flags:\n");
		printFlags(pvt->_actualflags);
		stdoutput.printf("  service: %s\n",
			(pvt->_service)?pvt->_service:"(none)");
		stdoutput.printf("  initiator: %s\n",
			(pvt->_initiator)?pvt->_initiator:"(none)");
		stdoutput.printf("  acceptor: %s\n",
			(pvt->_acceptor)?pvt->_acceptor:"(none)");
		stdoutput.printf("  is initiator: %s\n",
			(pvt->_isinitiator)?"yes":"no");
		stdoutput.printf("  is open: %s\n",
			(pvt->_isopen)?"yes":"no");
		stdoutput.write("}\n");
	#endif

	return retval;
}

bool gsscontext::accept() {

	// release any previously accepted context
	close();

	#ifdef DEBUG_GSS
		stdoutput.write("accept context...\n");
	#endif

	#if defined(RUDIMENTS_HAS_GSS)

		bool	error=false;

		OM_uint32	minor;

		// accept the context...
		gss_buffer_desc	inputtoken;
		gss_buffer_desc	outputtoken;

		gss_cred_id_t	credentials=
			(pvt->_credentials)?
			(gss_cred_id_t)pvt->_credentials->getCredentials():
			GSS_C_NO_CREDENTIAL;

		gss_OID	actualmechoid=GSS_C_NO_OID;

		do {

			// receive token from peer, into inputtoken...
			uint32_t	flags=0;
			inputtoken.value=NULL;
			inputtoken.length=0;
			if (receiveToken(&flags,
					&inputtoken.value,
					&inputtoken.length)<=0 ||
				!checkFlags(flags,TOKEN_FLAGS_TYPE_INITIATE)) {
				#ifdef DEBUG_GSS
					stdoutput.write("failed (receive)\n\n");
				#endif
				delete[] (unsigned char *)inputtoken.value;
				close();
				error=true;
				break;
			}

			// attempt to accept the context
			pvt->_major=gss_accept_sec_context(
						&pvt->_minor,
						&pvt->_context,
						credentials,
						&inputtoken,
						GSS_C_NO_CHANNEL_BINDINGS,
						NULL,
						&actualmechoid,
						&outputtoken,
						&pvt->_actualflags,
						&pvt->_actuallifetime,
						NULL);

			// clean up
			delete[] (unsigned char *)inputtoken.value;

			// bail on error
			if (GSS_ERROR(pvt->_major)) {
				#ifdef DEBUG_GSS
					stdoutput.printf(
						"failed "
						"(accept_sec_context) %d\n%s\n",
						pvt->_major,getStatus());
				#endif
				close();
				error=true;
				break;
			}

			// send token to peer, if necessary
			if (outputtoken.length) {
				if (sendToken(TOKEN_FLAGS_TYPE_ACCEPT,
						outputtoken.value,
						outputtoken.length)!=
						(ssize_t)outputtoken.length) {
					gss_release_buffer(&minor,&outputtoken);
					close();
					error=true;
					break;
				}
			}

			// clean up
			gss_release_buffer(&minor,&outputtoken);

		} while (pvt->_major==GSS_S_CONTINUE_NEEDED);

		// bail on error
		if (error || pvt->_major!=GSS_S_COMPLETE) {
			return false;
		}

		// populate actual mechanism
		pvt->_actualmechanism.initialize(actualmechoid);

	#elif defined(RUDIMENTS_HAS_SSPI)

		bool	error=false;

		// get maxiumum message size...
		// At this point, we don't definitively know what mech
		// (security package) we'll be using, so we'll get the
		// max message size from the Negotiate mech, which will
		// be the max message size for all mechs.
		if (!getMaxMessageSize("Negotiate")) {
			return false;
		}

		// input buffer
		SecBuffer	inputtoken;
		inputtoken.BufferType=SECBUFFER_TOKEN;

		SecBufferDesc	inputtokendesc;
		inputtokendesc.ulVersion=SECBUFFER_VERSION;
		inputtokendesc.cBuffers=1;
		inputtokendesc.pBuffers=&inputtoken;

		// output buffer
		BYTE	*outbuf=new BYTE[pvt->_maxmsgsize];

		SecBuffer	outputtoken;
		outputtoken.BufferType=SECBUFFER_TOKEN;
		outputtoken.pvBuffer=outbuf;

		SecBufferDesc	outputtokendesc;
		outputtokendesc.ulVersion=SECBUFFER_VERSION;
		outputtokendesc.cBuffers=1;
		outputtokendesc.pBuffers=&outputtoken;

		// get credentials
		CredHandle	*credentials=
			(pvt->_credentials)?
			(CredHandle *)(pvt->_credentials->getCredentials()):
			NULL;

		bool	first=true;
		do {

			// receive token from peer, into inputtoken...
			uint32_t	flags=0;
			void		*inbuf=NULL;
			size_t		inbufsize=0;
			ssize_t		result=receiveToken(&flags,
								&inbuf,
								&inbufsize);
			if (result<=0 ||
				!checkFlags(flags,TOKEN_FLAGS_TYPE_INITIATE)) {
				#ifdef DEBUG_GSS
					stdoutput.write("failed (receive)\n\n");
				#endif
				delete[] inbuf;
				close();
				error=true;
				break;
			}
			inputtoken.pvBuffer=inbuf;
			inputtoken.cbBuffer=inbufsize;

			// reset output buffer sizes
			outputtoken.cbBuffer=pvt->_maxmsgsize;

			// attempt to accept the context
			pvt->_sstatus=AcceptSecurityContext(
						credentials,
						(first)?NULL:&pvt->_context,
						&inputtokendesc,
						pvt->_desiredflags,
						SECURITY_NETWORK_DREP,
						&pvt->_context,
						&outputtokendesc,
						&pvt->_actualflags,
						&pvt->_actuallifetime);

			// clean up
			delete[] inbuf;

			// bail on error
			if (SSPI_ERROR(pvt->_sstatus) &&
				pvt->_sstatus!=SEC_E_INCOMPLETE_MESSAGE) {
				#ifdef DEBUG_GSS
					stdoutput.printf(
						"failed "
						"(AcceptSecurityContext)"
						"\n%s\n",
						getStatus());
				#endif
				close();
				error=true;
				break;
			}

			// complete the token, if needed
			if (pvt->_sstatus==SEC_I_COMPLETE_NEEDED ||
				pvt->_sstatus==SEC_I_COMPLETE_AND_CONTINUE) {
				pvt->_sstatus=CompleteAuthToken(
							&pvt->_context,
							&outputtokendesc);
				if (SSPI_ERROR(pvt->_sstatus)) {
					#ifdef DEBUG_GSS
						stdoutput.printf(
							"failed "
							"(CompleteAuthToken)"
							"\n%s\n",
							getStatus());
					#endif
					close();
					error=true;
					break;
				}
			}

			first=false;

			// send token to peer, if necessary
			if (outputtoken.cbBuffer &&
				pvt->_sstatus!=SEC_E_INCOMPLETE_MESSAGE) {
				if (sendToken(TOKEN_FLAGS_TYPE_ACCEPT,
						outbuf,
						outputtoken.cbBuffer)!=
						(ssize_t)outputtoken.cbBuffer) {
					#ifdef DEBUG_GSS
						stdoutput.write(
							"failed (send)\n\n");
					#endif
					close();
					error=true;
					break;
				}
			}

		} while (pvt->_sstatus==SEC_I_CONTINUE_NEEDED ||
				pvt->_sstatus==SEC_I_COMPLETE_AND_CONTINUE ||
				pvt->_sstatus==SEC_E_INCOMPLETE_MESSAGE);

		delete[] outbuf;

		// actual mechanism will be populated by inquire() below...

		// bail on error
		if (error || pvt->_sstatus!=SEC_E_OK) {
			return false;
		}
	#else
		#ifdef DEBUG_GSS
			stdoutput.write("failed (not supported)\n\n");
		#endif
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif

	#ifdef DEBUG_GSS
		stdoutput.printf("success\n\n");
	#endif

	// get additional info about the context
	return inquire();
}

bool gsscontext::close() {

	#if defined(RUDIMENTS_HAS_GSS)
		// delete the context
		if (pvt->_context!=GSS_C_NO_CONTEXT) {
			gss_delete_sec_context(
				&pvt->_minor,&pvt->_context,GSS_C_NO_BUFFER);
			// automatically sets pvt->_context to GSS_C_NO_CONTEXT
		}
	#elif defined(RUDIMENTS_HAS_SSPI)
		DeleteSecurityContext(&pvt->_context);
		bytestring::zero(&pvt->_context,sizeof(pvt->_context));
		if (pvt->_freecredentials) {
			delete pvt->_credentials;
			pvt->_credentials=NULL;
		}
		pvt->_freecredentials=false;
	#endif

	// reset initiator and acceptor names
	delete[] pvt->_initiator;
	pvt->_initiator=NULL;

	delete[] pvt->_acceptor;
	pvt->_acceptor=NULL;

	// reset the "actuals"
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_actuallifetime=0;
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_maxmsgsize=0;
		pvt->_streamheadersize=0;
		pvt->_streamtrailersize=0;
		pvt->_trailersize=0;
		pvt->_blksize=0;
		pvt->_mech=GSS_MECH_OTHER;
		pvt->_actuallifetime.u.LowPart=0;
		pvt->_actuallifetime.u.HighPart=0;
	#else
		pvt->_actuallifetime=0;
	#endif
	pvt->_actualmechanism.clear();
	pvt->_actualflags=0;

	// reset states
	pvt->_isinitiator=false;
	pvt->_isopen=false;
	pvt->_readbuffer.clear();
	pvt->_readbufferpos=0;

	return true;
}

ssize_t gsscontext::getSizeMax() {
	// FIXME: there's probably an intelligent
	// way of deciding what this should be
	return 65536;
}

uint32_t gsscontext::getActualLifetime() {
	#if defined(RUDIMENTS_HAS_GSS)
		return pvt->_actuallifetime;
	#elif defined(RUDIMENTS_HAS_SSPI)
		// this loses precision...
		return (uint32_t)pvt->_actuallifetime.QuadPart;
	#else
		return pvt->_actuallifetime;
	#endif
}

gssmechanism *gsscontext::getActualMechanism() {
	return &pvt->_actualmechanism;
}

uint32_t gsscontext::getActualFlags() {
	return pvt->_actualflags;
}

uint32_t gsscontext::getRemainingLifetime() {
	#if defined(RUDIMENTS_HAS_GSS)
		OM_uint32	remainingtime;
		pvt->_major=gss_context_time(&pvt->_minor,
						pvt->_context,
						&remainingtime);
		return (pvt->_major==GSS_S_COMPLETE)?remainingtime:0;
	#elif defined(RUDIMENTS_HAS_SSPI)
		SecPkgContext_Lifespan	l;
		pvt->_sstatus=QueryContextAttributes(
						&pvt->_context,
						SECPKG_ATTR_LIFESPAN,
						&l);
		if (SSPI_ERROR(pvt->_sstatus))	{
			#ifdef DEBUG_GSS
			stdoutput.write("failed "
				"(QueryContextAttributes("
				"SECPKG_ATTR_LIFESPAN))\n\n");
			#endif
			return 0;
		}
		// FIXME: get now and subtract from l.tsExpiry.QuadPart
		return 0;
	#else
		return 0;
	#endif
}

const char *gsscontext::getInitiator() {
	return pvt->_initiator;
}

const char *gsscontext::getAcceptor() {
	return pvt->_acceptor;
}

bool gsscontext::getIsInitiator() {
	return pvt->_isinitiator;
}

bool gsscontext::getIsOpen() {
	return pvt->_isopen;
}

bool gsscontext::wrap(const unsigned char *input,
					size_t inputsize,
					unsigned char **output,
					size_t *outputsize) {
	return wrap(input,inputsize,true,output,outputsize,NULL);
}

bool gsscontext::wrap(const unsigned char *input,
					size_t inputsize,
					bool useencryption,
					unsigned char **output,
					size_t *outputsize,
					bool *encryptionused) {

	#ifdef DEBUG_GSS_WRAP
		stdoutput.printf("wrap(%d,",inputsize);
		stdoutput.safePrint(input,inputsize);
		stdoutput.write(")\n");
	#endif

	#if defined(RUDIMENTS_HAS_GSS)

		// configure input buffer
		gss_buffer_desc	inputbuffer;
		inputbuffer.value=(void *)input;
		inputbuffer.length=inputsize;

		// declare output buffer
		gss_buffer_desc	outputbuffer;

		// use encryption or not
		int	useenc=useencryption;
		int	encused=0;

		// wrap the input
		pvt->_major=gss_wrap(&pvt->_minor,
					pvt->_context,
					useenc,
					GSS_C_QOP_DEFAULT,
					&inputbuffer,
					&encused,
					&outputbuffer);

		// whether encryption was used or not
		if (encryptionused) {
			*encryptionused=encused;
		}

		if (pvt->_major==GSS_S_COMPLETE) {

			// copy-out the wrapped 
			if (output) {
				*output=(unsigned char *)
					bytestring::duplicate(
						outputbuffer.value,
						outputbuffer.length);
			}
			if (outputsize) {
				*outputsize=outputbuffer.length;
			}

			// clean up
			OM_uint32	minor;
			gss_release_buffer(&minor,&outputbuffer);

			#ifdef DEBUG_GSS_WRAP
				stdoutput.write(" success\n\n");
			#endif

			return true;
		}

		// FIXME: are there cases where outputbuffer
		// should be cleaned up here too?

		#ifdef DEBUG_GSS_WRAP
			stdoutput.printf(" failed\n%s\n\n",getStatus());
		#endif

	#elif defined(RUDIMENTS_HAS_SSPI)

		// EncryptMessage encrypts a set of buffers.  Exactly what
		// buffers and how the encrypted data is formatted in them
		// depends on the encryption package...

		if (pvt->_mech==GSS_MECH_KRB) {

			// For kerberos, "trailer", "data", and "padding"
			// buffers are required, in that order.

			// allocate a temp buffer, large enough to hold the
			// input, trailer, and padding
			size_t	tempsize=pvt->_trailersize+
						inputsize+
						pvt->_blksize;
			BYTE	*temp=new BYTE[tempsize];
			bytestring::zero(temp,tempsize);

			// initialize the temp buffer with the input
			bytestring::copy(temp+pvt->_trailersize,
						input,inputsize);

			// prepare security buffers
			SecBuffer         secbuf[3];
			secbuf[0].BufferType=SECBUFFER_TOKEN;
			secbuf[0].cbBuffer=pvt->_trailersize;
			secbuf[0].pvBuffer=temp;

			secbuf[1].BufferType=SECBUFFER_DATA;
			// FIXME: this can't be larger than pvt->_maxmsgsize
			secbuf[1].cbBuffer=inputsize;
			secbuf[1].pvBuffer=temp+pvt->_trailersize;

			secbuf[2].BufferType=SECBUFFER_PADDING;
			secbuf[2].cbBuffer=pvt->_blksize;
			secbuf[2].pvBuffer=temp+pvt->_trailersize+inputsize;

			SecBufferDesc     secbufdesc;
			secbufdesc.ulVersion=SECBUFFER_VERSION;
			secbufdesc.cBuffers=3;
			secbufdesc.pBuffers=secbuf;

			// encrypt the input
			pvt->_sstatus=EncryptMessage(
					&pvt->_context,0,&secbufdesc,0);

			// copy out the parts of the buffers
			// that EncryptMessage ended up using
			*outputsize=secbuf[0].cbBuffer+
					secbuf[1].cbBuffer+
					secbuf[2].cbBuffer;
			*output=new BYTE[*outputsize];
			bytestring::copy(*output,secbuf[0].pvBuffer,
						secbuf[0].cbBuffer);
			bytestring::copy(*output+secbuf[0].cbBuffer,
						secbuf[1].pvBuffer,
						secbuf[1].cbBuffer);
			bytestring::copy(*output+secbuf[0].cbBuffer+
						secbuf[1].cbBuffer,
						secbuf[2].pvBuffer,
						secbuf[2].cbBuffer);

		} else if (pvt->_mech==GSS_MECH_SCHANNEL) {

			// For schannel, "stream header", "stream trailer",
			// and "empty" buffers are required, in that order.

			// allocate the output buffer, large enough to hold the
			// input, trailer, and padding
			*outputsize=pvt->_streamheadersize+
					inputsize+pvt->_streamtrailersize;
			*output=new BYTE[*outputsize];

			// initialize the output buffer with the input
			bytestring::copy(*output+pvt->_streamheadersize,
							input,inputsize);

			// prepare security buffers
			SecBuffer         secbuf[4];
			secbuf[0].BufferType=SECBUFFER_STREAM_HEADER;
			secbuf[0].cbBuffer=pvt->_streamheadersize;
			secbuf[0].pvBuffer=*output;

			secbuf[1].BufferType=SECBUFFER_DATA;
			// FIXME: this can't be larger than pvt->_maxmsgsize
			secbuf[1].cbBuffer=inputsize;
			secbuf[1].pvBuffer=*output+pvt->_streamheadersize;

			secbuf[2].BufferType=SECBUFFER_STREAM_TRAILER;
			secbuf[2].cbBuffer=pvt->_trailersize;
			secbuf[2].pvBuffer=*output+
						pvt->_streamheadersize+
						inputsize;

			secbuf[3].BufferType=SECBUFFER_EMPTY;
			secbuf[3].cbBuffer=0;
			secbuf[3].pvBuffer=NULL;

			SecBufferDesc     secbufdesc;
			secbufdesc.ulVersion=SECBUFFER_VERSION;
			secbufdesc.cBuffers=4;
			secbufdesc.pBuffers=secbuf;

			// encrypt the input
			pvt->_sstatus=EncryptMessage(
					&pvt->_context,0,&secbufdesc,0);

		} else {

			// In the general case, a "data" and "trailer" buffer
			// are required and the data buffer must hold the
			// unencrypted data. This data will be overwritten and
			// any overflow will be placed in the trailer.

			// allocate a buffer to hold the input size,
			// input, trailer size, and trailer
			*outputsize=sizeof(DWORD)+inputsize+
						sizeof(DWORD)+pvt->_trailersize;
			*output=new BYTE[*outputsize];

			// store that data, in that order
			bytestring::copy(*output,&inputsize,sizeof(DWORD));
			bytestring::copy(*output+sizeof(DWORD),input,inputsize);
			bytestring::copy(*output+sizeof(DWORD)+inputsize,
					&pvt->_trailersize,sizeof(DWORD));

			// prepare security buffers
			SecBuffer         secbuf[2];
			secbuf[0].BufferType=SECBUFFER_DATA;
			// FIXME: this can't be larger than pvt->_maxmsgsize
			secbuf[0].cbBuffer=inputsize;
			secbuf[0].pvBuffer=*output+sizeof(DWORD);

			secbuf[1].BufferType=SECBUFFER_TOKEN;
			secbuf[1].cbBuffer=pvt->_trailersize;
			secbuf[1].pvBuffer=*output+sizeof(DWORD)+
						inputsize+sizeof(DWORD);

			SecBufferDesc     secbufdesc;
			secbufdesc.ulVersion=SECBUFFER_VERSION;
			secbufdesc.cBuffers=2;
			secbufdesc.pBuffers=secbuf;

			// encrypt the input
			pvt->_sstatus=EncryptMessage(
					&pvt->_context,0,&secbufdesc,0);
		}

		if (!SSPI_ERROR(pvt->_sstatus)) {
			#ifdef DEBUG_GSS_WRAP
				stdoutput.write(" success\n\n");
			#endif
			return true;
		}

		#ifdef DEBUG_GSS_WRAP
			stdoutput.printf(" failed\n%s\n\n",getStatus());
		#endif
	#else
		#ifdef DEBUG_GSS_WRAP
			stdoutput.printf(" failed (not supported)\n\n");
		#endif
		RUDIMENTS_SET_ENOSYS
	#endif

	return false;
}

bool gsscontext::unwrap(const unsigned char *input,
					size_t inputsize,
					unsigned char **output,
					size_t *outputsize) {
	return unwrap(input,inputsize,output,outputsize,NULL);
}

bool gsscontext::unwrap(const unsigned char *input,
					size_t inputsize,
					unsigned char **output,
					size_t *outputsize,
					bool *decryptionused) {

	bool	retval=false;

	#if defined(RUDIMENTS_HAS_GSS)

		// configure input buffer
		gss_buffer_desc	inputbuffer;
		inputbuffer.value=(void *)input;
		inputbuffer.length=inputsize;

		// declare output buffer
		gss_buffer_desc	outputbuffer;

		// decryption used or not
		int	decused=0;

		// unwrap the input
		pvt->_major=gss_unwrap(&pvt->_minor,
					pvt->_context,
					&inputbuffer,
					&outputbuffer,
					&decused,
					NULL);

		// whether decryption was used or not
		if (decryptionused) {
			*decryptionused=decused;
		}

		if (pvt->_major==GSS_S_COMPLETE) {

			// copy-out the unwrapped 
			if (output) {
				*output=(unsigned char *)
					bytestring::duplicate(
						outputbuffer.value,
						outputbuffer.length);
			}
			if (outputsize) {
				*outputsize=outputbuffer.length;
			}

			// clean up
			OM_uint32	minor;
			gss_release_buffer(&minor,&outputbuffer);
			retval=true;
		}

		// FIXME: are there cases where outputbuffer
		// should be cleaned up here too?

		#ifdef DEBUG_GSS_WRAP
			if (retval) {
				stdoutput.printf("unwrap(%d,",*outputsize);
				stdoutput.safePrint(*output,*outputsize);
				stdoutput.write(") success\n\n");
			} else {
				stdoutput.printf("unwrap() failed\n%s\n\n",
								getStatus());
			}
		#endif

	#elif defined(RUDIMENTS_HAS_SSPI)

		ULONG	qop=0;

		// DecryptMessage decrypts a set of buffers.  Exactly what
		// buffers and how the encrypted data is formatted in them
		// depends on the encryption package...

		if (pvt->_mech==GSS_MECH_KRB) {

			// For kerberos, "stream" and "data" buffers are
			// required, in that order.  DecryptMessage will
			// allocate space to store the decrypted data and
			// update the "data" buffer to point to it and 
			// store its size.

			// prepare buffers
			SecBuffer         secbuf[2];
			secbuf[0].BufferType=SECBUFFER_STREAM;
			secbuf[0].cbBuffer=inputsize;
			secbuf[0].pvBuffer=(void *)input;

			secbuf[1].BufferType=SECBUFFER_DATA;
			secbuf[1].cbBuffer=0;
			secbuf[1].pvBuffer=NULL;

			SecBufferDesc     secbufdesc;
			secbufdesc.ulVersion=SECBUFFER_VERSION;
			secbufdesc.cBuffers=2;
			secbufdesc.pBuffers=secbuf;

			// decrypt the input
			pvt->_sstatus=DecryptMessage(
					&pvt->_context,&secbufdesc,0,&qop);

			// copy out the decrypted data
			*outputsize=secbuf[1].cbBuffer;
			*output=(unsigned char *)bytestring::duplicate(
							secbuf[1].pvBuffer,
							*outputsize);
			// FIXME: FreeContextBuffer(secbuf[1].pvBuffer) ???

		} else if (pvt->_mech==GSS_MECH_SCHANNEL) {

			// For schannel, 1 "data" buffer and 3 "empty" buffers
			// are required.

			// prepare buffers
			SecBuffer         secbuf[4];
			secbuf[0].BufferType=SECBUFFER_DATA;
			secbuf[0].cbBuffer=inputsize;
			secbuf[0].pvBuffer=(void *)input;
			secbuf[1].BufferType=SECBUFFER_EMPTY;
			secbuf[2].BufferType=SECBUFFER_EMPTY;
			secbuf[3].BufferType=SECBUFFER_EMPTY;

			SecBufferDesc     secbufdesc;
			secbufdesc.ulVersion=SECBUFFER_VERSION;
			secbufdesc.cBuffers=4;
			secbufdesc.pBuffers=secbuf;

			// decrypt the input
			pvt->_sstatus=DecryptMessage(
					&pvt->_context,&secbufdesc,0,&qop);

			// One of the buffers will contain the decrypted
			// data.  Another may contain "extra" data.
			SecBuffer         *extrabuf=NULL;
			for (uint8_t i=0; i<4; i++) {
				if (secbuf[i].BufferType==
							SECBUFFER_DATA) {
					// copy out the decrypted data
					*outputsize=secbuf[i].cbBuffer;
					*output=(unsigned char *)
						bytestring::duplicate(
							secbuf[i].pvBuffer,
							*outputsize);
				} else if (secbuf[i].BufferType==
							SECBUFFER_EXTRA) {
					extrabuf=secbuf;
				}
			}

			// FIXME: The peer might want to renegotiate.
			// That extrabuf is necessary to do that...
			if (pvt->_sstatus==SEC_I_RENEGOTIATE) {
				stdoutput.printf("FIXME: peer wants "
							"to renegotiate\n");
			}

		} else {

			// In the general case, "data" and "trailer" buffers
			// were required to store the encrypted data.
			// DecryptMessage needs to know the location of each
			// and will ultimately store the decrypted data in the
			// "data" buffer.

			// get output size, output, trailer size, and trailer
			bytestring::copy(outputsize,input,sizeof(DWORD));

			// create a buffer to store the output and initialize
			// it with the contents of the "data" buffer
			*output=(unsigned char *)bytestring::duplicate(
							(input+sizeof(DWORD)),
							*outputsize);

			// get the trailer size and location of the trailer
			DWORD	trailersize;
			bytestring::copy(&trailersize,
				(DWORD *)(input+sizeof(DWORD)+*outputsize),
				sizeof(DWORD));
			BYTE	*trailer=(BYTE *)
				(input+sizeof(DWORD)+*outputsize+sizeof(DWORD));

			// prepare buffers
			SecBuffer         secbuf[2];
			secbuf[0].BufferType=SECBUFFER_DATA;
			secbuf[0].cbBuffer=*outputsize;
			secbuf[0].pvBuffer=*output;

			secbuf[1].BufferType=SECBUFFER_TOKEN;
			secbuf[1].cbBuffer=trailersize;
			secbuf[1].pvBuffer=trailer;

			SecBufferDesc     secbufdesc;
			secbufdesc.ulVersion=SECBUFFER_VERSION;
			secbufdesc.cBuffers=2;
			secbufdesc.pBuffers=secbuf;

			// decrypt the input
			pvt->_sstatus=DecryptMessage(
					&pvt->_context,&secbufdesc,0,&qop);
		}

		retval=!SSPI_ERROR(pvt->_sstatus);

		#ifdef DEBUG_GSS_WRAP
			if (retval) {
				stdoutput.printf("unwrap(%d,",*outputsize);
				stdoutput.safePrint(*output,*outputsize);
				stdoutput.write(") success\n\n");
			} else {
				stdoutput.printf("unwrap() failed\n%s\n\n",
								getStatus());
			}
		#endif
	#else
		RUDIMENTS_SET_ENOSYS
		#ifdef DEBUG_GSS
			stdoutput.write("failed (not supported)\n\n");
		#endif
		RUDIMENTS_SET_ENOSYS
	#endif

	return retval;
}

bool gsscontext::getMic(const unsigned char *message,
					size_t messagesize,
					unsigned char **mic,
					size_t *micsize) {

	#if defined(RUDIMENTS_HAS_GSS)

		// configure message buffer
		gss_buffer_desc	messagebuffer;
		messagebuffer.value=(void *)message;
		messagebuffer.length=messagesize;

		// declare mic buffer
		gss_buffer_desc	micbuffer;

		// get the mic
		pvt->_major=gss_get_mic(&pvt->_minor,
					pvt->_context,
					GSS_C_QOP_DEFAULT,
					&messagebuffer,
					&micbuffer);

		if (pvt->_major==GSS_S_COMPLETE) {

			// copy-out the mic
			if (mic) {
				*mic=(unsigned char *)
					bytestring::duplicate(
						micbuffer.value,
						micbuffer.length);
			}
			if (micsize) {
				*micsize=micbuffer.length;
			}

			// clean up
			OM_uint32	minor;
			gss_release_buffer(&minor,&micbuffer);

			return true;
		}

		// FIXME: are there cases where micbuffer
		// should be cleaned up here too?
	#elif defined(RUDIMENTS_HAS_SSPI)
		// FIXME: implement this using MakeSignature
		RUDIMENTS_SET_ENOSYS
	#else
		RUDIMENTS_SET_ENOSYS
	#endif

	return false;
}

bool gsscontext::verifyMic(const unsigned char *message,
					size_t messagesize,
					const unsigned char *mic,
					size_t micsize) {

	#if defined(RUDIMENTS_HAS_GSS)

		// configure message buffer
		gss_buffer_desc	messagebuffer;
		messagebuffer.value=(void *)message;
		messagebuffer.length=messagesize;

		// configure mic buffer
		gss_buffer_desc	micbuffer;
		micbuffer.value=(void *)mic;
		micbuffer.length=micsize;

		// verify the mic
		pvt->_major=gss_verify_mic(&pvt->_minor,
						pvt->_context,
						&messagebuffer,
						&micbuffer,
						NULL);

		// return success/failure
		return (pvt->_major==GSS_S_COMPLETE);
	#elif defined(RUDIMENTS_HAS_SSPI)
		// FIXME: implement this using VerifySignature
		RUDIMENTS_SET_ENOSYS
	#else
		RUDIMENTS_SET_ENOSYS
	#endif

	return false;
}

ssize_t gsscontext::read(void *buf, ssize_t count) {

	// first, return any buffered data
	ssize_t bytestoread=count;
	ssize_t	bytesread=0;
	size_t	pendingbytes=pending();
	if (pendingbytes) {

		// copy data out...
		if (pendingbytes<=(size_t)bytestoread) {
			bytestring::copy(
				buf,pvt->_readbuffer.getBuffer()+
					pvt->_readbufferpos,pendingbytes);
			pvt->_readbuffer.clear();
			pvt->_readbufferpos=0;
			bytesread=pendingbytes;
			buf=((unsigned char *)buf)+pendingbytes;
			bytestoread-=pendingbytes;
		} else {
			bytestring::copy(
				buf,pvt->_readbuffer.getBuffer()+
					pvt->_readbufferpos,bytestoread);
			pvt->_readbufferpos+=bytestoread;
			bytesread=bytestoread;
			buf=((unsigned char *)buf)+bytestoread;
			bytestoread=0;
		}
	}

	// next, receive a token from the peer...
	//
	// This should be a low-level read, and it should allow short reads.
	// I.e. it should't try to receive multiple tokens until it's read
	// "count" bytes.  It should just return "what's currently available",
	// and in this case that should come from whatever was already buffered,
	// and/or the next token.  The filedescriptor class's read-buffering
	// depends on this method having short-read behavior.  We'll leave it
	// up to the filedescriptor class to make multiple calls to satisfy
	// cases where short-reads aren't allowed.

	// receive token
	uint32_t	tokenflags=0;
	void		*tokendata=NULL;
	size_t		tokensize=0;
	ssize_t	result=receiveToken(&tokenflags,&tokendata,&tokensize);
	if (result<=0 || !checkFlags(tokenflags,TOKEN_FLAGS_TYPE_DATA)) {
		delete[] (unsigned char *)tokendata;
		return result;
	}

	// unwrap
	unsigned char	*data=NULL;
	size_t		datasize=0;
	if (!unwrap((unsigned char *)tokendata,tokensize,
					&data,&datasize)) {
		delete[] (unsigned char *)tokendata;
		delete[] data;
		return RESULT_ERROR;
	}
	delete[] (unsigned char *)tokendata;

	// copy data out...
	if (datasize<=(size_t)bytestoread) {
		bytestring::copy(buf,data,datasize);
		bytesread+=datasize;
	} else {
		bytestring::copy(buf,data,bytestoread);
		bytesread+=bytestoread;

		// buffer what wasn't copied out
		pvt->_readbuffer.append(data+bytesread,
					datasize-bytesread);
	}
	delete[] data;

	return bytesread;
}

ssize_t gsscontext::receiveToken(uint32_t *tokenflags,
					void **tokendata,
					size_t *tokensize) {
	switch (pvt->_tokenformat) {
		case GSSTOKENFORMAT_KRB:
			return receiveKrbToken(tokenflags,tokendata,tokensize);
		case GSSTOKENFORMAT_TLS:
			return receiveTlsToken(tokenflags,tokendata,tokensize);
		default:
			return RESULT_ERROR;
	}
}

ssize_t gsscontext::receiveKrbToken(uint32_t *tokenflags,
					void **tokendata,
					size_t *tokensize) {

	#ifdef DEBUG_GSS_RECEIVE
		stdoutput.write("receiveKrbToken(");
	#endif

	// read token flags
	ssize_t		result=fullRead(tokenflags,sizeof(*tokenflags));
	if (result!=sizeof(*tokenflags)) {
		#ifdef DEBUG_GSS_RECEIVE
			stdoutput.write(") failed (flags)\n");
		#endif
		return (result<=0)?result:RESULT_ERROR;
	}
	*tokenflags=pvt->_fd->netToHost(*tokenflags);

	#ifdef DEBUG_GSS_RECEIVE
		stdoutput.printf("%08x,",*tokenflags);
	#endif

	// read token size
	uint32_t	size;
	result=fullRead(&size,sizeof(size));
	if (result!=sizeof(size)) {
		#ifdef DEBUG_GSS_RECEIVE
			stdoutput.write(") failed (size)\n");
		#endif
		return (result<=0)?result:RESULT_ERROR;
	}
	size=pvt->_fd->netToHost(size);

	#ifdef DEBUG_GSS_RECEIVE
		stdoutput.printf("%d,",size);
	#endif

	// reject tokens that are too large
	// (use getSizeMax()*2 to allow for the expansion of the encryption)
	// FIXME: There's probably a more accurate way to decide what the
	// maximum size of the encrypted data could be.
	if (size>(uint32_t)getSizeMax()*2) {
		#ifdef DEBUG_GSS_RECEIVE
			stdoutput.write(") failed (size too large)\n");
		#endif
		#ifdef EMSGSIZE
			error::setErrorNumber(EMSGSIZE);
		#endif
		#ifdef WSAMSGSIZE
			error::setNativeErrorNumber(WSAMSGSIZE);
		#endif
		return RESULT_ERROR;
	}

	// copy size out
	*tokensize=size;

	// read data
	*tokendata=NULL;
	if (size) {
		*tokendata=new unsigned char[size];

		// read token data
		result=fullRead(*tokendata,size);
		if (result!=(ssize_t)size) {
			#ifdef DEBUG_GSS_RECEIVE
				stdoutput.write(") failed (data)\n");
			#endif
			return (result<=0)?result:RESULT_ERROR;
		}
	}
	
	#ifdef DEBUG_GSS_RECEIVE
		stdoutput.safePrint((unsigned char *)*tokendata,*tokensize);
		stdoutput.write(") success\n\n");
	#endif
	return result;
}

ssize_t gsscontext::receiveTlsToken(uint32_t *tokenflags,
					void **tokendata,
					size_t *tokensize) {

	#ifdef DEBUG_GSS_RECEIVE
		stdoutput.write("receiveTlsToken(");
	#endif

	// read header
	// header format:
	// * record type - 1 byte
	// * tls/ssl version - 2 bytes (network byte order)
	// * data size - 2 bytes (network byte order)
	unsigned char	header[5];
	ssize_t		result=fullRead(header,sizeof(header));
	if (result!=sizeof(header)) {
		#ifdef DEBUG_GSS_RECEIVE
			stdoutput.write(") failed (header)\n");
		#endif
		return (result<=0)?result:RESULT_ERROR;
	}

	// get record size
	uint16_t	size=0;
	bytestring::copy(&size,&(header[3]),sizeof(uint16_t));
	size=pvt->_fd->netToHost(size);
	#ifdef DEBUG_GSS_RECEIVE
		stdoutput.printf("%d,",size);
	#endif

	// reject records that are too large
	// (use getSizeMax()*2 to allow for the expansion of the encryption)
	// FIXME: There's probably a more accurate way to decide what the
	// maximum size of the encrypted data could be.
	if ((uint32_t)size>(uint32_t)getSizeMax()*2) {
		#ifdef DEBUG_GSS_RECEIVE
			stdoutput.write(") failed (size too large)\n");
		#endif
		#ifdef EMSGSIZE
			error::setErrorNumber(EMSGSIZE);
		#endif
		#ifdef WSAMSGSIZE
			error::setNativeErrorNumber(WSAMSGSIZE);
		#endif
		return RESULT_ERROR;
	}

	// copy out flags and size
	*tokenflags=0;
	*tokensize=size+5;

	// read data
	*tokendata=NULL;
	if (size) {
		*tokendata=new unsigned char[*tokensize];

		// include the header in the data
		bytestring::copy(*tokendata,header,sizeof(header));

		// read token data
		result=fullRead(((unsigned char *)(*tokendata))+5,size);
		if (result!=(ssize_t)size) {
			#ifdef DEBUG_GSS_RECEIVE
				stdoutput.write(") failed (data)\n");
			#endif
			return (result<=0)?result:RESULT_ERROR;
		}
	}
	
	#ifdef DEBUG_GSS_RECEIVE
		stdoutput.safePrint((unsigned char *)*tokendata,*tokensize);
		stdoutput.printf(") success (%d bytes)\n\n",*tokensize);
	#endif
	return *tokensize;
}

ssize_t gsscontext::write(const void *buf, ssize_t count) {

	// create token
	unsigned char	*tokendata=NULL;
	size_t		tokensize=0;
	if (!wrap((const unsigned char *)buf,count,&tokendata,&tokensize)) {
		delete[] tokendata;
		return RESULT_ERROR;
	}

	// send token
	ssize_t	result=sendToken(TOKEN_FLAGS_TYPE_DATA,tokendata,tokensize);
	delete[] tokendata;
	if (result!=(ssize_t)tokensize) {
		return (result<=0)?result:RESULT_ERROR;
	}
	return count;
}

ssize_t gsscontext::sendToken(uint32_t tokenflags,
					const void *tokendata,
					size_t tokensize) {
	switch (pvt->_tokenformat) {
		case GSSTOKENFORMAT_KRB:
			return sendKrbToken(tokenflags,tokendata,tokensize);
		case GSSTOKENFORMAT_TLS:
			return sendTlsToken(tokenflags,tokendata,tokensize);
		default:
			return RESULT_ERROR;
	}
}

ssize_t gsscontext::sendKrbToken(uint32_t tokenflags,
					const void *tokendata,
					size_t tokensize) {

	#ifdef DEBUG_GSS_SEND
		stdoutput.printf("sendKrbToken(%08x,%d,",tokenflags,tokensize);
		stdoutput.safePrint((const unsigned char *)tokendata,tokensize);
		stdoutput.write(") ");
	#endif

	// write token flags
	uint32_t	temptokenflags=pvt->_fd->netToHost(tokenflags);
	ssize_t		result=fullWrite(&temptokenflags,sizeof(tokenflags));
	if (result!=sizeof(tokenflags)) {
		#ifdef DEBUG_GSS_SEND
			stdoutput.write("failed (flags)\n");
		#endif
		return (result<=0)?result:RESULT_ERROR;
	}

	// write token size
	uint32_t	size=pvt->_fd->netToHost((uint32_t)tokensize);
	result=fullWrite(&size,sizeof(size));
	if (result!=sizeof(size)) {
		#ifdef DEBUG_GSS_SEND
			stdoutput.write("failed (size)\n");
		#endif
		return (result<=0)?result:RESULT_ERROR;
	}

	// write token data
	result=fullWrite(tokendata,tokensize);
	if (result!=(ssize_t)tokensize) {
		#ifdef DEBUG_GSS_SEND
			stdoutput.write("failed (data)\n");
		#endif
		return (result<=0)?result:RESULT_ERROR;
	}

	#ifdef DEBUG_GSS_SEND
		stdoutput.write("success\n\n");
	#endif
	return result;
}

ssize_t gsscontext::sendTlsToken(uint32_t tokenflags,
					const void *tokendata,
					size_t tokensize) {

	#ifdef DEBUG_GSS_SEND
		stdoutput.printf("sendTlsToken(%08x,%d,",tokenflags,tokensize);
		stdoutput.safePrint((const unsigned char *)tokendata,tokensize);
		stdoutput.write(") ");
	#endif

	// write token data
	ssize_t	result=fullWrite(tokendata,tokensize);
	if (result!=(ssize_t)tokensize) {
		#ifdef DEBUG_GSS_SEND
			stdoutput.write("failed (data)\n");
		#endif
		return (result<=0)?result:RESULT_ERROR;
	}

	#ifdef DEBUG_GSS_SEND
		stdoutput.write("success\n\n");
	#endif
	return result;
}

ssize_t gsscontext::fullRead(void *data, ssize_t size) {

	if (!pvt->_fd) {
		return RESULT_ERROR;
	}

	// read, retrying if we got a short read,
	// until we've read "size" bytes...

	ssize_t	bytesread=0;
	ssize_t bytestoread=size;
	while (bytestoread) {
		ssize_t	result=pvt->_fd->lowLevelRead(
					((unsigned char *)data)+bytesread,
					bytestoread);
		if (result<=0) {
			return result;
		}
		bytesread+=result;
		bytestoread-=result;
	}
	return bytesread;
}

ssize_t gsscontext::fullWrite(const void *data, ssize_t size) {

	if (!pvt->_fd) {
		return RESULT_ERROR;
	}

	// write, retrying if we got a short write,
	// until we've written "size" bytes...

	ssize_t	byteswritten=0;
	ssize_t bytestowrite=size;
	while (bytestowrite) {
		ssize_t	result=pvt->_fd->lowLevelWrite(
				((const unsigned char *)data)+byteswritten,
				bytestowrite);
		if (result<=0) {
			return result;
		}
		byteswritten+=result;
		bytestowrite-=result;
	}
	return byteswritten;
}

bool gsscontext::checkFlags(uint32_t actualflags, uint32_t desiredflags) {
	switch (pvt->_tokenformat) {
		case GSSTOKENFORMAT_KRB:
			return (actualflags&desiredflags);
		default:
			return true;
	}
}

ssize_t gsscontext::pending() {
	return (pvt->_readbuffer.getSize()-pvt->_readbufferpos);
}

uint32_t gsscontext::getMajorStatus() {
	#if defined(RUDIMENTS_HAS_GSS)
		return pvt->_major;
	#elif defined(RUDIMENTS_HAS_SSPI)
		return pvt->_sstatus;
	#else
		return 0;
	#endif
}

uint32_t gsscontext::getMinorStatus() {
	#if defined(RUDIMENTS_HAS_GSS)
		return pvt->_minor;
	#elif defined(RUDIMENTS_HAS_SSPI)
		return 0;
	#else
		return 0;
	#endif
}

const char *gsscontext::getStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_status.append("GSS - major:\n");
		setStatus(pvt->_major,GSS_C_GSS_CODE);
		pvt->_status.append("GSS - minor:\n");
		setStatus(pvt->_minor,GSS_C_GSS_CODE);
		pvt->_status.append("MECH - major:\n");
		setStatus(pvt->_major,GSS_C_MECH_CODE);
		pvt->_status.append("MECH - minor:\n");
		setStatus(pvt->_minor,GSS_C_MECH_CODE);
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_status.append("SSPI - status:\n");
		setStatus(pvt->_sstatus,0);
	#endif
	return pvt->_status.getString();
}

const char *gsscontext::getGSSMajorStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		setStatus(pvt->_major,GSS_C_GSS_CODE);
	#elif defined(RUDIMENTS_HAS_SSPI)
		setStatus(pvt->_sstatus,0);
	#endif
	return pvt->_status.getString();
}

const char *gsscontext::getGSSMinorStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		setStatus(pvt->_minor,GSS_C_GSS_CODE);
	#elif defined(RUDIMENTS_HAS_SSPI)
		setStatus(pvt->_sstatus,0);
	#endif
	return pvt->_status.getString();
}

const char *gsscontext::getMechanismMajorStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		setStatus(pvt->_major,GSS_C_MECH_CODE);
	#elif defined(RUDIMENTS_HAS_SSPI)
		setStatus(pvt->_sstatus,0);
	#endif
	return pvt->_status.getString();
}

const char *gsscontext::getMechanismMinorStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		setStatus(pvt->_minor,GSS_C_MECH_CODE);
	#elif defined(RUDIMENTS_HAS_SSPI)
		setStatus(pvt->_sstatus,0);
	#endif
	return pvt->_status.getString();
}

void gsscontext::setStatus(uint32_t status, int32_t type) {

	#if defined(RUDIMENTS_HAS_GSS)

		gss_buffer_desc	statusbuffer;

		OM_uint32	msgctx=0;
		do {
			OM_uint32	major;
			OM_uint32	minor;
			major=gss_display_status(&minor,
						status,
						type,
						GSS_C_NO_OID,
						&msgctx,
						&statusbuffer);
			if (major!=GSS_S_COMPLETE) {
				break;
			}

			pvt->_status.append((unsigned char *)
						statusbuffer.value,
						statusbuffer.length);
			pvt->_status.append('\n');

			gss_release_buffer(&minor,&statusbuffer);
		} while (msgctx);

	#elif defined(RUDIMENTS_HAS_SSPI)

		const char	*str=gss::getSspiStatusString(status);
		pvt->_status.append(str)->append("\n");
		if (error::getNativeErrorNumber()) {
			char	*errstr=error::getNativeErrorString();
			pvt->_status.append(errstr)->append("\n");
			delete[] errstr;
		}
	#endif
}



/*int32_t _containerutil_compare(gssmechanism *value1, gssmechanism *value2) {
	return _containerutil_compare((void *)value1,(void *)value2);
}

void _containerutil_print(gssmechanism *value) {
	_containerutil_print((void *)value);
}*/
