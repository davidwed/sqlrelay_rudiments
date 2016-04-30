// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SHA1_H
#define RUDIMENTS_SHA1_H

#include <rudiments/private/sha1includes.h>

class sha1private;

/** sha1 errors */
enum sha1error {
	SHA1_ERROR_SUCCESS=0,
	SHA1_ERROR_NULL,
	SHA1_ERROR_INPUT_TOO_LONG,
	SHA1_ERROR_STATE_ERROR
};

/** The sha1 class provides a simple sha1 implementation. */
class RUDIMENTS_DLLSPEC sha1 {
	public:
		/** Creates an instance of the sha1 class. */
		sha1();

		/** Deletes this instance of the sha1 class. */
		virtual	~sha1();

		/** Appends "length" bytes of "data" to the data to be
		 *  hashed.  Returns true on success or false if an error
		 *  occurred. */
		bool	append(const unsigned char *data, uint32_t length);

		/** Returns the current hash on success or NULL if an error
		 *  occurred.  Note that the current hash will be an empty
		 *  string if no data has been appended yet. */
		const unsigned char	*getHash();

		/** Returns the length of the hash in bytes. */
		uint32_t	getHashLength();

		/** Clears the current hash and any data that had
 		 *  previously been appended.  Returns true on success or
 		 *  false if an error occurred. */
		bool	clear();

		/** Returns the most recent error. */
		sha1error	getError();

	#include <rudiments/private/sha1.h>
};

#endif
