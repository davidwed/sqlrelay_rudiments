// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#include <rudiments/charstring.h>
#include <rudiments/character.h>
#include <rudiments/filedescriptor.h>
#include <rudiments/bytestring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/linkedlist.h>
#include <rudiments/stdio.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/gss.h>

//#define DEBUG_GSS 1
//#define DEBUG_GSS_WRAP 1
//#define DEBUG_GSS_SEND 1
//#define DEBUG_GSS_RECEIVE 1

#if defined(RUDIMENTS_HAS_GSS)

	#ifdef RUDIMENTS_HAS_GSSAPI_GSSAPI_EXT_H
		// for gss_acquire_cred_with_password and gss_str_to_oid
		#include <gssapi/gssapi_ext.h>
	#endif

	#ifndef RUDIMENTS_HAS_GSS_STR_TO_OID
		// for local gss_str_to_oid implementation
		#include "gssoid.cpp"
	#endif

	#ifndef RUDIMENTS_HAS_GSS_ACQUIRE_CRED_WITH_PASSWORD
		// for direct krb5 functions
		#if defined(RUDIMENTS_HAS_KRB5_KRB5_H)
			#include <krb5/krb5.h>
		#elif defined(RUDIMENTS_HAS_KRB5_H)
			#include <krb5.h>
		#endif
	#endif

	#ifdef RUDIMENTS_HAS_GSSAPI_GSSAPI_KRB5_H
		// for GSS_KRB5_NT_PRINCIPAL_NAME on some platforms
		#include <gssapi/gssapi_krb5.h>
	#endif

	#ifndef RUDIMENTS_HAS_GSS_NAME_TYPES
		#ifdef RUDIMENTS_HAS_GSSAPI_GSSAPI_GENERIC_H
			// for gss_nt_* identifiers
			#include <gssapi/gssapi_generic.h>
		#endif
		#define GSS_C_NT_HOSTBASED_SERVICE gss_nt_service_name
		#define GSS_C_NT_USER_NAME gss_nt_user_name
		#define GSS_KRB5_NT_PRINCIPAL_NAME gss_nt_krb5_principal
		#define GSS_C_NT_MACHINE_UID_NAME gss_nt_machine_uid_name
		#define GSS_C_NT_STRING_UID_NAME gss_nt_string_uid_name
	#endif

#elif defined(RUDIMENTS_HAS_SSPI)

	#define SSPI_ERROR(sstatus)	((sstatus)<0)

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

		// FIXME: this is overly complex, but hopefully there's
		// a programatic way of getting this list that this can
		// easily be adapted to
		const char	*mechs[]={
					"Negotiate",
					"Kerberos",
					"NTLM",
					"Digest",
					"Schannel",
					NULL
		};
		pvt->_mechs=new char *[6+1];
		size_t	i=0;
		for (const char * const *m=mechs; *m; m++) {
			pvt->_mechs[i++]=charstring::duplicate(*m);
			#ifdef DEBUG_GSS
				stdoutput.printf("  %s\n",*m);
			#endif
		}
		pvt->_mechs[i]=NULL;

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
			bool			_acquired;
		#endif

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
		pvt->_actuallifetime=GSS_C_INDEFINITE;
	#elif defined(RUDIMENTS_HAS_SSPI)
		// FIXME: how to set TimeStamp (FILETIME) to max?
	#else
		pvt->_desiredlifetime=UINT_MAX;
		pvt->_actuallifetime=UINT_MAX;
	#endif

	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_desiredmechanisms=GSS_C_NO_OID_SET;
		pvt->_actualmechanisms=GSS_C_NO_OID_SET;

		pvt->_credusage=GSS_C_BOTH;
		pvt->_credentials=GSS_C_NO_CREDENTIAL;
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_credusage=SECPKG_CRED_BOTH;
		pvt->_acquired=false;
	#endif
}

