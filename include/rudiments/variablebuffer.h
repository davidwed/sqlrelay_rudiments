// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef VARIABLEBUFFER_H
#define VARIABLEBUFFER_H

#include <rudiments/private/config.h>

// The variablebuffer class can be used to store raw data of arbitrary length.

class variablebuffer {
	public:
			variablebuffer(unsigned long initialsize,
						unsigned long increment);
			~variablebuffer();

		// The append() methods return a pointer to the variablebuffer
		// instance.  This enables append chaining:
		// sb->append(data1)->append(data2)->append(data3);
		variablebuffer	*append(const unsigned char *data,
						unsigned long size);
			// Appends "data" to the variablebuffer.

		void	clear();
			// Empties the variablebuffer.

		unsigned char	*getBuffer();
				// Returns the current data stored in the
				// variablebuffer.
		unsigned long	getSize();
				// Returns the current amount of data stored
				// in the variablebuffer.

	#include <rudiments/private/variablebuffer.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/variablebufferinlines.h>
#endif

#endif
