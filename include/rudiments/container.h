// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CONTAINER_H
#define RUDIMENTS_CONTAINER_H

// The container class provides base class for classes which store related data.

class container {
	public:
			container();
			~container();

		// These methods control the behavior of the setter methods.
		void	copyReferences();
			// Instructs the setter methods to copy the values
			// passed into them rather than just keeping track
			// of the pointers.
		void	dontCopyReferences();
			// Instructs the setter methods to just keep track
			// of pointers passed into them rather than copying
			// the values.

		// These methods control the behavior when the container
		// is deleted.
		void	cascadeOnDelete();
			// Instructs the destructor to call delete or delete[]
			// (as appropriate) on whatever data is contained in
			// the class.
		void	dontCascadeOnDelete();
			// Instructs the destructor not to call delete or
			// delete[] (as appropriate) on whatever data is
			// contained in the class.  This is the default.

		virtual	bool	save(filedescriptor *fd);
		virtual	bool	save(unsigned char *buffer);
				// Writes a representation of the data in the
				// container out to the filedescriptor or
				// buffer.
				//
				// Returns true on success and false on failure.
		virtual	bool	load(filedescriptor *fd);
		virtual	bool	load(unsigned char *buffer);
				// Loads the data from filedescriptor or buffer.
				// Replaces data currently stored in the
				// container and implies copyReferences() and
				// cascadeOnDelete().
				//
				// Returns true on success and false on failure.

	#include <rudiments/private/container.h>
};

#endif
