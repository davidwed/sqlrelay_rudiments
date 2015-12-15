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

#ifdef RUDIMENTS_HAS_GSS
	#ifndef RUDIMENTS_HAS_GSS_STR_TO_OID
		#include "gssoid.cpp"
	#endif

	// for gss_acquire_cred_with_password and possibly gss_str_to_oid
	#ifdef RUDIMENTS_HAS_GSSAPI_GSSAPI_EXT_H
		#include <gssapi/gssapi_ext.h>
	#endif

	#if !defined(GSS_KRB5_NT_PRINCIPAL_NAME) && \
		defined(RUDIMENTS_HAS_GSSAPI_GSSAPI_KRB5_H)
		#include <gssapi/gssapi_krb5.h>
	#endif
#else
	// for UINT_MAX
	#ifdef RUDIMENTS_HAVE_LIMITS_H
		#include <limits.h>
	#endif
#endif

#define TOKEN_FLAGS_TYPE_INITIATE	(1<<0)
#define TOKEN_FLAGS_TYPE_ACCEPT		(1<<1)
#define TOKEN_FLAGS_TYPE_DATA		(1<<2)
#define TOKEN_FLAGS_TYPE_MIC		(1<<3)
#define TOKEN_FLAGS_ENCRYPTED		(1<<4)
#define TOKEN_FLAGS_VERIFY		(1<<5)

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

	#ifdef RUDIMENTS_HAS_GSS
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
			}

			pvt->_mechs[mechs->count]=NULL;

			gss_release_oid_set(&minor,&mechs);
		}
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


class gssmechanismprivate {
	friend class gssmechanism;
	private:
		char	*_str;

		#ifdef RUDIMENTS_HAS_GSS
			gss_OID	_oid;
		#else
			void	*_oid;
		#endif
};

gssmechanism::gssmechanism() {
	pvt=new gssmechanismprivate;
	pvt->_str=NULL;
	#ifdef RUDIMENTS_HAS_GSS
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

	#ifdef RUDIMENTS_HAS_GSS
		gss_buffer_desc	mechbuffer;
		mechbuffer.value=(void *)str;
		mechbuffer.length=charstring::length(str);

		OM_uint32		major;
		OM_uint32		minor;
		major=gss_str_to_oid(&minor,&mechbuffer,&pvt->_oid);
		return (major==GSS_S_COMPLETE);
	#else
		return false;
	#endif
}

bool gssmechanism::initialize(const void *oid) {

	clear();

	#ifdef RUDIMENTS_HAS_GSS
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
	#else
		return false;
	#endif
}

void gssmechanism::clear() {

	#ifdef RUDIMENTS_HAS_GSS
		if (pvt->_oid!=GSS_C_NO_OID) {
			OM_uint32	minor;
			gss_release_oid(&minor,&pvt->_oid);
		}
		pvt->_oid=GSS_C_NO_OID;
	#else
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
		#ifdef RUDIMENTS_HAS_GSS
			OM_uint32		_major;
			OM_uint32		_minor;
		#else
			uint32_t		_major;
			uint32_t		_minor;
		#endif

		stringbuffer			_status;

		const char			*_name;

		#ifdef RUDIMENTS_HAS_GSS
			OM_uint32		_desiredlifetime;
			OM_uint32		_actuallifetime;
		#else
			uint32_t		_desiredlifetime;
			uint32_t		_actuallifetime;
		#endif

		#ifdef RUDIMENTS_HAS_GSS
			gss_OID_set		_desiredmechanisms;
			gss_OID_set		_actualmechanisms;

			gss_cred_usage_t	_credusage;
			gss_cred_id_t		_credentials;
		#endif

		linkedlist< gssmechanism * >	_dmlist;
		linkedlist< gssmechanism * >	_amlist;
};

