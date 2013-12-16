// Copyright (c) 2005 David Muse
// See the COPYING file for more information.

#ifndef ERROR_H
#define ERROR_H

#include <rudiments/private/errorincludes.h>

/** The error class provides an interface to system errors. */
class RUDIMENTS_DLLSPEC error {
	public:
		/** Set the current posix system error to 0, "Success". */
		static	void		clearError();

		/** Set the current posix system error to "err".  "err" should
		 *  be one of the posix system errors such as EINTR, ENOMEM,
		 *  etc. */
		static	void		setErrorNumber(int32_t err);

		/** Returns the current posix system error.  Will match one of
		 *  the posix system errors such as EINTR, ENOMEM, etc. */
		static	int32_t		getErrorNumber();

		/** Returns a string representation of the current posix system
		 *  error.
		 *  Note that the return value is allocated internally and
		 *  returned, and must be freed by the calling program. */
		static	char	*getErrorString();

		/** Returns the current native error number.  For systems (like
		 *  Windows) that have their own native error number system,
		 *  distinct from posix, this method returns that error number.
		 *  For systems where the posix system error is the native
		 *  error, this method returns the same error as
		 *  getErrorNumber(). */
		static	int32_t		getNativeErrorNumber();

		/** Returns a string representatation of the current native
		 *  system error as returned by getNativeErrorNumber().
		 *  Note that the return value is allocated internally and
		 *  returned, and must be freed by the calling program. */
		static	char		*getNativeErrorString();
};

#endif
