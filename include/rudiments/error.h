// Copyright (c) 2005 David Muse
// See the COPYING file for more information.

#ifndef ERROR_H
#define ERROR_H

#include <rudiments/private/errorincludes.h>

/** The error class provides an interface to the errors that system calls
 *  can generate. */

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class RUDIMENTS_DLLSPEC error {
	public:
		/** Set the current system call error to 0, "Success". */
		static	void		clearError();

		/** Set the current system call error to "err".  "err" should
		 *  be one of the errors defined in error.h such as EINTR,
		 *  ENOMEM, etc. */
		static	void		setErrorNumber(int32_t err);

		/** Returns the current system call error.  Will match one of
		 *  the errors defined in error.h such as EINTR, ENOMEM, etc. */
		static	int32_t		getErrorNumber();

		/** Returns a string representation of the current system
		 *  error.  Note: this method returns a pointer to an internal
		 *  buffer which may be overwritten at any time. */
		static	const char	*getErrorString();
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