gsscredentials::gsscredentials() {
	pvt=new gsscredentialsprivate;

	pvt->_major=0;
	pvt->_minor=0;
	pvt->_name=NULL;

	#ifdef RUDIMENTS_HAS_GSS
		pvt->_desiredlifetime=GSS_C_INDEFINITE;
		pvt->_actuallifetime=GSS_C_INDEFINITE;
	#else
		pvt->_desiredlifetime=UINT_MAX;
		pvt->_actuallifetime=UINT_MAX;
	#endif

	#ifdef RUDIMENTS_HAS_GSS
		pvt->_desiredmechanisms=GSS_C_NO_OID_SET;
		pvt->_actualmechanisms=GSS_C_NO_OID_SET;

		pvt->_credusage=GSS_C_BOTH;
		pvt->_credentials=GSS_C_NO_CREDENTIAL;
	#endif
}

gsscredentials::~gsscredentials() {
	release();
	delete pvt;
}

void gsscredentials::setDesiredLifetime(uint32_t desiredlifetime) {
	pvt->_desiredlifetime=desiredlifetime;
}

uint32_t gsscredentials::getDesiredLifetime() {
	return pvt->_desiredlifetime;
}

void gsscredentials::addDesiredMechanism(gssmechanism *mech) {
	pvt->_dmlist.append(mech);
}

void gsscredentials::removeDesiredMechanism(gssmechanism *mech) {
	pvt->_dmlist.removeAll(mech);
}

