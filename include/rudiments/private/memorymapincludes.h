// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/private/inttypes.h>
#include <sys/types.h>
#ifdef RUDIMENTS_HAVE_MMAP
	#include <sys/mman.h>
#endif
#ifdef RUDIMENTS_HAVE_CREATE_FILE_MAPPING
	#define _WINSOCKAPI_
	#include <windows.h>
#endif
#ifndef RUDIMENTS_HAVE_MMAP
	#define	PROT_NONE	0
	#define	PROT_EXEC	1
	#define	PROT_READ	2
	#define	PROT_WRITE	4

	#define MAP_SHARED	1
	#define MAP_PRIVATE	2
	#define	MAP_FIXED 	4
 	#define	MAP_DENYWRITE	8
 	#define	MAP_EXECUTABLE	16
 	#define	MAP_NORESERVE	32
	#define	MAP_LOCKED	64
	#define	MAP_GROWSDOWN	128
	#define	MAP_ANONYMOUS	256
	#define	MAP_ANON	512
	#define	MAP_32BIT	1024
	#define	MAP_AUTOGROW	2048
	#define	MAP_AUTORESRV	4096
	#define	MAP_COPY	8192
	#define	MAP_LOCAL	16384
#endif
