// Copyright (c) 2016 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MD5_H
#define RUDIMENTS_MD5_H

#include <rudiments/private/md5includes.h>

class md5private;

/** The md5 class provides a simple md5 implementation. */
class RUDIMENTS_DLLSPEC md5 : public hash {
	public:
		/** Creates an instance of the md5 class. */
		md5();

		/** Deletes this instance of the md5 class. */
		virtual	~md5();

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
		hasherror_t	getError();

	#include <rudiments/private/md5.h>
};

#endif
