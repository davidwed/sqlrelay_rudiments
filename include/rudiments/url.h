// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_URL_H
#define RUDIMENTS_URL_H

#include <rudiments/private/urlincludes.h>

class urlprivate;

/** The url class provides read access to urls.
 *
 *  Its parent class: file provides provides open(), read(), and close()
 *  methods.
 *
 *  http urls, with Basic Authentication, are supported on all platforms.
 *
 *  urls for other protocols, including ftp, https, scp, sftp, and others, are
 *  supported if rudiments was compiled with support for libcurl.
 *
 *  Examples of supported urls include:
 *
 *    * http://somedomain.com/path/to/file.html
 *    * http://user:password@somedomain.com/path/to/file.html
 *    * http://[/path/to/userpwdfile]@somedomain.com/path/to/file.html
 *
 *  The url class supports storing the user:password portion of a url in a
 *  user-password file.  These files may be referenced in the url by enclosing
 *  the full pathname of the file in square brackets.  The file should consist
 *  of a single line containing the user and password, separated by a colon.
 *
 *  For example:
 *  
 *    myusername:mypassword
 *
 *  */
class RUDIMENTS_DLLSPEC url : public file {
	public:

		/** Creates an instance of the url class. */
		url();

		/** Deletes this instance of the url class. */
		virtual	~url();

	#include <rudiments/private/url.h>
};

#endif