gsscredentials::~gsscredentials() {
	release();
	delete pvt;
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
		// FIXME: convert TimeStamp to uint32_t
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

bool gsscredentials::acquireService(const char *name) {
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_credusage=GSS_C_ACCEPT;
		return acquire(name,charstring::length(name)+1,
					NULL,GSS_C_NT_HOSTBASED_SERVICE);
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_credusage=SECPKG_CRED_INBOUND;
		return acquire(name,0,NULL,NULL);
	#else
		return false;
	#endif
}

bool gsscredentials::acquireUserName(const char *name,
					const char *password) {
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_credusage=GSS_C_INITIATE;
		return acquire(name,charstring::length(name)+1,
					password,GSS_C_NT_USER_NAME);
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_credusage=SECPKG_CRED_OUTBOUND;
		return acquire(name,0,password,NULL);
	#else
		return false;
	#endif
}

bool gsscredentials::acquireKerberosPrincipalName(const char *name,
							const char *password) {
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_credusage=GSS_C_INITIATE;
		return acquire(name,charstring::length(name)+1,
				password,(gss_OID)GSS_KRB5_NT_PRINCIPAL_NAME);
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_credusage=SECPKG_CRED_OUTBOUND;
		return acquire(name,0,password,NULL);
	#else
		return false;
	#endif
}

bool gsscredentials::acquireAnonymous() {
	#if defined(RUDIMENTS_HAS_GSS) && \
		defined(RUDIMENTS_HAS_GSS_C_NT_ANONYMOUS)
		pvt->_credusage=GSS_C_INITIATE;
		return acquire("",0,NULL,GSS_C_NT_ANONYMOUS);
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_credusage=SECPKG_CRED_OUTBOUND;
		return acquire(NULL,0,NULL,NULL);
	#else
		return false;
	#endif
}

bool gsscredentials::acquireUid(uid_t uid) {
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_credusage=GSS_C_INITIATE;
		return acquire(&uid,sizeof(uid_t),NULL,
					GSS_C_NT_MACHINE_UID_NAME);
	#elif defined(RUDIMENTS_HAS_SSPI)
		// FIXME: get user name from uid?
		return false;
	#else
		return false;
	#endif
}

bool gsscredentials::acquireUidString(const char *uid) {
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_credusage=GSS_C_INITIATE;
		return acquire(uid,charstring::length(uid)+1,
					NULL,GSS_C_NT_STRING_UID_NAME);
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_credusage=SECPKG_CRED_OUTBOUND;
		return acquire(uid,0,NULL,NULL);
	#else
		return false;
	#endif
}

bool gsscredentials::acquire(const void *name,
				size_t namesize,
				const char *password,
				const void *nametype) {

	// release any previously acquired credentials
	release();

	bool	retval=false;

	#if defined(RUDIMENTS_HAS_GSS)

		#ifdef DEBUG_GSS
			stdoutput.printf("acquire credentials\n(%s/%s/",
					name,(password)?password:"(none)");
			if ((gss_OID)nametype==
					GSS_C_NT_HOSTBASED_SERVICE) {
				stdoutput.write("GSS_C_NT_HOSTBASED_SERVICE");
			}
			if ((gss_OID)nametype==(gss_OID)GSS_C_NT_USER_NAME) {
				stdoutput.write("GSS_C_NT_USER_NAME");
			}
			if ((gss_OID)nametype==
					(gss_OID)GSS_KRB5_NT_PRINCIPAL_NAME) {
				stdoutput.write("GSS_KRB5_NT_PRINCIPAL_NAME");
			}
			if ((gss_OID)nametype==
					(gss_OID)GSS_C_NT_STRING_UID_NAME) {
				stdoutput.write("GSS_C_NT_STRING_UID_NAME");
			}
			stdoutput.write(") - ");
		#endif

		// keep track of the name for nametypes
		// where the name is a string
		if ((gss_OID)nametype==
				GSS_C_NT_HOSTBASED_SERVICE ||
			(gss_OID)nametype==
				GSS_C_NT_USER_NAME ||
			(gss_OID)nametype==
				(gss_OID)GSS_KRB5_NT_PRINCIPAL_NAME ||
			(gss_OID)nametype==
				(gss_OID)GSS_C_NT_STRING_UID_NAME) {
			pvt->_name=(const char *)name;
		}

		// degenerate case
		if (!name && !password) {
			pvt->_major=GSS_S_COMPLETE;
			#ifdef DEBUG_GSS
				stdoutput.write("success (no credentials)\n\n");
			#endif
			return true;
		}

		// Acquire credentials associated with "name" and "password",
		// where "name" is type "nametype"...

		// by default, we'll use "no name"
		gss_name_t	desiredname=GSS_C_NO_NAME;

		if (name) {

			// if a name was provided then use it...
			gss_buffer_desc	namebuffer;
			namebuffer.value=(void *)name;
			namebuffer.length=namesize;

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
			OM_uint32	minor;
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

			OM_uint32	major;
			OM_uint32	minor;

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

		// "log in"...

		#ifdef RUDIMENTS_HAS_GSS_ACQUIRE_CRED_WITH_PASSWORD

		if (password) {

			// if a password was provided, then use it...

			// create a data/length struct
			// from the password string...
			gss_buffer_desc	passwordbuffer;
			passwordbuffer.value=(void *)password;
			passwordbuffer.length=
				charstring::length(
					(const char *)passwordbuffer.value)+1;

			// acquire the credentials associated
			// with the name/password...
			pvt->_major=gss_acquire_cred_with_password(
						&pvt->_minor,
						desiredname,
						&passwordbuffer,
						pvt->_desiredlifetime,
						pvt->_desiredmechanisms,
						pvt->_credusage,
						&pvt->_credentials,
						&pvt->_actualmechanisms,
						&pvt->_actuallifetime);
		} else {

		#else

		krb5_error_code	kerror=0;
		if (password) {

			// if a password was provided, then use it...

			// the code below is analagous to calling
			// kinit with a user/password...

			// FIXME: Somehow the code below doesn't allow the
			// user to use any mechanism but the default, but
			// calling gss_acquire_cred_with_password() does.

			krb5_context		kctx=NULL;
			krb5_principal		kpr=NULL;
			krb5_creds		kcrd;
			bytestring::zero(&kcrd,sizeof(kcrd));

			// create a context
			kerror=krb5_init_context(&kctx);

			// authenticate with the KDC
			if (!kerror) {
				kerror=krb5_parse_name(kctx,
						(const char *)name,&kpr);
			}
			if (!kerror) {
				// older implementations have a char *
				// type for argument 4
				kerror=krb5_get_init_creds_password(
						kctx,&kcrd,kpr,
						(char *)password,
						NULL,NULL,0,NULL,NULL);
			}

			// cache the credentials
			krb5_ccache	kcc=NULL;
			if (!kerror) {
				kerror=krb5_cc_default(kctx,&kcc);
			}
			if (!kerror) {
				kerror=krb5_cc_initialize(kctx,kcc,kpr);
			}
			if (!kerror) {
				kerror=krb5_cc_store_cred(kctx,kcc,&kcrd);
			}
			krb5_cc_close(kctx,kcc);

			// clean up
			krb5_free_cred_contents(kctx,&kcrd);
			krb5_free_principal(kctx,kpr);
			krb5_free_context(kctx);

			// At this point, the cred cache should contain
			// credentials for the specified user/password.
			//
			// The call to gss_acquire_cred() below will fetch
			// these creds from the cache.

			if (kerror) {
				// this will make the whole method fail
				// FIXME: getStatus() won't return why though...
				pvt->_major=GSS_S_FAILURE;
				#ifdef DEBUG_GSS
					stdoutput.write("failed (kinit) ");
				#endif
			}
		}

		if (!kerror) {

		#endif

			// acquire the credentials associated with the name...
			pvt->_major=gss_acquire_cred(&pvt->_minor,
						desiredname,
						pvt->_desiredlifetime,
						pvt->_desiredmechanisms,
						pvt->_credusage,
						&pvt->_credentials,
						&pvt->_actualmechanisms,
						&pvt->_actuallifetime);
		}

		// success/failure
		retval=(pvt->_major==GSS_S_COMPLETE);

		// save the actual mechanisms
		if (retval &&
			pvt->_actualmechanisms!=GSS_C_NO_OID_SET &&
			pvt->_actualmechanisms->count) {

			for (uint64_t i=0;
				i<pvt->_actualmechanisms->count; i++) {

				gssmechanism	*mech=new gssmechanism;
				mech->initialize(&pvt->
						_actualmechanisms->
						elements[index]);
				pvt->_amlist.append(mech);
			}
		}

		// clean up
		if (desiredname!=GSS_C_NO_NAME) {
			OM_uint32	minor;
			gss_release_name(&minor,&desiredname);
		}
		if (pvt->_desiredmechanisms!=GSS_C_NO_OID_SET) {
			OM_uint32	minor;
			gss_release_oid_set(&minor,&pvt->_desiredmechanisms);
			pvt->_desiredmechanisms=GSS_C_NO_OID_SET;
		}

	#elif defined(RUDIMENTS_HAS_SSPI)

		#ifdef DEBUG_GSS
			stdoutput.printf("acquire credentials\n(%s/%s/",
					name,(password)?password:"(none)");
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
		pvt->_name=(const char *)name;

		// degenerate case
		if (!name && !password) {
			pvt->_sstatus=SEC_E_OK;
			#ifdef DEBUG_GSS
				stdoutput.write("success (no credentials)\n\n");
			#endif
			return true;
		}

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
							NULL,
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

		pvt->_acquired=retval;
	#else
		retval=false;
	#endif

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

void gsscredentials::release() {

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
		pvt->_actuallifetime=GSS_C_INDEFINITE;
	#elif defined(RUDIMENTS_HAS_SSPI)
		// FIXME: how to set TimeStamp (FILETIME) to max?
	#else
		pvt->_actuallifetime=UINT_MAX;
	#endif
}

const char *gsscredentials::getName() {
	return pvt->_name;
}

uint32_t gsscredentials::getActualLifetime() {
	#if defined(RUDIMENTS_HAS_GSS)
		return pvt->_actuallifetime;
	#elif defined(RUDIMENTS_HAS_SSPI)
		// FIXME: convert TimeStamp to uint32_t
		return 0;
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
		getStatus(pvt->_major,GSS_C_GSS_CODE,&pvt->_status);
		pvt->_status.append("GSS - minor:\n");
		getStatus(pvt->_minor,GSS_C_GSS_CODE,&pvt->_status);
		pvt->_status.append("MECH - major:\n");
		getStatus(pvt->_major,GSS_C_MECH_CODE,&pvt->_status);
		pvt->_status.append("MECH - minor:\n");
		getStatus(pvt->_minor,GSS_C_MECH_CODE,&pvt->_status);
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_status.append("SSPI - status:\n");
		getStatus(pvt->_sstatus,0,&pvt->_status);
	#endif
	return pvt->_status.getString();
}

const char *gsscredentials::getGSSMajorStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		getStatus(pvt->_major,GSS_C_GSS_CODE,&pvt->_status);
	#elif defined(RUDIMENTS_HAS_SSPI)
		getStatus(pvt->_sstatus,0,&pvt->_status);
	#endif
	return pvt->_status.getString();
}

const char *gsscredentials::getGSSMinorStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		getStatus(pvt->_minor,GSS_C_GSS_CODE,&pvt->_status);
	#endif
	return pvt->_status.getString();
}

const char *gsscredentials::getMechanismMajorStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		getStatus(pvt->_major,GSS_C_MECH_CODE,&pvt->_status);
	#endif
	return pvt->_status.getString();
}

const char *gsscredentials::getMechanismMinorStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		getStatus(pvt->_minor,GSS_C_MECH_CODE,&pvt->_status);
	#endif
	return pvt->_status.getString();
}

