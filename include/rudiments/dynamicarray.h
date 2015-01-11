// Copyright (c) 2015 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DYNAMICARRAY_H
#define RUDIMENTS_DYNAMICARRAY_H

#include <rudiments/private/dynamicarrayincludes.h>

/** The dynamicarray class allows you to store an arbitrary number of values
 *  and access them with array-like syntax. */
template < class valuetype >
class dynamicarray {
	public:
		/** Creates an empty instance of the dynamicarray class. */
		dynamicarray();

		/** Creates an empty instance of the dynamicarray class.
		 *  The initial extent will be created with "initialsize"
		 *  members and each successive extent will be created with
		 *  "increment" members. */
		dynamicarray(int initialsize, int increment);

		/** Deletes this instance of the dynamicarray class and all of
		 *  its values. */
		~dynamicarray();

		/** Provides access to the "index"th element of the
		 *  dynamicarray. */
		valuetype	&operator[](int index);

		/** Returns the length of the array. */
		int	getLength();

		/** Clears the array, deleting all of its values. */
		void	clear();

	#include <rudiments/private/dynamicarray.h>
};

#include <rudiments/private/dynamicarrayinlines.h>

#endif
