// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_FILEDESCRIPTORNODE_H
#define RUDIMENTS_FILEDESCRIPTORNODE_H

class filedescriptornode {
	friend class listener;
	private:
		int			fd;
		filedescriptornode	*next;
		filedescriptornode	*previous;
};

#endif