void gsscredentials::getStatus(uint32_t status, int32_t type,
						stringbuffer *strb) {

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

			strb->append((unsigned char *)
						statusbuffer.value,
						statusbuffer.length);
			strb->append('\n');

			gss_release_buffer(&minor,&statusbuffer);
		} while (msgctx);

	#elif defined(RUDIMENTS_HAS_SSPI)

		const char	*str="";
		switch (status) {
			case SEC_E_INSUFFICIENT_MEMORY:
				str="SEC_E_INSUFFICIENT_MEMORY";
				break;
			case SEC_E_INVALID_HANDLE:
				str="SEC_E_INVALID_HANDLE";
				break;
			case SEC_E_UNSUPPORTED_FUNCTION:
				str="SEC_E_UNSUPPORTED_FUNCTION";
				break;
			case SEC_E_TARGET_UNKNOWN:
				str="SEC_E_TARGET_UNKNOWN";
				break;
			case SEC_E_INTERNAL_ERROR:
				str="SEC_E_INTERNAL_ERROR";
				break;
			case SEC_E_SECPKG_NOT_FOUND:
				str="SEC_E_SECPKG_NOT_FOUND";
				break;
			case SEC_E_NOT_OWNER:
				str="SEC_E_NOT_OWNER";
				break;
			case SEC_E_CANNOT_INSTALL:
				str="SEC_E_CANNOT_INSTALL";
				break;
			case SEC_E_INVALID_TOKEN:
				str="SEC_E_INVALID_TOKEN";
				break;
			case SEC_E_CANNOT_PACK:
				str="SEC_E_CANNOT_PACK";
				break;
			case SEC_E_QOP_NOT_SUPPORTED:
				str="SEC_E_QOP_NOT_SUPPORTED";
				break;
			case SEC_E_NO_IMPERSONATION:
				str="SEC_E_NO_IMPERSONATION";
				break;
			case SEC_E_LOGON_DENIED:
				str="SEC_E_LOGON_DENIED";
				break;
			case SEC_E_UNKNOWN_CREDENTIALS:
				str="SEC_E_UNKNOWN_CREDENTIALS";
				break;
			case SEC_E_NO_CREDENTIALS:
				str="SEC_E_NO_CREDENTIALS";
				break;
			case SEC_E_MESSAGE_ALTERED:
				str="SEC_E_MESSAGE_ALTERED";
				break;
			case SEC_E_OUT_OF_SEQUENCE:
				str="SEC_E_OUT_OF_SEQUENCE";
				break;
			case SEC_E_NO_AUTHENTICATING_AUTHORITY:
				str="SEC_E_NO_AUTHENTICATING_AUTHORITY";
				break;
			case SEC_E_CONTEXT_EXPIRED:
				str="SEC_E_CONTEXT_EXPIRED";
				break;
			case SEC_E_INCOMPLETE_MESSAGE:
				str="SEC_E_INCOMPLETE_MESSAGE";
				break;
			case SEC_E_OK:
				str="SEC_E_OK";
				break;
			default:
				str="";
		}
		strb->append(str)->append("\n");
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
			DWORD		_trailersize;
			DWORD		_blksize;
			bool		_kerberos;
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
		char			*_initiatortype;

		char			*_acceptor;
		char			*_acceptortype;

		bool			_isinitiator;
		bool			_isopen;

		bytebuffer		_readbuffer;
		uint64_t		_readbufferpos;
};

