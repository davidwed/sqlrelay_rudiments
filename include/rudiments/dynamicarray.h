// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DYNAMICARRAY_H
#define RUDIMENTS_DYNAMICARRAY_H

#include <rudiments/private/dynamicarrayincludes.h>

/** The dynamicarray class allows you to store an arbitrary number of values
 *  and access them with array-like syntax.
 *
 *  Internally, the class stores these values in a series of extents.  The
 *  size of the initial and incremental extents are defined in the constructor.
 *
 *  "valuetype" can be any scalar type, including a pointer, but cannot be
 *  an array.
 *
 *  Ie. these are legal:
 * 	dynamicarray< int > d;
 * 	dynamicarray< myclass > d;
 * 	dynamicarray< myclass * > d;
 *
 *  These are not legal and whether they work or even compile would be
 *  platform-dependent:
 * 	dynamicarray< int[100] > d;
 * 	dynamicarray< myclass[100] > d;
 * 	dynamicarray< myclass *[100] > d;
 *
 *  However, it is possible to create an array of arrays by nesting
 *  dynamicarrays and/or staticarrays, like:
 *
 * 	dynamicarray< dynamicarray< int > > d;
 * 	dynamicarray< dynamicarray< myclass > > d;
 * 	dynamicarray< dynamicarray< myclass * > > d;
 * 	dynamicarray< staticarray< int > > d;
 * 	dynamicarray< staticarray< myclass > > d;
 * 	dynamicarray< staticarray< myclass * > > d;
 */
template < class valuetype >
class dynamicarray {
	public:
		/** Creates an empty instance of the dynamicarray class. */
		dynamicarray();

		/** Creates an empty instance of the dynamicarray class.
		 *  The initial extent will be created with "initialsize"
		 *  members and each successive extent will be created with
		 *  "increment" members. */
		dynamicarray(uint64_t initialsize, uint64_t increment);

		/** Creates an instance of the dynamicarray class
		 *  that is a copy of "v". */
		dynamicarray(const dynamicarray<valuetype> &v);

		/** Makes this instance of the dynamicarray class
		 *  identical to "v". */
		dynamicarray<valuetype>	&operator=(
					const dynamicarray<valuetype> &v);

		/** Deletes this instance of the dynamicarray class and all of
		 *  its values. */
		~dynamicarray();

		/** Provides access to the "index"th element of the
		 *  dynamicarray. */
		valuetype	&operator[](uint64_t index);

		/** Provides access to the "index"th element of the
		 *  dynamicarray. */
		const valuetype	&operator[](uint64_t index) const;

		/** Returns the length of the array. */
		uint64_t	getLength() const;

		/** Clears the array, deleting all of its values. */
		void	clear();

	#include <rudiments/private/dynamicarray.h>
};

#include <rudiments/private/dynamicarrayinlines.h>

#endif
