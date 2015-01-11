// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_STATICARRAY_H
#define RUDIMENTS_STATICARRAY_H

#include <rudiments/private/inttypes.h>

/** The staticarray class allows you to store a pre-defined number of values
 *  and access them with array-like syntax.
 *
 *  "valuetype" can be any scalar type, including a pointer, but cannot be
 *  an array.
 *
 *  Ie. these are legal:
 * 	staticarray< int, 10 > d;
 * 	staticarray< myclass, 10 > d;
 * 	staticarray< myclass *, 10 > d;
 *
 *  These are not legal and whether they work or even compile would be
 *  platform-dependent:
 * 	staticarray< int[100], 10 > d;
 * 	staticarray< myclass[100], 10 > d;
 * 	staticarray< myclass *[100], 10 > d;
 *
 *  However, it is possible to create an array of arrays by nesting
 *  staticarrays or dynamicarrays, like:
 *
 * 	staticarray< staticarray< int >, 10 > d;
 * 	staticarray< staticarray< myclass >, 10 > d;
 * 	staticarray< staticarray< myclass * >, 10 > d;
 * 	staticarray< dynamicarray< int >, 10 > d;
 * 	staticarray< dynamicarray< myclass >, 10 > d;
 * 	staticarray< dynamicarray< myclass * >, 10 > d;
 */
template < class valuetype, uint64_t length >
class staticarray {
	public:
		/** Creates an empty instance of the staticarray class with
		 *  "length" elements.  "length" is given in the template
		 *  definition. */
		staticarray();

		/** Creates an instance of the staticarray class
		 *  that is a copy of "v". */
		staticarray(const staticarray<valuetype,length> &v);

		/** Makes this instance of the staticarray class
		 *  identical to "v". */
		staticarray<valuetype,length>	&operator=(
					const staticarray<valuetype,length> &v);

		/** Deletes this instance of the staticarray class and all of
		 *  its values. */
		~staticarray();

		/** Provides access to the "index"th element of the
		 *  staticarray. */
		valuetype	&operator[](uint64_t index);

		/** Returns the length of the array. */
		uint64_t	getLength() const;

		/** Clears the array, deleting all of its values. */
		void	clear();

	#include <rudiments/private/staticarray.h>
};

#include <rudiments/private/staticarrayinlines.h>

#endif