gsscontext::gsscontext() {
	pvt=new gsscontextprivate;
	pvt->_credentials=NULL;
	pvt->_fd=NULL;
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_desiredlifetime=GSS_C_INDEFINITE;
		pvt->_actuallifetime=GSS_C_INDEFINITE;
	#elif defined(RUDIMENTS_HAS_SSPI)
		// FIXME: how to set TimeStamp (FILETIME) to max?
		pvt->_maxmsgsize=0;
		pvt->_trailersize=0;
		pvt->_blksize=0;
		pvt->_kerberos=false;
	#else
		pvt->_desiredlifetime=UINT_MAX;
		pvt->_actuallifetime=UINT_MAX;
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
	pvt->_initiatortype=NULL;
	pvt->_acceptor=NULL;
	pvt->_acceptortype=NULL;
	pvt->_isinitiator=false;
	pvt->_isopen=false;
	pvt->_readbufferpos=0;
}

gsscontext::~gsscontext() {
	release();
	delete pvt;
}

void gsscontext::setCredentials(gsscredentials *credentials) {
	pvt->_credentials=credentials;
}

gsscredentials *gsscontext::getCredentials() {
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
		// FIXME: convert TimeStamp to uint32_t
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

bool gsscontext::initiate() {
	#if defined(RUDIMENTS_HAS_GSS)
		return initiate(pvt->_service,pvt->_servicelength+1,
						GSS_C_NT_HOSTBASED_SERVICE);
	#elif defined(RUDIMENTS_HAS_SSPI)
		return initiate(pvt->_service,0,NULL);
	#else
		return false;
	#endif
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

bool gsscontext::initiate(const void *name,
				size_t namesize,
				const void *nametype) {

	// release any previously initialized context
	release();

	#ifdef DEBUG_GSS
		stdoutput.write("initiate context...\n");
	#endif

	bool	error=false;

	#if defined(RUDIMENTS_HAS_GSS)

		// by default, we'll use "no name"
		gss_name_t	desiredname=GSS_C_NO_NAME;

		if (name) {

			// if a name was provided then use it...
			gss_buffer_desc	namebuffer;
			namebuffer.value=(void *)name;
			namebuffer.length=namesize;

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
				release();
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

					// clean up
					OM_uint32	minor;
					gss_release_buffer(&minor,
							&outputtoken);
					release();
					error=true;
					break;
				}
			}

			// clean up
			OM_uint32	minor;
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
					flags!=TOKEN_FLAGS_TYPE_ACCEPT) {

					#ifdef DEBUG_GSS
						stdoutput.write(
							"failed (receive)\n\n");
					#endif
			
					delete[] (unsigned char *)
							inputtoken.value;

					OM_uint32	minor;
					gss_release_buffer(&minor,&outputtoken);

					release();
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

		// bail on error
		if (error || pvt->_major!=GSS_S_COMPLETE) {
			return false;
		}

	#elif defined(RUDIMENTS_HAS_SSPI)

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

		SecBuffer         inputtoken;
		inputtoken.BufferType=SECBUFFER_TOKEN;

		SecBufferDesc     inputtokendesc;
		inputtokendesc.ulVersion=0;
		inputtokendesc.cBuffers=1;
		inputtokendesc.pBuffers=&inputtoken;

		// output buffer
		BYTE	*outbuf=new BYTE[pvt->_maxmsgsize];

		SecBuffer         outputtoken;
		outputtoken.BufferType=SECBUFFER_TOKEN;
		outputtoken.pvBuffer=outbuf;

		SecBufferDesc     outputtokendesc;
		outputtokendesc.ulVersion=0;
		outputtokendesc.cBuffers=1;
		outputtokendesc.pBuffers=&outputtoken;

		// get credentials
		CredHandle	*credentials=
			(pvt->_credentials)?
			(CredHandle *)(pvt->_credentials->getCredentials()):
			NULL;

		bool	done=false;
		while (!done) {

			outputtoken.cbBuffer=pvt->_maxmsgsize;

			if (inbuf) {
				inputtoken.cbBuffer=inbufsize;
				inputtoken.pvBuffer=inbuf;
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

			// bail on error
			if (SSPI_ERROR(pvt->_sstatus)) {
				#ifdef DEBUG_GSS
					stdoutput.printf(
						"failed "
						"(InitializeSecurityContext)"
						"\n%s\n",
						getStatus());
				#endif
				release();
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
					release();
					error=true;
					break;
				}
			}

			// are we done?
			done=!(pvt->_sstatus==SEC_I_CONTINUE_NEEDED ||
				pvt->_sstatus==SEC_I_COMPLETE_AND_CONTINUE);

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
					release();
					error=true;
					break;
				}
			}

			// receive token from peer, if necessary
			if (!done) {
				void	*inbuftemp;
				size_t	inbufsizetemp;
				uint32_t	flags=0;
				ssize_t		result=
					receiveToken(&flags,
							&inbuftemp,
							&inbufsizetemp);
				inbuf=(BYTE *)inbuftemp;
				inbufsize=inbufsizetemp;
				if (result<=0 ||
					flags!=TOKEN_FLAGS_TYPE_ACCEPT) {
					#ifdef DEBUG_GSS
						stdoutput.write(
							"failed (receive)\n\n");
					#endif
					release();
					error=true;
					break;
				}
			}
		}

		// actual mechanism will be populated by inquire() below...

		// clean up
		delete[] inbuf;
		delete[] outbuf;

		// bail on error
		if (error || pvt->_sstatus!=SEC_E_OK) {
			return false;
		}

		// we are the initiator
		pvt->_isinitiator=true;

		// the context is opn
		pvt->_isopen=true;
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

		// expose initiator and initiator type
		gss_buffer_desc	initiatorbuffer;
		gss_OID		initiatortype;
		pvt->_major=gss_display_name(&pvt->_minor,
						initiator,
						&initiatorbuffer,
						&initiatortype);
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

		gss_buffer_desc	initiatortypebuffer;
		pvt->_major=gss_oid_to_str(&pvt->_minor,
						initiatortype,
						&initiatortypebuffer);
		if (pvt->_major!=GSS_S_COMPLETE) {
			#ifdef DEBUG_GSS
				stdoutput.write("failed "
					"(oid_to_str(initiatortype))\n\n");
			#endif
			return false;
		}

		delete[] pvt->_initiatortype;
		pvt->_initiatortype=new char[initiatortypebuffer.length+1];
		charstring::copy(pvt->_initiatortype,
					(char *)initiatortypebuffer.value,
					initiatortypebuffer.length);
		pvt->_initiatortype[initiatortypebuffer.length]='\0';

		gss_release_buffer(&minor,&initiatortypebuffer);
	

		// expose acceptor and acceptor type
		gss_buffer_desc	acceptorbuffer;
		gss_OID		acceptortype;
		pvt->_major=gss_display_name(&pvt->_minor,
						acceptor,
						&acceptorbuffer,
						&acceptortype);
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

		gss_buffer_desc	acceptortypebuffer;
		pvt->_major=gss_oid_to_str(&pvt->_minor,
						acceptortype,
						&acceptortypebuffer);
		if (pvt->_major!=GSS_S_COMPLETE) {
			#ifdef DEBUG_GSS
				stdoutput.write("failed "
					"(oid_to_str(acceptortype))\n\n");
			#endif
			return false;
		}

		delete[] pvt->_acceptortype;
		pvt->_acceptortype=new char[acceptortypebuffer.length+1];
		charstring::copy(pvt->_acceptortype,
					(char *)acceptortypebuffer.value,
					acceptortypebuffer.length);
		pvt->_acceptortype[acceptortypebuffer.length]='\0';
	
		gss_release_buffer(&minor,&acceptortypebuffer);


		// expose isinitiator
		pvt->_isinitiator=isinitiator;


		// expose isopen
		pvt->_isopen=isopen;


		// return success/failure
		retval=(pvt->_major==GSS_S_COMPLETE);

	#elif defined(RUDIMENTS_HAS_SSPI)

		// get actual mechanism
		SecPkgContext_NegotiationInfo	SecPkgNegInfo;
		pvt->_sstatus=QueryContextAttributes(&pvt->_context,
						SECPKG_ATTR_NEGOTIATION_INFO,
						&SecPkgNegInfo);
		if (SSPI_ERROR(pvt->_sstatus))	{
			#ifdef DEBUG_GSS
				stdoutput.write("failed "
					"(QueryContextAttributes("
					"SECPKG_ATTR_NEGOTIATION_INFO))\n\n");
			#endif
			return false;
		}
		pvt->_actualmechanism.initialize(
					SecPkgNegInfo.PackageInfo->Name);
		FreeContextBuffer(SecPkgNegInfo.PackageInfo);

		// are we using kerberos?
		pvt->_kerberos=!charstring::compareIgnoringCase(
					pvt->_actualmechanism.getString(),
					"Kerberos");

		// FIXME: Arguably, we should reset the max message size,
		// now that we know the actual mechanism that we're using.
		// As it might be smaller.


		// get trailer and block sizes
		SecPkgContext_Sizes	SecPkgContextSizes;
		pvt->_sstatus=QueryContextAttributes(&pvt->_context,
						SECPKG_ATTR_SIZES,
						&SecPkgContextSizes);
		if (SSPI_ERROR(pvt->_sstatus)) {
			#ifdef DEBUG_GSS
				stdoutput.write("failed "
					"(QueryContextAttributes("
					"SECPKG_ATTR_SIZES))\n\n");
			#endif
			return false;
		}
		pvt->_trailersize=SecPkgContextSizes.cbSecurityTrailer;
		pvt->_blksize=SecPkgContextSizes.cbBlockSize;


		if (pvt->_isinitiator) {
			// FIXME: set:
			// char	*_initiator;
			// char	*_initiatortype;
		} else {

			// expose initiator
			// FIXME: does this work across machines
			// or with non-NTLM mechs?
			pvt->_sstatus=ImpersonateSecurityContext(&pvt->_context);
			if (SSPI_ERROR(pvt->_sstatus)) {
				#ifdef DEBUG_GSS
					stdoutput.write("failed "
						"(ImpersonateSecurityContext)"
						"\n\n");
				#endif
				return false;
			}
			DWORD	usernamesize=0;
			GetUserName(NULL,&usernamesize);
			TCHAR	*username=new TCHAR[usernamesize];
			if (!GetUserName(username,&usernamesize)) {
				delete[] username;
				#ifdef DEBUG_GSS
					stdoutput.write(
						"failed (GetUserName)\n\n");
				#endif
				return false;
			}
			pvt->_sstatus=RevertSecurityContext(&pvt->_context);
			if (SSPI_ERROR(pvt->_sstatus)) {
				#ifdef DEBUG_GSS
					stdoutput.write("failed "
						"(RevertSecurityContext)\n\n");
				#endif
				return false;
			}
			// FIXME: needs TCHAR -> CHAR conversion
			pvt->_initiator=charstring::duplicate(username);
			delete[] username;

			// FIXME: set:
			// char	*_initiatortype;
		}

		// FIXME: QuerySecurityContextToken?

		// FIXME: set:
		// char	*_acceptor;
		// char	*_acceptortype;

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
		stdoutput.printf("  desired flags:\n");
		printFlags(pvt->_desiredflags);
		stdoutput.printf("  actual flags:\n");
		printFlags(pvt->_actualflags);
		stdoutput.printf("  service: %s\n",
			(pvt->_service)?pvt->_service:"(none)");
		stdoutput.printf("  initiator: %s\n",
			(pvt->_initiator)?pvt->_initiator:"(none)");
		stdoutput.printf("  initiator type: %s\n",
			(pvt->_initiatortype)?
			pvt->_initiatortype:"(none)");
		stdoutput.printf("  acceptor: %s\n",
			(pvt->_acceptor)?pvt->_acceptor:"(none)");
		stdoutput.printf("  acceptor type: %s\n",
			(pvt->_acceptortype)?
			pvt->_acceptortype:"(none)");
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
	release();

	#ifdef DEBUG_GSS
		stdoutput.write("accept context...\n");
	#endif

	bool	error=false;

	#if defined(RUDIMENTS_HAS_GSS)

		// accept the context...
		gss_buffer_desc	inputtoken;
		gss_buffer_desc	outputtoken;

		gss_cred_id_t	credentials=
			(pvt->_credentials)?
			(gss_cred_id_t)pvt->_credentials->getCredentials():
			GSS_C_NO_CREDENTIAL;

		// FIXME: expose this in the API
		gss_name_t	clientname;

		gss_OID	actualmechoid=GSS_C_NO_OID;

		do {

			// receive token from peer, into inputtoken...
			uint32_t	flags=0;
			inputtoken.value=NULL;
			inputtoken.length=0;
			if (receiveToken(&flags,
					&inputtoken.value,
					&inputtoken.length)<=0 ||
				flags!=TOKEN_FLAGS_TYPE_INITIATE) {

				#ifdef DEBUG_GSS
					stdoutput.write("failed (receive)\n\n");
				#endif

				// clean up
				delete[] (unsigned char *)inputtoken.value;
				release();
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
						&clientname,
						&actualmechoid,
						&outputtoken,
						&pvt->_actualflags,
						&pvt->_actuallifetime,
						// FIXME: support
						// delegated credentials
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
				release();
				error=true;
				break;
			}

			// send token to peer, if necessary
			if (outputtoken.length) {
				if (sendToken(TOKEN_FLAGS_TYPE_ACCEPT,
						outputtoken.value,
						outputtoken.length)!=
						(ssize_t)outputtoken.length) {

					// clean up
					OM_uint32	minor;
					gss_release_buffer(&minor,&outputtoken);

					release();
					error=true;
					break;
				}
			}

			// clean up
			OM_uint32	minor;
			gss_release_buffer(&minor,&outputtoken);

		} while (pvt->_major==GSS_S_CONTINUE_NEEDED);

		// bail on error
		if (error || pvt->_major!=GSS_S_COMPLETE) {
			return false;
		}

		// populate actual mechanism
		pvt->_actualmechanism.initialize(actualmechoid);

	#elif defined(RUDIMENTS_HAS_SSPI)

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
		inputtokendesc.ulVersion=0;
		inputtokendesc.cBuffers=1;
		inputtokendesc.pBuffers=&inputtoken;

		// output buffer
		BYTE	*outbuf=new BYTE[pvt->_maxmsgsize];

		SecBuffer	outputtoken;
		outputtoken.BufferType=SECBUFFER_TOKEN;
		outputtoken.pvBuffer=outbuf;

		SecBufferDesc	outputtokendesc;
		outputtokendesc.ulVersion=0;
		outputtokendesc.cBuffers=1;
		outputtokendesc.pBuffers=&outputtoken;

		// get credentials
		CredHandle	*credentials=
			(pvt->_credentials)?
			(CredHandle *)(pvt->_credentials->getCredentials()):
			NULL;

		bool	done=false;
		bool	first=true;
		while (!done) {

			// receive token from peer, into inputtoken...
			uint32_t	flags=0;
			void		*inbuf;
			size_t		inbufsize;
			ssize_t		result=receiveToken(&flags,
								&inbuf,
								&inbufsize);
			if (result<=0 || flags!=TOKEN_FLAGS_TYPE_INITIATE) {
				#ifdef DEBUG_GSS
					stdoutput.write("failed (receive)\n\n");
				#endif
				release();
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
			if (SSPI_ERROR(pvt->_sstatus)) {
				#ifdef DEBUG_GSS
					stdoutput.printf(
						"failed "
						"(AcceptSecurityContext)"
						"\n%s\n",
						getStatus());
				#endif
				release();
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
					release();
					error=true;
					break;
				}
			}

			// are we done?
			done=!(pvt->_sstatus==SEC_I_CONTINUE_NEEDED ||
				pvt->_sstatus==SEC_I_COMPLETE_AND_CONTINUE);

			first=false;

			// send token to peer, if necessary
			if (outputtoken.cbBuffer) {
				if (sendToken(TOKEN_FLAGS_TYPE_ACCEPT,
						outbuf,
						outputtoken.cbBuffer)!=
						(ssize_t)outputtoken.cbBuffer) {
					#ifdef DEBUG_GSS
						stdoutput.write(
							"failed (send)\n\n");
					#endif
					release();
					error=true;
					break;
				}
			}
		}

		delete[] outbuf;

		// actual mechanism will be populated by inquire() below...

		// bail on error
		if (error || pvt->_sstatus!=SEC_E_OK) {
			return false;
		}

		// the context is opn
		pvt->_isopen=true;
	#endif

	#ifdef DEBUG_GSS
		stdoutput.printf("success\n\n");
	#endif

	// get additional info about the context
	return inquire();
}

