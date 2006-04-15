// Copyright (c) 2005 David Muse
// See the COPYING file for more information.

#ifndef ERROR_H
#define ERROR_H

#include <errno.h>

// The error class provides an interface to the errors that system calls
// can generate.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class error {
	public:
		static	void		clearError();
					// Set the current system call error to
					// 0, "Success".

		static	void		setErrorNumber(int err);
					// Set the current system call error to
					// "err".  "err" should be one of the
					// errors defined in error.h such as
					// EINTR, ENOMEM, etc.

		static	int		getErrorNumber();
					// Returns the current system call
					// error.  Will match one of the errors
					// defined in error.h such as EINTR,
					// ENOMEM, etc.

		static	const char	*getErrorString();
					// Returns a string representation
					// of the current system call error.
					//
					// Note: this method returns a pointer
					// to an internal buffer which may be
					// overwritten at any time.
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
