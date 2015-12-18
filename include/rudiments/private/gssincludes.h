// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/private/inttypes.h>

#include <rudiments/stringbuffer.h>

#ifdef RUDIMENTS_HAS_GSS
	#include <gssapi/gssapi.h>
#endif

#ifndef RUDIMENTS_HAS_GSS_NAME_MACROS
	extern gss_OID GSS_C_NT_HOSTBASED_SERVICE;
	extern gss_OID GSS_C_NT_USER_NAME;
	extern const gss_OID_desc * const GSS_KRB5_NT_PRINCIPAL_NAME;
	extern gss_OID GSS_C_NT_ANONYMOUS;
	extern gss_OID GSS_C_NT_MACHINE_UID_NAME;
	extern gss_OID GSS_C_NT_STRING_UID_NAME;
#endif

class gssprivate;
class gssmechanismprivate;
class gsscredentialsprivate;
class gsscontextprivate;

class filedescriptor;