bool gsscredentials::inDesiredMechanisms(gssmechanism *mech) {

	// just return false for degenerate mechs
	#ifdef RUDIMENTS_HAS_GSS
		if (!mech || (gss_OID)mech->getObjectId()==GSS_C_NO_OID) {
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
		// this isn't super efficient, but I don't know of a better way
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
	#ifdef RUDIMENTS_HAS_GSS
		pvt->_credusage=GSS_C_ACCEPT;
		return acquire(name,charstring::length(name)+1,
					NULL,GSS_C_NT_HOSTBASED_SERVICE);
	#else
		return false;
	#endif
}

bool gsscredentials::acquireUserName(const char *name,
					const char *password) {
	#ifdef RUDIMENTS_HAS_GSS
		pvt->_credusage=GSS_C_INITIATE;
		return acquire(name,charstring::length(name)+1,
					password,GSS_C_NT_USER_NAME);
	#else
		return false;
	#endif
}

bool gsscredentials::acquireKerberosPrincipalName(const char *name,
							const char *password) {
	#ifdef RUDIMENTS_HAS_GSS
		pvt->_credusage=GSS_C_INITIATE;
		return acquire(name,charstring::length(name)+1,
				password,(gss_OID)GSS_KRB5_NT_PRINCIPAL_NAME);
	#else
		return false;
	#endif
}

bool gsscredentials::acquireAnonymous() {
	#ifdef RUDIMENTS_HAS_GSS
		pvt->_credusage=GSS_C_INITIATE;
		return acquire("",0,NULL,GSS_C_NT_ANONYMOUS);
	#else
		return false;
	#endif
}

bool gsscredentials::acquireUid(uid_t uid) {
	#ifdef RUDIMENTS_HAS_GSS
		pvt->_credusage=GSS_C_INITIATE;
		return acquire(&uid,sizeof(uid_t),NULL,
					GSS_C_NT_MACHINE_UID_NAME);
	#else
		return false;
	#endif
}

bool gsscredentials::acquireUidString(const char *uid) {
	#ifdef RUDIMENTS_HAS_GSS
		pvt->_credusage=GSS_C_INITIATE;
		return acquire(uid,charstring::length(uid)+1,
					NULL,GSS_C_NT_STRING_UID_NAME);
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

	#ifdef RUDIMENTS_HAS_GSS
		// keep track of the name for nametypes
		// where the name is a string
		if ((gss_OID)nametype==GSS_C_NT_HOSTBASED_SERVICE ||
			(gss_OID)nametype==GSS_C_NT_USER_NAME ||
			(gss_OID)nametype==
				(gss_OID)GSS_KRB5_NT_PRINCIPAL_NAME ||
			(gss_OID)nametype==
				(gss_OID)GSS_C_NT_STRING_UID_NAME) {
			pvt->_name=(const char *)name;
		}

		// degenerate case
		if (!name && !password) {
			pvt->_major=GSS_S_COMPLETE;
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
			#ifdef RUDIMENTS_HAS_GSS_ACQUIRE_CRED_WITH_PASSWORD
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
			#else
				#error no gss_acquire_cred_with_password or anything like it
			#endif
		} else {

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

		// return success/failure
		return (pvt->_major==GSS_S_COMPLETE);
	#else
		return false;
	#endif
}

void gsscredentials::release() {

	// release the credentials
	#ifdef RUDIMENTS_HAS_GSS
		OM_uint32	minor;
		gss_release_cred(&minor,&pvt->_credentials);
	#endif

	// reset the name
	pvt->_name=NULL;

	// reset the "actuals"
	#ifdef RUDIMENTS_HAS_GSS
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
	#ifdef RUDIMENTS_HAS_GSS
		pvt->_actuallifetime=GSS_C_INDEFINITE;
	#else
		pvt->_actuallifetime=UINT_MAX;
	#endif
}

const char *gsscredentials::getName() {
	return pvt->_name;
}

uint32_t gsscredentials::getActualLifetime() {
	return pvt->_actuallifetime;
}

bool gsscredentials::inActualMechanisms(gssmechanism *mech) {

	// just return false for degenerate mechs
	#ifdef RUDIMENTS_HAS_GSS
		if ((gss_OID)mech->getObjectId()==GSS_C_NO_OID) {
			return false;
		}
	#else
		if (!mech->getObjectId()) {
			return false;
		}
	#endif

	// just return false for degenerate sets
	#ifdef RUDIMENTS_HAS_GSS
		if (pvt->_actualmechanisms==GSS_C_NO_OID_SET) {
			return false;
		}
	#endif

	// look for the mech in the set
	#ifdef RUDIMENTS_HAS_GSS
		OM_uint32	major;
		OM_uint32	minor;
		int		present;
		major=gss_test_oid_set_member(&minor,
						(gss_OID)mech->getObjectId(),
						pvt->_actualmechanisms,
						&present);
		return (major==GSS_S_COMPLETE && present!=0);
	#else
		return false;
	#endif
}

uint64_t gsscredentials::getActualMechanismCount() {
	#ifdef RUDIMENTS_HAS_GSS
		return (pvt->_actualmechanisms==GSS_C_NO_OID_SET)?
					0:pvt->_actualmechanisms->count;
	#else
		return 0;
	#endif
}

gssmechanism *gsscredentials::getActualMechanism(uint64_t index) {
	#ifdef RUDIMENTS_HAS_GSS
		if (pvt->_actualmechanisms==GSS_C_NO_OID_SET ||
				index>pvt->_actualmechanisms->count) {
			return NULL;
		}
		gssmechanism	*mech=new gssmechanism;
		mech->initialize(&pvt->_actualmechanisms->elements[index]);
		pvt->_amlist.append(mech);
		return mech;
	#else
		return NULL;
	#endif
}

const void *gsscredentials::getCredentials() {
	#ifdef RUDIMENTS_HAS_GSS
		return pvt->_credentials;
	#else
		return NULL;
	#endif
}

uint32_t gsscredentials::getMajorStatus() {
	return pvt->_major;
}

uint32_t gsscredentials::getMinorStatus() {
	return pvt->_minor;
}

const char *gsscredentials::getStatus() {
	pvt->_status.clear();
	pvt->_status.append("GSS - major:\n");
	#ifdef RUDIMENTS_HAS_GSS
		getStatus(pvt->_major,GSS_C_GSS_CODE);
	#endif
	pvt->_status.append("GSS - minor:\n");
	#ifdef RUDIMENTS_HAS_GSS
		getStatus(pvt->_minor,GSS_C_GSS_CODE);
	#endif
	pvt->_status.append("MECH - major:\n");
	#ifdef RUDIMENTS_HAS_GSS
		getStatus(pvt->_major,GSS_C_MECH_CODE);
	#endif
	pvt->_status.append("MECH - minor:\n");
	#ifdef RUDIMENTS_HAS_GSS
		getStatus(pvt->_minor,GSS_C_MECH_CODE);
	#endif
	return pvt->_status.getString();
}

void gsscredentials::getStatus(uint32_t status, int32_t type) {

	#ifdef RUDIMENTS_HAS_GSS
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
	#endif
}



class gsscontextprivate {
	friend class gsscontext;
	private:
		#ifdef RUDIMENTS_HAS_GSS
			OM_uint32	_major;
			OM_uint32	_minor;
		#else
			uint32_t	_major;
			uint32_t	_minor;
		#endif

		stringbuffer		_status;

		gsscredentials		*_credentials;

		filedescriptor		*_fd;

		#ifdef RUDIMENTS_HAS_GSS
			OM_uint32	_desiredlifetime;
			OM_uint32	_actuallifetime;
		#else
			uint32_t	_desiredlifetime;
			uint32_t	_actuallifetime;
		#endif

		gssmechanism		*_desiredmechanism;
		gssmechanism		_actualmechanism;

		#ifdef RUDIMENTS_HAS_GSS
			OM_uint32	_desiredflags;
			OM_uint32	_actualflags;
		#else
			uint32_t	_desiredflags;
			uint32_t	_actualflags;
		#endif

		const char		*_service;
		size_t			_servicelength;

		#ifdef RUDIMENTS_HAS_GSS
			gss_ctx_id_t	_context;
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
	#ifdef RUDIMENTS_HAS_GSS
		pvt->_desiredlifetime=GSS_C_INDEFINITE;
		pvt->_actuallifetime=GSS_C_INDEFINITE;
	#else
		pvt->_desiredlifetime=UINT_MAX;
		pvt->_actuallifetime=UINT_MAX;
	#endif
	pvt->_desiredmechanism=NULL;
	pvt->_desiredflags=0;
	pvt->_actualflags=0;
	pvt->_service=NULL;
	pvt->_servicelength=0;
	#ifdef RUDIMENTS_HAS_GSS
		pvt->_context=GSS_C_NO_CONTEXT;
	#else
		pvt->_context=NULL;
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
	pvt->_desiredlifetime=desiredlifetime;
}

uint32_t gsscontext::getDesiredLifetime() {
	return pvt->_desiredlifetime;
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
	#ifdef RUDIMENTS_HAS_GSS
		return initiate(pvt->_service,pvt->_servicelength+1,
						GSS_C_NT_HOSTBASED_SERVICE);
	#else
		return false;
	#endif
}

bool gsscontext::initiate(const void *name,
				size_t namesize,
				const void *nametype) {

	// release any previously initialized context
	release();

	#ifdef RUDIMENTS_HAS_GSS

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
				release();
				break;
			}

			// send token to peer, if necessary
			if (outputtoken.length) {
				if (sendToken(TOKEN_FLAGS_TYPE_INITIATE,
						outputtoken.value,
						outputtoken.length)!=
						(ssize_t)outputtoken.length) {

					// clean up
					OM_uint32	minor;
					gss_release_buffer(&minor,
							&outputtoken);
					release();
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
			
					delete[] (unsigned char *)
							inputtoken.value;

					OM_uint32	minor;
					gss_release_buffer(&minor,&outputtoken);

					release();
					break;
				}

			} else {
				// break out if we've completed the process
				break;
			}
		}

		// bail on error
		if (pvt->_major!=GSS_S_COMPLETE) {
			return false;
		}

		// populate actual mechanism
		pvt->_actualmechanism.initialize(actualmechoid);
	#endif

	// get additional info about the context
	return inquire();
}

