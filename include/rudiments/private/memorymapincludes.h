// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#include <rudiments/private/inttypes.h>
#include <sys/types.h>
#ifdef HAVE_MMAP
	#include <sys/mman.h>
#endif
#ifdef HAVE_CREATE_FILE_MAPPING
	#include <windows.h>
	#define	PROT_NONE	0
	#define	PROT_EXEC	1
	#define	PROT_READ	2
	#define	PROT_WRITE	4
	#define	MAP_FIXED 	0
 	#define	MAP_DENYWRITE	1
 	#define	MAP_EXECUTABLE	2
 	#define	MAP_NORESERVE	4
	#define	MAP_LOCKED	8
	#define	MAP_GROWSDOWN	16
	#define	MAP_ANONYMOUS	32
	#define	MAP_ANON	64
	#define	MAP_32BIT	128
	#define	MAP_AUTOGROW	256
	#define	MAP_AUTORESRV	512
	#define	MAP_COPY	1024
	#define	MAP_LOCAL	2048
#endif
