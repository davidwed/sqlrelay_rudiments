// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

class logdestnode {
	friend class logger;
	private:
		logdestination	*logdest;
		logdestnode	*next;
		logdestnode	*previous;
};
