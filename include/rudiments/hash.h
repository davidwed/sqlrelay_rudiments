// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_HASH_H
#define RUDIMENTS_HASH_H

#include <rudiments/private/hashincludes.h>

/** hash errors */
enum hasherror_t {
	HASH_ERROR_SUCCESS=0,
	HASH_ERROR_NULL,
	HASH_ERROR_INPUT_TOO_LONG,
	HASH_ERROR_STATE_ERROR
};

/** The hash class provides a simple hash interface. */
class RUDIMENTS_DLLSPEC hash {
	public:
		/** Creates an instance of the hash class. */
		hash();

		/** Deletes this instance of the hash class. */
		virtual	~hash();

		/** Appends "length" bytes of "data" to the data to be
		 *  hashed.  Returns true on success or false if an error
		 *  occurred. */
		virtual	bool	append(const unsigned char *data,
							uint32_t length)=0;

		/** Returns the current hash on success or NULL if an error
		 *  occurred.  Note that the current hash will be an empty
		 *  string if no data has been appended yet. */
		virtual	const unsigned char	*getHash()=0;

		/** Returns the length of the hash in bytes. */
		virtual	uint32_t	getHashLength()=0;

		/** Clears the current hash and any data that had
 		 *  previously been appended.  Returns true on success or
 		 *  false if an error occurred. */
		virtual	bool	clear()=0;

		/** Returns the most recent error. */
		virtual	hasherror_t	getError()=0;
};

#endif
