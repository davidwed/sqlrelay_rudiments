// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_CONNECTIONDATANODE_H
#define RUDIMENTS_CONNECTIONDATANODE_H

class connectiondatanode {
	friend class connectiondata;
	private:
			connectiondatanode(const char *key, const char *value);

		char	*key;
		char	*value;

		connectiondatanode	*next;
		connectiondatanode	*previous;
};

#include <rudiments/private/connectiondatanodeinlines.h>

#endif
