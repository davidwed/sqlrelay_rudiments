// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/private/inttypes.h>

#include <rudiments/stringbuffer.h>

#ifdef RUDIMENTS_HAS_GSS
	#include <gssapi/gssapi.h>
#endif

class gssprivate;
class gssmechanismprivate;
class gsscredentialsprivate;
class gsscontextprivate;

class filedescriptor;