bool gsscontext::inquire() {

	#ifdef RUDIMENTS_HAS_GSS

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
		return (pvt->_major==GSS_S_COMPLETE);
	#else
		return false;
	#endif
}

bool gsscontext::accept() {

	// release any previously accepted context
	release();

	#ifdef RUDIMENTS_HAS_GSS

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

				// clean up
				delete[] (unsigned char *)inputtoken.value;
				release();
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

			// bail on error
			if (GSS_ERROR(pvt->_major)) {
				release();
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
					break;
				}
			}

			// clean up
			OM_uint32	minor;
			gss_release_buffer(&minor,&outputtoken);

		} while (pvt->_major==GSS_S_CONTINUE_NEEDED);

		// bail on error
		if (pvt->_major!=GSS_S_COMPLETE) {
			return false;
		}

		// populate actual mechanism
		pvt->_actualmechanism.initialize(actualmechoid);

	#endif

	// get additional info about the context
	return inquire();
}

void gsscontext::release() {

	#ifdef RUDIMENTS_HAS_GSS
		// delete the context
		if (pvt->_context!=GSS_C_NO_CONTEXT) {
			gss_delete_sec_context(
				&pvt->_minor,&pvt->_context,GSS_C_NO_BUFFER);
			// automatically sets pvt->_context to GSS_C_NO_CONTEXT
		}
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
	#ifdef RUDIMENTS_HAS_GSS
		pvt->_actuallifetime=GSS_C_INDEFINITE;
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
	return pvt->_actuallifetime;
}

