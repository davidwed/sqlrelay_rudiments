// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

#include <stdlib.h>

#ifdef HAVE_SETENV
RUDIMENTS_INLINE environment::~environment() {
}
#endif

RUDIMENTS_INLINE char *environment::getValue(const char *variable) const {
	return getenv(variable);
}

#ifdef HAVE_SETENV
RUDIMENTS_INLINE bool environment::setValue(const char *variable,
						const char *value) {
	return (setenv(variable,value,1)!=-1);
}
#endif

RUDIMENTS_INLINE void environment::remove(const char *variable) {
#ifdef HAVE_UNSETENV
	unsetenv(variable);
#else
	// I know this isn't the same as calling unsetenv, but as far as I
	// know, it's all that can be done.
	setValue(variable,"");
#endif
}
