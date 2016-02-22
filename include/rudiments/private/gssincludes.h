// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/private/inttypes.h>

#include <rudiments/security.h>
#include <rudiments/stringbuffer.h>

#ifdef RUDIMENTS_HAS_GSS
	#if defined(RUDIMENTS_HAS_GSSAPI_GSSAPI_H)
		#include <gssapi/gssapi.h>
	#elif defined(RUDIMENTS_HAS_GSSAPI_H)
		#include <gssapi.h>
	#endif
#endif

#ifdef RUDIMENTS_HAS_SSPI
	#ifdef RUDIMENTS_HAVE_WINSOCK2_H
		#include <winsock2.h>
	#endif

	// must be included after winsock2.h
	#ifdef RUDIMENTS_HAVE_WINDOWS_H
		#include <windows.h>
	#endif

	#define SECURITY_WIN32
	#include <security.h>

	// map some GSS flags
	#define	GSS_C_DELEG_FLAG	ISC_REQ_DELEGATE
	#define	GSS_C_MUTUAL_FLAG	ISC_REQ_MUTUAL_AUTH
	#define	GSS_C_REPLAY_FLAG	ISC_REQ_REPLAY_DETECT
	#define	GSS_C_SEQUENCE_FLAG	ISC_REQ_SEQUENCE_DETECT
	#define	GSS_C_CONF_FLAG		ISC_REQ_CONFIDENTIALITY
	#define	GSS_C_INTEG_FLAG	0
	#define	GSS_C_ANON_FLAG		0
	#define	GSS_C_PROT_READY_FLAG	0
	#define	GSS_C_TRANS_FLAG	0
	#define	GSS_C_DELEG_POLICY_FLAG	0
#endif


enum gsstokenformat_t {
	GSSTOKENFORMAT_KRB=0,
	GSSTOKENFORMAT_TLS
};

class gssprivate;
class gssmechanismprivate;
class gsscredentialsprivate;
class gsscontextprivate;

class filedescriptor;
