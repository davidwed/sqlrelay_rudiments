// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

class memorypoolnode {
	friend class memorypool;
	private:
			memorypoolnode(size_t size);
			~memorypoolnode();

		unsigned char	*buffer;
		size_t		size;
		size_t		remaining;
		size_t		position;
};
