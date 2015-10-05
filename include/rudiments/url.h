// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_URL_H
#define RUDIMENTS_URL_H

#include <rudiments/private/urlincludes.h>

class urlprivate;

/** The url class provides ... */
class RUDIMENTS_DLLSPEC url : public file {
	public:

		/** Creates an instance of the url class. */
		url();

		/** Creates an instance of the url class
		 *  that is a copy of "u". */
		url(const url &u);

		/** Makes this instance of the url class
		 *  idenitcal to "u". */
		url	&operator=(const url &u);

		/** Deletes this instance of the url class. */
		virtual	~url();

	#include <rudiments/private/url.h>
};

#endif