void gsscontext::release() {

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
	#endif

	// reset initiator and acceptor names
	delete[] pvt->_initiator;
	pvt->_initiator=NULL;
	delete[] pvt->_initiatortype;
	pvt->_initiatortype=NULL;

	delete[] pvt->_acceptor;
	pvt->_acceptor=NULL;
	delete[] pvt->_acceptortype;
	pvt->_acceptortype=NULL;

	// reset the "actuals"
	#if defined(RUDIMENTS_HAS_GSS)
		pvt->_actuallifetime=GSS_C_INDEFINITE;
	#elif defined(RUDIMENTS_HAS_SSPI)
		// FIXME: how to set TimeStamp (FILETIME) to max?
		pvt->_maxmsgsize=0;
		pvt->_trailersize=0;
		pvt->_blksize=0;
		pvt->_kerberos=false;
	#else
		pvt->_actuallifetime=UINT_MAX;
	#endif
	pvt->_actualmechanism.clear();
	pvt->_actualflags=0;

	// reset states
	pvt->_isinitiator=false;
	pvt->_isopen=false;
	pvt->_readbuffer.clear();
	pvt->_readbufferpos=0;
}

uint32_t gsscontext::getActualLifetime() {
	#if defined(RUDIMENTS_HAS_GSS)
		return pvt->_actuallifetime;
	#elif defined(RUDIMENTS_HAS_SSPI)
		// FIXME: convert TimeStamp to uint32_t
		return 0;
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
		return 0;
	#else
		return 0;
	#endif
}