gssmechanism *gsscontext::getActualMechanism() {
	return &pvt->_actualmechanism;
}

uint32_t gsscontext::getActualFlags() {
	return pvt->_actualflags;
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

	#ifdef RUDIMENTS_HAS_GSS

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

			return true;
		}

		// FIXME: are there cases where outputbuffer
		// should be cleaned up here too?
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

	#ifdef RUDIMENTS_HAS_GSS

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

			return true;
		}

		// FIXME: are there cases where outputbuffer
		// should be cleaned up here too?
	#endif

	return false;
}

bool gsscontext::getMic(const unsigned char *message,
					size_t messagesize,
					unsigned char **mic,
					size_t *micsize) {

	#ifdef RUDIMENTS_HAS_GSS

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
	#endif

	return false;
}

bool gsscontext::verifyMic(const unsigned char *message,
					size_t messagesize,
					const unsigned char *mic,
					size_t micsize) {

	#ifdef RUDIMENTS_HAS_GSS

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
	#else
		return false;
	#endif
}

ssize_t gsscontext::read(void *buf, ssize_t count) {

	// first, return any buffered data
	ssize_t bytestoread=count;
	ssize_t	bytesread=0;
	size_t	bytesinbuffer=(pvt->_readbuffer.getSize()-pvt->_readbufferpos);
	if (bytesinbuffer) {

		// copy data out...
		if (bytesinbuffer<=(size_t)bytestoread) {
			bytestring::copy(
				buf,pvt->_readbuffer.getBuffer(),bytesinbuffer);
			pvt->_readbuffer.clear();
			pvt->_readbufferpos=0;
			bytesread=bytesinbuffer;
			buf=((unsigned char *)buf)+bytesinbuffer;
			bytestoread-=bytesinbuffer;
		} else {
			bytestring::copy(
				buf,pvt->_readbuffer.getBuffer(),bytestoread);
			pvt->_readbufferpos+=bytestoread;
			bytesread=bytestoread;
			buf=((unsigned char *)buf)+bytestoread;
			bytestoread=0;
		}
	}

	// next, receive tokens from the peer...
	while (bytestoread) {

		// receive token
		uint32_t	tokenflags=0;
		void		*tokendata=NULL;
		size_t		tokensize=0;
		ssize_t	result=receiveToken(&tokenflags,&tokendata,&tokensize);
		if (result<=0 || !(tokenflags&TOKEN_FLAGS_TYPE_DATA)) {
			return result;
		}

		// unwrap
		unsigned char	*data;
		size_t		datasize;
		if (!unwrap((unsigned char *)tokendata,tokensize,
						&data,&datasize)) {
			delete[] data;
			return RESULT_ERROR;
		}

		// copy data out...
		if (datasize<=(size_t)bytestoread) {
			bytestring::copy(buf,data,datasize);
			bytesread+=datasize;
			buf=((unsigned char *)buf)+datasize;
			bytestoread-=datasize;
		} else {
			bytestring::copy(buf,data,bytestoread);
			bytesread+=bytestoread;
			buf=((unsigned char *)buf)+bytestoread;
			bytestoread=0;

			// buffer what wasn't copied out
			pvt->_readbuffer.append(data+bytesread,
						datasize-bytesread);
		}
		delete[] data;
	}

	return bytesread;
}

