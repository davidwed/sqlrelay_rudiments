// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#ifdef ENABLE_RUDIMENTS_INLINES
	#define RUDIMENTS_INLINE inline
#else
	#define RUDIMENTS_INLINE
#endif

#ifdef DONT_INLINE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
	#define RUDIMENTS_TEMPLATE_INLINE
#else
	#define RUDIMENTS_TEMPLATE_INLINE inline
#endif