const char *gsscontext::getInitiator() {
	return pvt->_initiator;
}

const char *gsscontext::getInitiatorType() {
	return pvt->_initiatortype;
}

const char *gsscontext::getAcceptor() {
	return pvt->_acceptor;
}

const char *gsscontext::getAcceptorType() {
	return pvt->_acceptortype;
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

	#elif defined(RUDIMENTS_HAS_SSPI)

		ULONG	qop=0;

		// EncryptMessage encrypts into a set of buffers.  Exactly what
		// buffers and how the encrypted data is formatted in them
		// depends on the encryption package.
		//
		// In the general case, a "data" and "trailer" buffer are
		// required and the data buffer must hold the unencrypted data.
		// This data will be overwritten and any overflow will be
		// placed in the trailer.
		//
		// For kerberos, a "padding" buffer is also required for
		// who-knows-what.

		if (pvt->_kerberos) {

			// allocate the output buffer, large enough to hold the
			// input, trailer, and padding
			*outputsize=inputsize+pvt->_trailersize+pvt->_blksize;
			*output=new BYTE[*outputsize];

			// initialize the output buffer with the input
			bytestring::copy(*output,input,inputsize);

			// prepare security buffers
			SecBuffer         secbuf[3];
			secbuf[0].BufferType=SECBUFFER_DATA;
			secbuf[0].cbBuffer=inputsize;
			secbuf[0].pvBuffer=*output;

			secbuf[1].BufferType=SECBUFFER_TOKEN;
			secbuf[1].cbBuffer=pvt->_trailersize;
			secbuf[1].pvBuffer=*output+inputsize;

			secbuf[2].BufferType=SECBUFFER_PADDING;
			secbuf[2].cbBuffer=pvt->_blksize;
			secbuf[2].pvBuffer=*output+inputsize+pvt->_trailersize;

			SecBufferDesc     secbufdesc;
			secbufdesc.ulVersion=SECBUFFER_VERSION;
			secbufdesc.cBuffers=3;
			secbufdesc.pBuffers=secbuf;

			// encrypt the input
			pvt->_sstatus=EncryptMessage(
					&pvt->_context,qop,&secbufdesc,0);

		} else {

			// allocate a buffer to hold the input size,
			// input, trailer size, and trailer
			*outputsize=sizeof(DWORD)+inputsize+
						sizeof(DWORD)+pvt->_trailersize;
			*output=new BYTE[*outputsize];

			// store that data, in that order
			*((DWORD *)*output)=inputsize;
			bytestring::copy(*output+sizeof(DWORD),
						input,inputsize);
			*((DWORD *)(*output+sizeof(DWORD)+inputsize))=
							pvt->_trailersize;

			// prepare security buffers
			SecBuffer         secbuf[2];
			secbuf[0].BufferType=SECBUFFER_DATA;
			secbuf[0].cbBuffer=inputsize;
			secbuf[0].pvBuffer=*output+sizeof(DWORD);

			secbuf[1].BufferType=SECBUFFER_TOKEN;
			secbuf[1].cbBuffer=pvt->_trailersize;
			secbuf[1].pvBuffer=*output+sizeof(DWORD)+
						inputsize+sizeof(DWORD);

			SecBufferDesc     secbufdesc;
			secbufdesc.ulVersion=0;
			secbufdesc.cBuffers=2;
			secbufdesc.pBuffers=secbuf;

			// encrypt the input
			pvt->_sstatus=EncryptMessage(
					&pvt->_context,qop,&secbufdesc,0);
		}

		if (!SSPI_ERROR(pvt->_sstatus)) {
			#ifdef DEBUG_GSS_WRAP
				stdoutput.write(" success\n\n");
			#endif
			return true;
		}

	#endif

	#ifdef DEBUG_GSS_WRAP
		stdoutput.write(" failed\n\n");
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

	#elif defined(RUDIMENTS_HAS_SSPI)

		ULONG	qop=0;

		// DecryptMessage decrypts a set of buffers.  Exactly what
		// buffers and how the encrypted data is formatted in them
		// depends on the encryption package...

		if (!pvt->_kerberos) {

			// In the general case, "data" and "trailer" buffers
			// were required to store the encrypted data.
			// DecryptMessage needs to know the location of each
			// and will ultimately store the decrypted data in the
			// "data" buffer.

			// get output size, output, trailer size, and trailer
			*outputsize=*((DWORD *)input);

			// create a buffer to store the output and initialize
			// it with the contents of the "data" buffer
			*output=(unsigned char *)bytestring::duplicate(
							(input+sizeof(DWORD)),
							*outputsize);

			// get the trailer size and location of the trailer
			DWORD	trailersize=
				*((DWORD *)(input+sizeof(DWORD)+*outputsize));
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
			secbufdesc.ulVersion=0;
			secbufdesc.cBuffers=2;
			secbufdesc.pBuffers=secbuf;

			pvt->_sstatus=DecryptMessage(
					&pvt->_context,&secbufdesc,0,&qop);

		} else {

			// For kerberos, "data", "trailer", and "padding"
			// buffers were required to store the encrypted data,
			// but apparently, information for how to do the
			// decryption was stored in there too, so
			// DecryptMessage just needs the input buffer as a
			// single "stream".  It will allocate a buffer
			// internally to store the decrypted data.  A pointer
			// to this buffer must be designated in the "data"
			// buffer.

			// prepare buffers
			SecBuffer         secbuf[2];
			secbuf[0].BufferType=SECBUFFER_STREAM;
			secbuf[0].cbBuffer=inputsize;
			secbuf[0].pvBuffer=(void *)input;

			secbuf[1].BufferType=SECBUFFER_DATA;
			secbuf[1].cbBuffer=*outputsize;
			secbuf[1].pvBuffer=*output;

			SecBufferDesc     secbufdesc;
			secbufdesc.ulVersion=SECBUFFER_VERSION;
			secbufdesc.cBuffers=2;
			secbufdesc.pBuffers=secbuf;

			pvt->_sstatus=DecryptMessage(
					&pvt->_context,&secbufdesc,0,&qop);
		}

		retval=!SSPI_ERROR(pvt->_sstatus);
	#endif

	#ifdef DEBUG_GSS_WRAP
		if (retval) {
			stdoutput.printf("unwrap(%d,",*outputsize);
			stdoutput.safePrint(*output,*outputsize);
			stdoutput.write(") success\n\n");
		} else {
			stdoutput.write("unwrap() failed\n\n");
		}
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
	#else
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
	if (result<=0 || !(tokenflags&TOKEN_FLAGS_TYPE_DATA)) {
		return result;
	}

	// unwrap
	unsigned char	*data=NULL;
	size_t		datasize=0;
	if (!unwrap((unsigned char *)tokendata,tokensize,
					&data,&datasize)) {
		delete[] data;
		return RESULT_ERROR;
	}

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

	#ifdef DEBUG_GSS_RECEIVE
		stdoutput.write("receiveToken(");
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

	// copy size out and create data buffer
	*tokensize=size;
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

ssize_t gsscontext::write(const void *buf, ssize_t count) {

	// create token
	unsigned char	*tokendata;
	size_t		tokensize;
	if (!wrap((const unsigned char *)buf,count,&tokendata,&tokensize)) {
		return RESULT_ERROR;
	}

	// send token
	ssize_t	result=sendToken(TOKEN_FLAGS_TYPE_DATA,tokendata,tokensize);
	if (result!=(ssize_t)tokensize) {
		return (result<=0)?result:RESULT_ERROR;
	}
	return count;
}

ssize_t gsscontext::sendToken(uint32_t tokenflags,
					const void *tokendata,
					size_t tokensize) {

	#ifdef DEBUG_GSS_SEND
		stdoutput.printf("sendToken(%08x,%d,",tokenflags,tokensize);
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
		getStatus(pvt->_major,GSS_C_GSS_CODE,&pvt->_status);
		pvt->_status.append("GSS - minor:\n");
		getStatus(pvt->_minor,GSS_C_GSS_CODE,&pvt->_status);
		pvt->_status.append("MECH - major:\n");
		getStatus(pvt->_major,GSS_C_MECH_CODE,&pvt->_status);
		pvt->_status.append("MECH - minor:\n");
		getStatus(pvt->_minor,GSS_C_MECH_CODE,&pvt->_status);
	#elif defined(RUDIMENTS_HAS_SSPI)
		pvt->_status.append("SSPI - status:\n");
		getStatus(pvt->_sstatus,0,&pvt->_status);
	#endif
	return pvt->_status.getString();
}

const char *gsscontext::getGSSMajorStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		getStatus(pvt->_major,GSS_C_GSS_CODE,&pvt->_status);
	#elif defined(RUDIMENTS_HAS_SSPI)
		getStatus(pvt->_sstatus,0,&pvt->_status);
	#endif
	return pvt->_status.getString();
}