ssize_t gsscontext::receiveToken(uint32_t *tokenflags,
					void **tokendata,
					size_t *tokensize) {

	// read token flags
	ssize_t		result=fullRead(tokenflags,sizeof(*tokenflags));
	if (result!=sizeof(*tokenflags)) {
		return (result<=0)?result:RESULT_ERROR;
	}

	// read token size
	uint32_t	size;
	result=fullRead(&size,sizeof(size));
	if (result!=sizeof(size)) {
		return (result<=0)?result:RESULT_ERROR;
	}

	// copy size out and create data buffer
	*tokensize=size;
	*tokendata=new unsigned char[size];

	// read token data
	result=fullRead(*tokendata,size);
	if (result!=(ssize_t)size) {
		return (result<=0)?result:RESULT_ERROR;
	}
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
	// write token flags
	ssize_t		result=fullWrite(&tokenflags,sizeof(tokenflags));
	if (result!=sizeof(tokenflags)) {
		return (result<=0)?result:RESULT_ERROR;
	}

	// write token size
	uint32_t	size=tokensize;
	result=fullWrite(&size,sizeof(size));
	if (result!=sizeof(size)) {
		return (result<=0)?result:RESULT_ERROR;
	}

	// write token data
	result=fullWrite(tokendata,tokensize);
	if (result!=(ssize_t)tokensize) {
		return (result<=0)?result:RESULT_ERROR;
	}
	return result;
}

ssize_t gsscontext::fullRead(void *data, ssize_t size) {

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

uint32_t gsscontext::getRemainingLifetime() {
	#ifdef RUDIMENTS_HAS_GSS
		OM_uint32	remainingtime;
		pvt->_major=gss_context_time(&pvt->_minor,
						pvt->_context,
						&remainingtime);
		return (pvt->_major==GSS_S_COMPLETE)?remainingtime:0;
	#else
		return 0;
	#endif
}

uint32_t gsscontext::getMajorStatus() {
	return pvt->_major;
}

uint32_t gsscontext::getMinorStatus() {
	return pvt->_minor;
}

const char *gsscontext::getStatus() {
	pvt->_status.clear();
	pvt->_status.append("GSS - major:\n");
	#ifdef RUDIMENTS_HAS_GSS
		getStatus(pvt->_major,GSS_C_GSS_CODE);
	#endif
	pvt->_status.append("GSS - minor:\n");
	#ifdef RUDIMENTS_HAS_GSS
		getStatus(pvt->_minor,GSS_C_GSS_CODE);
	#endif
	pvt->_status.append("MECH - major:\n");
	#ifdef RUDIMENTS_HAS_GSS
		getStatus(pvt->_major,GSS_C_MECH_CODE);
	#endif
	pvt->_status.append("MECH - minor:\n");
	#ifdef RUDIMENTS_HAS_GSS
		getStatus(pvt->_minor,GSS_C_MECH_CODE);
	#endif
	return pvt->_status.getString();
}

void gsscontext::getStatus(uint32_t status, int32_t type) {

	#ifdef RUDIMENTS_HAS_GSS

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
	#endif
}
