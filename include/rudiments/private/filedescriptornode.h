// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.
#ifndef FILEDESCRIPTORNODE_H
#define FILEDESCRIPTORNODE_H

class filedescriptornode {
	friend class listener;
	private:
		int			fd;
		filedescriptornode	*next;
		filedescriptornode	*previous;
};

#endif
