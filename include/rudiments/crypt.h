// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CRYPT_H
#define RUDIMENTS_CRYPT_H

#include <rudiments/private/cryptincludes.h>

// The crypt class provides a method that is useful for password encryption.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class crypt {
	public:
		static char	*encrypt(const char *password,
						const char *salt);
				// Encrypts "password" using the des algorithm
				// and "salt" (which should be a 2 character
				// string from the set [a-zA-Z0-9./].
				//
				// Returns the encrypted password on success
				// or NULL on failure.
				//
				// Note that this method allocates a buffer
				// internally and returns it.  The calling
				// program must deallocate this buffer.

#ifdef RUDIMENTS_HAS_THREADS
		static	bool	needsMutex();
			// If your system doesn't support crypt_r() then this
			// class needs a mutex to assure thread safety.
			//
			// This method returns true if this class needs a mutex
			// to operate safely in a threaded environment and false
			// otherwise.
		static	void	setMutex(pthread_mutex_t *mutex);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.
#endif

	#include <rudiments/private/crypt.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