const char *gsscontext::getGSSMinorStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		getStatus(pvt->_minor,GSS_C_GSS_CODE,&pvt->_status);
	#endif
	return pvt->_status.getString();
}

const char *gsscontext::getMechanismMajorStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		getStatus(pvt->_major,GSS_C_MECH_CODE,&pvt->_status);
	#endif
	return pvt->_status.getString();
}

const char *gsscontext::getMechanismMinorStatus() {
	pvt->_status.clear();
	#if defined(RUDIMENTS_HAS_GSS)
		getStatus(pvt->_minor,GSS_C_MECH_CODE,&pvt->_status);
	#endif
	return pvt->_status.getString();
}

void gsscontext::getStatus(uint32_t status, int32_t type, stringbuffer *strb) {

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

			strb->append((unsigned char *)
						statusbuffer.value,
						statusbuffer.length);
			strb->append('\n');

			gss_release_buffer(&minor,&statusbuffer);
		} while (msgctx);

	#elif defined(RUDIMENTS_HAS_SSPI)

		const char	*str="";
		switch (status) {
			case SEC_E_INSUFFICIENT_MEMORY:
				str="SEC_E_INSUFFICIENT_MEMORY";
				break;
			case SEC_E_INVALID_HANDLE:
				str="SEC_E_INVALID_HANDLE";
				break;
			case SEC_E_UNSUPPORTED_FUNCTION:
				str="SEC_E_UNSUPPORTED_FUNCTION";
				break;
			case SEC_E_TARGET_UNKNOWN:
				str="SEC_E_TARGET_UNKNOWN";
				break;
			case SEC_E_INTERNAL_ERROR:
				str="SEC_E_INTERNAL_ERROR";
				break;
			case SEC_E_SECPKG_NOT_FOUND:
				str="SEC_E_SECPKG_NOT_FOUND";
				break;
			case SEC_E_NOT_OWNER:
				str="SEC_E_NOT_OWNER";
				break;
			case SEC_E_CANNOT_INSTALL:
				str="SEC_E_CANNOT_INSTALL";
				break;
			case SEC_E_INVALID_TOKEN:
				str="SEC_E_INVALID_TOKEN";
				break;
			case SEC_E_CANNOT_PACK:
				str="SEC_E_CANNOT_PACK";
				break;
			case SEC_E_QOP_NOT_SUPPORTED:
				str="SEC_E_QOP_NOT_SUPPORTED";
				break;
			case SEC_E_NO_IMPERSONATION:
				str="SEC_E_NO_IMPERSONATION";
				break;
			case SEC_E_LOGON_DENIED:
				str="SEC_E_LOGON_DENIED";
				break;
			case SEC_E_UNKNOWN_CREDENTIALS:
				str="SEC_E_UNKNOWN_CREDENTIALS";
				break;
			case SEC_E_NO_CREDENTIALS:
				str="SEC_E_NO_CREDENTIALS";
				break;
			case SEC_E_MESSAGE_ALTERED:
				str="SEC_E_MESSAGE_ALTERED";
				break;
			case SEC_E_OUT_OF_SEQUENCE:
				str="SEC_E_OUT_OF_SEQUENCE";
				break;
			case SEC_E_NO_AUTHENTICATING_AUTHORITY:
				str="SEC_E_NO_AUTHENTICATING_AUTHORITY";
				break;
			case SEC_E_CONTEXT_EXPIRED:
				str="SEC_E_CONTEXT_EXPIRED";
				break;
			case SEC_E_INCOMPLETE_MESSAGE:
				str="SEC_E_INCOMPLETE_MESSAGE";
				break;
			case SEC_E_OK:
				str="SEC_E_OK";
				break;
			default:
				str="";
		}
		strb->append(str)->append("\n");
	#endif
}
