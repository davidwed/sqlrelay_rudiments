// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

class memorypoolnode {
	friend class memorypool;
	private:
			memorypoolnode(unsigned long size);
			~memorypoolnode();

		char	*buffer;
		memorypoolnode	*next;
		memorypoolnode	*previous;
		unsigned long	size;
		unsigned long	remaining;
		unsigned long	position;
};
